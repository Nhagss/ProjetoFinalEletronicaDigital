#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "person.h"
#include "pet.h"
#include "petType.h"
//*********** CommandQueue.C ***************
#define MAX_KEYS 10
#define MAX_LEN 256

// Structure to store a SQL command
typedef struct {
    char *method;           // Method: insert, update, delete, search
    char *table;            // Table selected
    char **keys;            // Table fields
    char **values;          // Corresponding values
    char *extra;            // Extra command part (ORDER BY, WHERE, etc.)
    char *error;            // Error message
    int key_count;          // Number of keys
} Command;

// Command Node
typedef struct CommandNode {
    Command *cmd;
    struct CommandNode *next;
} CommandNode;

// Command Stack
typedef struct {
    CommandNode *head;
    CommandNode *tail;
} CommandQueue;

// Function to remove extra spaces
void trim(char *str) {
    int i, start = 0, end = strlen(str) - 1;
    while (str[start] == ' ') start++;
    while (end > start && (str[end] == ' ' || str[end] == ';')) end--;
    for (i = 0; start <= end; i++, start++) str[i] = str[start];
    str[i] = '\0';
}

void trim_whitespace(char *str) {
    if (!str) return;

    char *end;
    // Trim leading
    while (isspace((unsigned char)*str)) str++;

    // Trim trailing
    end = str + strlen(str) - 1;
    while (end >= str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
}

char **split_sql(char *sql) {
    int capacity = 10;
    char **substrings = malloc(capacity * sizeof(char *));
    if (!substrings) {
        fprintf(stderr, "Erro ao alocar memória!\n");
        return NULL;
    }

    char *sub = strtok(sql, ";");
    int i = 0;

    while (sub) {
        // Trim and check for empty strings
        trim_whitespace(sub);
        if (strlen(sub) == 0) {
            sub = strtok(NULL, ";\n");
            continue;
        }

        // Realloc check
        if (i >= capacity - 1) {
            capacity *= 2;
            char **temp = realloc(substrings, capacity * sizeof(char *));
            if (!temp) {
                fprintf(stderr, "Erro ao realocar memória!\n");
                for (int j = 0; j < i; j++) free(substrings[j]);
                free(substrings);
                return NULL;
            }
            substrings = temp;
        }

        // Allocate and copy
        substrings[i] = strdup(sub);
        if (!substrings[i]) {
            fprintf(stderr, "Erro ao alocar memória!\n");
            for (int j = 0; j < i; j++) free(substrings[j]);
            free(substrings);
            return NULL;
        }

        i++;
        sub = strtok(NULL, ";");
    }

    substrings[i] = NULL;
    return substrings;
}

// Function to initialize a command
// Example fix for memory allocation check
void initialize_command(Command *cmd) {
    // Allocate memory for keys and values arrays
    cmd->keys = malloc((MAX_KEYS + 1) * sizeof(char *)); // +1 for sentinel
    cmd->values = malloc((MAX_KEYS + 1) * sizeof(char *)); // +1 for sentinel

    if (!cmd->keys || !cmd->values) {
        printf("Error allocating memory for keys or values\n");
        return;
    }

    // Initialize keys and values with empty strings
    for (int i = 0; i < MAX_KEYS; i++) {
        cmd->keys[i] = malloc(MAX_LEN * sizeof(char));
        cmd->values[i] = malloc(MAX_LEN * sizeof(char));
        if (!cmd->keys[i] || !cmd->values[i]) {
            printf("Error allocating memory for key or value %d\n", i);
            return;
        }
        cmd->keys[i][0] = '\0';
        cmd->values[i][0] = '\0';
    }

    // Add sentinel (NULL) at the end
    cmd->keys[MAX_KEYS] = NULL;
    cmd->values[MAX_KEYS] = NULL;

    // Initialize other fields
    cmd->method = malloc(10 * sizeof(char));
    cmd->table = malloc(MAX_LEN * sizeof(char));
    cmd->extra = malloc(MAX_LEN * sizeof(char));
    cmd->error = malloc(MAX_LEN * sizeof(char));
    cmd->key_count = 0;

    if (!cmd->method || !cmd->table || !cmd->extra || !cmd->error) {
        printf("Error allocating memory for command fields\n");
        return;
    }

    cmd->method[0] = '\0';
    cmd->table[0] = '\0';
    cmd->extra[0] = '\0';
    cmd->error[0] = '\0';
}

// Function to extract values between parentheses
void extract_between_parentheses(const char *str, char *dest) {
    const char *start = strchr(str, '(');
    const char *end = strchr(str, ')');
    if (start && end && end > start) {
        strncpy(dest, start + 1, end - start - 1);
        dest[end - start - 1] = '\0';
    } else {
        dest[0] = '\0';
    }
}

// Function to split a string by commas and store in an array
int split_by_comma(const char *str, char **arr) {
    char temp[MAX_LEN];
    strcpy(temp, str);
    int count = 0;
    char *token = strtok(temp, ",");

    while (token && count < MAX_KEYS) {
        trim(token);
        arr[count] = malloc((strlen(token) + 1) * sizeof(char));
        if (!arr[count]) {
            printf("Error allocating memory for token %d\n", count);
            return -1; // Error
        }
        strcpy(arr[count], token);
        count++;
        token = strtok(NULL, ",");
    }

    // Add sentinel (NULL) at the end
    arr[count] = NULL;
    return count; // Return the number of tokens
}

// Function to parse an SQL command and fill it into the Command struct
int parse_command(char *sql, Command *cmd) {
    initialize_command(cmd); // Initialize the command with sentinels
    trim(sql); // Remove leading/trailing spaces

    // List of valid tables
    char *valid_tables[] = {"people", "pets", "pet_types"};
    int num_tables = sizeof(valid_tables) / sizeof(valid_tables[0]);

    // Check if the SQL command references a valid table
    char *table_ext = NULL;
    for (int i = 0; i < num_tables; i++) {
        table_ext = strstr(sql, valid_tables[i]);
        if (table_ext) {
            strcpy(cmd->table, valid_tables[i]);
            break;
        }
    }
    if (!table_ext) {
        strcpy(cmd->error, "Invalid table");
        return 0; // Invalid table
    }

    // Parse INSERT command
    if (strncmp(sql, "insert into ", 12) == 0) {
        strcpy(cmd->method, "insert");

        char *values_ptr = strstr(sql, "values");
        if (!values_ptr) {
            strcpy(cmd->error, "Invalid INSERT command: missing 'values'");
            return 0; // Invalid command
        }

        *values_ptr = '\0'; // Split the SQL into columns and values
        values_ptr += 6; // Move past "values"

        char columns[MAX_LEN], values[MAX_LEN];
        extract_between_parentheses(sql + 12, columns); // Extract columns
        extract_between_parentheses(values_ptr, values); // Extract values

        // Split columns and values into keys and values arrays
        cmd->key_count = split_by_comma(columns, cmd->keys);
        split_by_comma(values, cmd->values);
    }

        // Parse UPDATE command
    else if (strncmp(sql, "update ", 7) == 0) {
        strcpy(cmd->method, "update");

        char *set_ptr = strstr(sql, " set ");
        char *where_ptr = strstr(sql, " where ");
        if (!set_ptr) {
            strcpy(cmd->error, "Invalid UPDATE command: missing 'set'");
            return 0; // Invalid command
        }

        *set_ptr = '\0'; // Split the SQL into table and set/where parts
        set_ptr += 5; // Move past " set "

        if (where_ptr) {
            *where_ptr = '\0'; // Split the SQL into set and where parts
            where_ptr += 7; // Move past " where "
            if (strncmp(where_ptr, "code =", 6) == 0) {
                strcpy(cmd->extra, where_ptr); // Store the WHERE clause
            } else {
                strcpy(cmd->error, "Invalid WHERE condition in UPDATE");
                return 0; // Invalid WHERE condition
            }
        }

        // Split the SET clause into key-value pairs
        char **columns_values = malloc(MAX_KEYS * sizeof(char *));
        for (int i = 0; i < MAX_KEYS; i++) {
            columns_values[i] = malloc(MAX_LEN * sizeof(char));
        }

        int count = split_by_comma(set_ptr, columns_values);
        for (int i = 0; i < count; i++) {
            char *equal = strchr(columns_values[i], '=');
            if (!equal) {
                strcpy(cmd->error, "Invalid SET clause in UPDATE");
                return 0; // Invalid format
            }
            *equal = '\0'; // Split into key and value
            trim(columns_values[i]); // Trim the key
            trim(equal + 1); // Trim the value
            strcpy(cmd->keys[i], columns_values[i]); // Store the key
            strcpy(cmd->values[i], equal + 1); // Store the value
        }
        cmd->key_count = count;

        // Free temporary memory
        for (int i = 0; i < MAX_KEYS; i++) {
            free(columns_values[i]);
        }
        free(columns_values);
    }

        // Parse DELETE command
    else if (strncmp(sql, "delete from ", 12) == 0) {
        strcpy(cmd->method, "delete");

        char *where_ptr = strstr(sql, " where ");
        if (where_ptr) {
            where_ptr += 7; // Move past " where "
            if (strncmp(where_ptr, "code =", 6) == 0) {
                strcpy(cmd->extra, where_ptr); // Store the WHERE clause
            } else {
                strcpy(cmd->error, "Invalid WHERE condition in DELETE");
                return 0; // Invalid WHERE condition
            }
        }
    }

        // Parse SELECT command
    else if (strncmp(sql, "select * from ", 14) == 0) {
        strcpy(cmd->method, "search");
        strcpy(cmd->keys[0], "*"); // Use "*" as the key
        strcpy(cmd->values[0], "*"); // Use "*" as the value
        cmd->key_count = 1;

        char *extra_ptr = strstr(sql, " order by ");
        if (extra_ptr) {
            strcpy(cmd->extra, extra_ptr); // Store the ORDER BY clause
        } else {
            char *extra_ptr = strstr(sql, " where code = ");
            if(extra_ptr) {
                strcpy(cmd->extra, extra_ptr); // Store the ORDER BY clause

            }
        }
    }

        // Invalid command
    else {
        strcpy(cmd->error, "Invalid SQL command");
        return 0; // Invalid command
    }

    return 1; // Valid command
}

// Function to print a formatted command
void print_command(const Command *cmd) {
    printf("\n+ Method: %s\n", cmd->method);
    printf("+ Keys: [ ");
    for (int i = 0; cmd->keys[i] != NULL; i++) { // Use sentinel
        printf("%s ", cmd->keys[i]);
    }
    printf("]\n");

    printf("+ Values: [ ");
    for (int i = 0; cmd->values[i] != NULL; i++) { // Use sentinel
        printf("%s ", cmd->values[i]);
    }
    printf("]\n");

    if (strlen(cmd->extra) > 0) {
        printf("+ Extra: %s\n", cmd->extra);
    }
    printf("\n");
}

int queue_command(char *sql, CommandQueue *command_queue) {
    char *saved_sql = malloc(sizeof(char) * (strlen(sql) + 1)); // +1 for null terminator
    if (!saved_sql) {
        printf("Error allocating memory\n");
        return 0;
    }
    strcpy(saved_sql, sql);

    Command *cmd = malloc(sizeof(Command));
    if (!cmd) {
        free(saved_sql); // Cleanup on failure
        printf("Error allocating memory\n");
        return 0;
    }

    if (parse_command(sql, cmd)) {
        CommandNode *cmd_node = malloc(sizeof(CommandNode));
        if(!cmd_node) {
            free(cmd);
            printf("Error allocating memory\n");
            return 0;
        }

        cmd_node->cmd = cmd;
        cmd_node->next = NULL;

        printf("+ Valid Command: %s", saved_sql);
        free(saved_sql);
        if(command_queue->head || command_queue->tail) {
            command_queue->tail->next = cmd_node;
            command_queue->tail = cmd_node;

        } else {
            command_queue->head = cmd_node;
            command_queue->tail = cmd_node;
        }
        print_command(cmd);
        return(1);
    } else {
        printf("! Invalid command: %s\n", saved_sql);
        free(saved_sql);
        printf("! Reason: %s\n\n", cmd->error);
        free(cmd);
        return(0);
    }
}

// Initializes a command queue
void initialize_command_queue(CommandQueue *command_queue) {
    command_queue->head = NULL;
    command_queue->tail = NULL;
}

// Receives a sql-like command array and add it to the queue
void queue_commands(char **sql, CommandQueue *command_queue) {
    for(int i = 0; sql[i]; i++) {
        queue_command(sql[i], command_queue);
    }
}

// Function to dequeue a command from the queue
Command* dequeue_command(CommandQueue *command_queue) {
    if (command_queue->head == NULL) {
        printf("Queue is empty, nothing to dequeue.\n");
        return NULL;
    }

    CommandNode *temp = command_queue->head;
    Command *cmd = temp->cmd;

    // Update the head of the queue
    command_queue->head = command_queue->head->next;

    // If the queue becomes empty after dequeue, update the tail as well
    if (command_queue->head == NULL) {
        command_queue->tail = NULL;
    }

    // Free the node but not the command itself, as it is returned
    free(temp);

    return cmd;
}

// Helper: remove aspas simples do início e fim da string (se existirem)
void remove_quotes(char *str) {
    int len = strlen(str);
    if (len >= 2 && str[0] == '\'' && str[len - 1] == '\'') {
        // Desloca para a esquerda para remover a primeira aspa
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
}

// Execute a command on the Person table
int execute_person_command(Command *cmd, PersonList *person_list, PetList *pet_list) {
    if (strcmp(cmd->method, "insert") == 0) {
        int code = -1;
        char name[50] = "", phone[15] = "", address[100] = "", birthdate[11] = "";
        for (int i = 0; i < cmd->key_count; i++) {
            if (strcmp(cmd->keys[i], "code") == 0) {
                remove_quotes(cmd->values[i]);
                code = atoi(cmd->values[i]);
            } else if (strcmp(cmd->keys[i], "name") == 0) {
                strcpy(name, cmd->values[i]);
                remove_quotes(name);
            } else if (strcmp(cmd->keys[i], "phone") == 0) {
                strcpy(phone, cmd->values[i]);
                remove_quotes(phone);
            } else if (strcmp(cmd->keys[i], "address") == 0) {
                strcpy(address, cmd->values[i]);
                remove_quotes(address);
            } else if (strcmp(cmd->keys[i], "birthdate") == 0) {
                strcpy(birthdate, cmd->values[i]);
                remove_quotes(birthdate);
            }
        }
        // Required fields: code, name, birthdate
        if (code == -1 || strlen(name) == 0 || strlen(birthdate) == 0) {
            strcpy(cmd->error, "Missing required fields for person insertion.");
            return 0;
        }
        Person *p = insert_bottom_person(person_list, code, name, phone, address, birthdate);
        if (p == NULL) {
            strcpy(cmd->error, "Failed to insert person (duplicate code or memory error).");
            return 0;
        }
        return 1;
    } else if (strcmp(cmd->method, "update") == 0) {
        int current_code = -1;
        char name[50] = "", phone[15] = "", address[100] = "", birthdate[11] = "";
        // Expect extra: \"code = 'X'\" to indicate which person to update
        if (cmd->extra && strlen(cmd->extra) > 0) {
            char *equal = strchr(cmd->extra, '=');
            if (equal) {
                char numStr[20] = {0};
                strcpy(numStr, equal + 1);
                remove_quotes(numStr);
                current_code = atoi(numStr);
            }
        }
        if (current_code == -1) {
            strcpy(cmd->error, "Missing or invalid code in WHERE clause for person update.");
            return 0;
        }

        for (int i = 0; i < cmd->key_count; i++) {
            if (strcmp(cmd->keys[i], "name") == 0) {
                strcpy(name, cmd->values[i]);
                remove_quotes(name);
            } else if (strcmp(cmd->keys[i], "phone") == 0) {
                strcpy(phone, cmd->values[i]);
                remove_quotes(phone);
            } else if (strcmp(cmd->keys[i], "address") == 0) {
                strcpy(address, cmd->values[i]);
                remove_quotes(address);
            } else if (strcmp(cmd->keys[i], "birthdate") == 0) {
                strcpy(birthdate, cmd->values[i]);
                remove_quotes(birthdate);
            }
        }
        Person *current_p = search_person(person_list, current_code);
        if(!strcmp(name, ""))
            strcpy(name, current_p->name);
        if(!strcmp(phone, ""))
            strcpy(phone, current_p->phone);
        if(!strcmp(address, ""))
            strcpy(address, current_p->address);
        if(!strcmp(birthdate, ""))
            strcpy(birthdate, current_p->birthdate);
        Person *p = update_person(person_list, current_code, name, phone, address, birthdate);
        if (p == NULL) {
            strcpy(cmd->error, "Failed to update person.");
            return 0;
        }
        return 1;
    } else if (strcmp(cmd->method, "delete") == 0) {
        int code = -1;
        if (cmd->extra && strlen(cmd->extra) > 0) {
            char *equal = strchr(cmd->extra, '=');
            if (equal) {
                char numStr[20] = {0};
                strcpy(numStr, equal + 1);
                remove_quotes(numStr);
                code = atoi(numStr);
            }
        }
        if (code == -1) {
            strcpy(cmd->error, "Missing or invalid code in WHERE clause for person deletion.");
            return 0;
        }
        int result = remove_person(person_list, pet_list, code);
        if (!result) {
            strcpy(cmd->error, "Failed to delete pet.");
            return 0;
        }
        return 1;
    } else if (strcmp(cmd->method, "search") == 0) {

        if (cmd->extra && strlen(cmd->extra) > 0) {
            if (strstr(cmd->extra, "order by name")) {
                person_print_order_by_name(person_list);
            } else {
                char *equal = strchr(cmd->extra, '=');
                int code = -1;
                if (equal) {
                    char numStr[20] = {0};
                    strcpy(numStr, equal + 1);
                    remove_quotes(numStr);
                    code = atoi(numStr);
                }
                if (code != -1) {
                    Person *person = search_person(person_list, code);
                    if (person)
                        printf("Found person: Code: %d, Name: %s\n", person->code, person->name);
                    else
                        printf("Person with code %d not found.\n", code);
                }
            }
        } else {
            print_list_person(*person_list);

        }
        return 1;
    }
    strcpy(cmd->error, "Unknown method for person command.");
    return 0;
}

// Execute command on the Pet table
int execute_pet_command(Command *cmd, PetList *pet_list, PetTypeList *pt_list, PersonList *person_list) {
    if (strcmp(cmd->method, "insert") == 0) {
        int code = -1, person_code = -1, type_code = -1;
        char name[50] = "";
        for (int i = 0; i < cmd->key_count; i++) {
            if (strcmp(cmd->keys[i], "code") == 0) {
                remove_quotes(cmd->values[i]);
                code = atoi(cmd->values[i]);
            } else if (strcmp(cmd->keys[i], "person_code") == 0) {
                remove_quotes(cmd->values[i]);
                person_code = atoi(cmd->values[i]);
            } else if (strcmp(cmd->keys[i], "name") == 0) {
                strcpy(name, cmd->values[i]);
                remove_quotes(name);
            } else if (strcmp(cmd->keys[i], "type_code") == 0) {
                remove_quotes(cmd->values[i]);
                type_code = atoi(cmd->values[i]);
            }
        }
        if (code == -1 || person_code == -1 || type_code == -1 || strlen(name) == 0) {
            strcpy(cmd->error, "Missing required fields for pet insertion.");
            return 0;
        }
        // Validate that person and pet type exist
        Person *p = search_person(person_list, person_code);
        if (p == NULL) {
            strcpy(cmd->error, "Invalid person_code for pet insertion.");
            return 0;
        }
        PetType *pt = search_pt(pt_list, type_code);
        if (pt == NULL) {
            strcpy(cmd->error, "Invalid type_code for pet insertion.");
            return 0;
        }
        Pet *pet = insert_bottom_pet(pet_list, pt_list, person_list, code, person_code, name, type_code);
        if (pet == NULL) {
            strcpy(cmd->error, "Failed to insert pet (duplicate code or memory error).");
            return 0;
        }
        return 1;
    } else if (strcmp(cmd->method, "update") == 0) {
        int current_code = -1, person_code = -1, type_code = -1;
        char name[50] = "";

        // Obter o código atual do pet
        if (cmd->extra && strlen(cmd->extra) > 0) {
            char *equal = strchr(cmd->extra, '=');
            if (equal) {
                char numStr[20] = {0};
                strcpy(numStr, equal + 1);
                remove_quotes(numStr);
                current_code = atoi(numStr);
            }
        }
        if (current_code == -1) {
            strcpy(cmd->error, "Missing or invalid code in WHERE clause for pet update.");
            return 0;
        }

        for (int i = 0; i < cmd->key_count; i++) {
            if (strcmp(cmd->keys[i], "person_code") == 0) {
                remove_quotes(cmd->values[i]);
                person_code = atoi(cmd->values[i]);
            } else if (strcmp(cmd->keys[i], "name") == 0) {
                strcpy(name, cmd->values[i]);
                remove_quotes(name);
            } else if (strcmp(cmd->keys[i], "type_code") == 0) {
                remove_quotes(cmd->values[i]);
                type_code = atoi(cmd->values[i]);
            }
        }

        // Buscar o pet atual
        Pet *current_pet = search_pet(pet_list, current_code);
        if (current_pet == NULL) {
            strcpy(cmd->error, "Pet not found.");
            return 0;
        }

        // Preencher campos não especificados com os valores atuais
        if (strlen(name) == 0) strcpy(name, current_pet->name);
        if (person_code == -1) person_code = current_pet->person_code;
        if (type_code == -1) type_code = current_pet->type_code;

        // Validar códigos de pessoa e tipo de pet
        if (search_person(person_list, person_code) == NULL) {
            strcpy(cmd->error, "Invalid person_code for pet update.");
            return 0;
        }
        if (search_pt(pt_list, type_code) == NULL) {
            strcpy(cmd->error, "Invalid type_code for pet update.");
            return 0;
        }

        // Atualizar pet
        Pet *pet = update_pet(pet_list, current_code, person_code, name, type_code);
        if (pet == NULL) {
            strcpy(cmd->error, "Failed to update pet.");
            return 0;
        }
        return 1;
    } else if (strcmp(cmd->method, "delete") == 0) {
        int code = -1;
        if (cmd->extra && strlen(cmd->extra) > 0) {
            char *equal = strchr(cmd->extra, '=');
            if (equal) {
                char numStr[20] = {0};
                strcpy(numStr, equal + 1);
                remove_quotes(numStr);
                code = atoi(numStr);
            }
        }
        if (code == -1) {
            strcpy(cmd->error, "Missing or invalid code in WHERE clause for pet deletion.");
            return 0;
        }
        int result = remove_pet(pet_list, code);
        if (!result) {
            strcpy(cmd->error, "Failed to delete pet.");
            return 0;
        }
        return 1;
    } else if (strcmp(cmd->method, "search") == 0) {

        if (cmd->extra && strlen(cmd->extra) > 0) {
            if (strstr(cmd->extra, "order by name")) {
                pet_print_order_by_name(pet_list);
            } else {
                char *equal = strchr(cmd->extra, '=');
                int code = -1;
                if (equal) {
                    char numStr[20] = {0};
                    strcpy(numStr, equal + 1);
                    remove_quotes(numStr);
                    code = atoi(numStr);
                }
                if (code != -1) {
                    Pet *pet = search_pet(pet_list, code);
                    if (pet)
                        printf("Found pet: Code: %d, Name: %s\n", pet->code, pet->name);
                    else
                        printf("Pet with code %d not found.\n", code);
                }
            }
        } else {
            print_list_pet(*pet_list);

        }
        return 1;
    }
    strcpy(cmd->error, "Unknown method for pet command.");
    return 0;
}

// Execute command on the Pet Type table
int execute_pt_command(Command *cmd, PetTypeList *pt_list, PetList *pet_list) {
    if (strcmp(cmd->method, "insert") == 0) {
        int code = -1;
        char name[50] = "";
        for (int i = 0; i < cmd->key_count; i++) {
            if (strcmp(cmd->keys[i], "code") == 0) {
                remove_quotes(cmd->values[i]);
                code = atoi(cmd->values[i]);
            } else if (strcmp(cmd->keys[i], "name") == 0) {
                strcpy(name, cmd->values[i]);
                remove_quotes(name);
            }
        }
        if (code == -1 || strlen(name) == 0) {
            strcpy(cmd->error, "Missing required fields for pet type insertion.");
            return 0;
        }
        PetType *pt = insert_bottom_pt(pt_list, code, name);
        if (pt == NULL) {
            strcpy(cmd->error, "Failed to insert pet type (duplicate code or memory error).");
            return 0;
        }
        return 1;
    } else if (strcmp(cmd->method, "update") == 0) {
        int current_code = -1;
        char name[50] = "";

        // Obter o código atual do tipo de pet
        if (cmd->extra && strlen(cmd->extra) > 0) {
            char *equal = strchr(cmd->extra, '=');
            if (equal) {
                char numStr[20] = {0};
                strcpy(numStr, equal + 1);
                remove_quotes(numStr);
                current_code = atoi(numStr);
            }
        }
        if (current_code == -1) {
            strcpy(cmd->error, "Missing or invalid code in WHERE clause for pet type update.");
            return 0;
        }

        for (int i = 0; i < cmd->key_count; i++) {
            if (strcmp(cmd->keys[i], "name") == 0) {
                strcpy(name, cmd->values[i]);
                remove_quotes(name);
            }
        }

        // Buscar o tipo de pet atual
        PetType *current_pt = search_pt(pt_list, current_code);
        if (current_pt == NULL) {
            strcpy(cmd->error, "Pet type not found.");
            return 0;
        }

        // Preencher campos não especificados com os valores atuais
        if (strlen(name) == 0) strcpy(name, current_pt->name);

        // Atualizar tipo de pet
        PetType *pt = update_pt(pt_list, current_code, name);
        if (pt == NULL) {
            strcpy(cmd->error, "Failed to update pet type.");
            return 0;
        }
        return 1;
    } else if (strcmp(cmd->method, "delete") == 0) {
        int code = -1;
        if (cmd->extra && strlen(cmd->extra) > 0) {
            char *equal = strchr(cmd->extra, '=');
            if (equal) {
                char numStr[20] = {0};
                strcpy(numStr, equal + 1);
                remove_quotes(numStr);
                code = atoi(numStr);
            }
        }
        if (code == -1) {
            strcpy(cmd->error, "Missing or invalid code in WHERE clause for pet type deletion.");
            return 0;
        }
        int result = remove_pt(pt_list, pet_list, code);
        if (!result) {
            strcpy(cmd->error, "Failed to delete pet type.");
            return 0;
        }
        return 1;
    } else if (strcmp(cmd->method, "search") == 0) {

        if (cmd->extra && strlen(cmd->extra) > 0) {
            if (strstr(cmd->extra, "order by name")) {
                pt_print_order_by_name(pt_list);
            } else {
                char *equal = strchr(cmd->extra, '=');
                int code = -1;
                if (equal) {
                    char numStr[20] = {0};
                    strcpy(numStr, equal + 1);
                    remove_quotes(numStr);
                    code = atoi(numStr);
                }
                if (code != -1) {
                    PetType *pt = search_pt(pt_list, code);
                    if (pt)
                        printf("Found pet type: Code: %d, Name: %s\n", pt->code, pt->name);
                    else
                        printf("Pet type with code %d not found.\n", code);
                }
            }
        } else {
            print_list_pt(*pt_list);

        }
        return 1;
    }
    strcpy(cmd->error, "Unknown method for pet type command.");
    return 0;
}

// General command execution: routes command to appropriate table executor
int execute_commmand(Command *cmd, PetTypeList *pt_list, PetList *pet_list, PersonList *person_list) {
    if (!strcmp(cmd->table, "people")) {
        return execute_person_command(cmd, person_list, pet_list);
    } else if (!strcmp(cmd->table, "pets")) {
        return execute_pet_command(cmd, pet_list, pt_list, person_list);
    } else if (!strcmp(cmd->table, "pet_types")) {
        return execute_pt_command(cmd, pt_list, pet_list);
    } else {
        printf("Unexpected table and error on parse_command\n");
        return 0;
    }
}


void free_command(Command *cmd) {
    if (!cmd) return;

    free(cmd->method);
    free(cmd->table);

    // Free keys
    for (int i = 0; cmd->keys[i] != NULL; i++) { // Stop at sentinel
        free(cmd->keys[i]);
    }
    free(cmd->keys);

    // Free values
    for (int i = 0; cmd->values[i] != NULL; i++) { // Stop at sentinel
        free(cmd->values[i]);
    }

    free(cmd->values);

    free(cmd->extra);
    free(cmd->error);
    free(cmd);
}

// Function to free the command queue
void execute_and_free_command_queue(PersonList *person_list, PetTypeList *pt_list, PetList *pet_list, CommandQueue *command_queue) {
    Command *cmd;
    while ((cmd = dequeue_command(command_queue)) != NULL) {
        printf("Processing command:\n");
        execute_commmand(cmd, pt_list, pet_list, person_list);
        // Free the command after processing
        if(strcmp(cmd->error, "")) {
            printf("ERROR: %s\n",cmd->error);
        }
        printf("\n");
        free_command(cmd);
    }
}

void print_and_free_command_queue(CommandQueue *command_queue) {
    Command *cmd;
    while ((cmd = dequeue_command(command_queue)) != NULL) {
        printf("Processing command:\n");
        print_command(cmd); ///MARK: CHANGE THIS TO A PROCESS COMMAND THAT WILL CALL A FUNCTION RELATIVE TO THE COMMAND SELECTED
        // Free the command after processing
        free_command(cmd);
    }
}
void normalize_line_endings(char *str) {
    if (!str) return;

    char *src = str;
    char *dst = str;

    while (*src) {
        if (*src == '\r') {
            // Skip CR characters
            src++;
            src++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}
char *read_file_to_string(const char *filename) {
    FILE *file = fopen(filename, "rb");  // Open in binary mode for accurate size calculation
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    // Determine file size
    if (fseek(file, 0, SEEK_END) != 0) {
        perror("Error seeking file");
        fclose(file);
        return NULL;
    }

    long file_size = ftell(file);
    if (file_size == -1) {
        perror("Error getting file size");
        fclose(file);
        return NULL;
    }

    rewind(file);

    // Allocate memory for file content + null terminator
    char *buffer = (char *)malloc(file_size + 1);
    if (!buffer) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    // Read file content
    size_t bytes_read = fread(buffer, 1, file_size, file);
    if (bytes_read != (size_t)file_size) {
        perror("Error reading file");
        free(buffer);
        fclose(file);
        return NULL;
    }

    // Null-terminate the string
    buffer[bytes_read] = '\0';

    if (fclose(file) != 0) {
        perror("Error closing file");
        free(buffer);
        return NULL;
    }

    normalize_line_endings(buffer);

    return buffer;
}