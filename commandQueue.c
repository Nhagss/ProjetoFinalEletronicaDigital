#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

char **split_sql(char *sql) {
    int capacity = 10; // Capacidade inicial
    char **substrings = malloc(capacity * sizeof(char *));
    if (!substrings) {
        printf("Erro ao alocar memória!\n");
        return NULL;
    }

    char *sub = strtok(sql, ";");
    int i = 0;

    while (sub) {
        // Realocar se atingirmos a capacidade
        if (i >= capacity - 1) {  // -1 para garantir espaço para NULL
            capacity *= 2;
            substrings = realloc(substrings, capacity * sizeof(char *));
            if (!substrings) {
                printf("Erro ao realocar memória!\n");
                return NULL;
            }
        }

        substrings[i] = malloc(strlen(sub) + 1);
        if (!substrings[i]) {
            printf("Erro ao alocar memória!\n");
            return NULL;
        }
        strcpy(substrings[i], sub);
        i++;
        sub = strtok(NULL, ";");
    }

    substrings[i] = NULL; // Adiciona NULL ao final para indicar fim da lista
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

// Function to remove extra spaces
void trim(char *str) {
    int i, start = 0, end = strlen(str) - 1;
    while (str[start] == ' ') start++;
    while (end > start && (str[end] == ' ' || str[end] == ';')) end--;
    for (i = 0; start <= end; i++, start++) str[i] = str[start];
    str[i] = '\0';
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

int execute_person_command(Command *cmd, PersonList *person_list) {

}

int execute_pet_command(Command *cmd, PetList *pet_list) {

}

int execute_pt_command(Command *cmd, PetTypeList *pt_list) {

}

int execute_commmand(Command *cmd, PetTypeList *pt_list, PetList *pet_list, PersonList *person_list) {
    if(!strcmp(cmd->table, "people")){
        execute_person_command(cmd, person_list);
    } else if(!strcmp(cmd->table, "pets")){
        execute_pet_command(cmd, pet_list);

    } else if(!strcmp(cmd->table, "pet_types")){
        execute_pt_command(cmd, pt_list);
    } else {
        printf("Unexpected table and error on parse_command");
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
void execute_and_free_command_queue(CommandQueue *command_queue) {
    Command *cmd;
    while ((cmd = dequeue_command(command_queue)) != NULL) {
        printf("Processing command:\n");
        print_command(cmd); ///MARK: CHANGE THIS TO A PROCESS COMMAND THAT WILL CALL A FUNCTION RELATIVE TO THE COMMAND SELECTED
        // Free the command after processing
        free_command(cmd);
    }
}


int testAF() {
    CommandQueue *command_queue = malloc(sizeof(CommandQueue));
    if (!command_queue) {
        printf("Error allocating memory for command queue\n");
        return 1;
    }
    initialize_command_queue(command_queue);

    // Test SQL commands
    char *test_sqls[] = {
            "insert into people (name, age) values ('John', '30');", // Valid INSERT
            "update people set age = '31' where name = 'John';",     // Valid UPDATE
            "delete from people where code = 10;",                   // Valid DELETE
            "select * from people order by age;",                    // Valid SELECT
            "insert into pets (name, type) values ('Buddy', 'Dog');", // Valid INSERT (different table)
            "update pets set type = 'Cat' where name = 'Buddy';",     // Valid UPDATE (different table)
            "delete from pets where code = 5;",                      // Valid DELETE (different table)
            "select * from pets order by name;",                     // Valid SELECT (different table)
            "insert into invalid_table (name) values ('Test');",     // Invalid table
            "update invalid_table set name = 'Test';",               // Invalid table
            "delete from invalid_table where code = 1;",             // Invalid table
            "select * from invalid_table;",                          // Invalid table
            "invalid SQL command;",                                  // Invalid SQL
            NULL // Sentinel to mark the end of the array
    };

    // Queue and process all commands
    queue_commands(test_sqls, command_queue);

    // Execute and free the command queue
    execute_and_free_command_queue(command_queue);

    // Free the command queue itself
    free(command_queue);

    return 0;
}

// Main function for testing
// Example fix for double free in main function
int main() {
    CommandQueue *command_queue = malloc(sizeof(CommandQueue));
    initialize_command_queue(command_queue);

    // Use a modifiable char array (not a string literal)
    char test_sql[] = "insert into people (name, age) values ('John', '30'); update people set age = '31' where name = 'John'; delete from people where code = 10; select * from people order by age; insert into pets (name, type) values ('Buddy', 'Dog'); update pets set type = 'Cat' where name = 'Buddy'; delete from pets where code = 5; select * from pets order by name; insert into invalid_table (name) values ('Test'); update invalid_table set name = 'Test'; delete from invalid_table where code = 1; select * from invalid_table; invalid SQL command; NULL";

    char **test_sqls = split_sql(test_sql);
    queue_commands(test_sqls, command_queue);

    // Free only once
    if (test_sqls) {
        for (int i = 0; test_sqls[i] != NULL; i++) {
            free(test_sqls[i]);
        }
        free(test_sqls);
    }

    free(command_queue);
    return 0;
}
/*
int main() {
    CommandQueue *command_queue = malloc(sizeof(CommandQueue));
    if (!command_queue) {
        printf("Error allocating memory for command queue\n");
        return 1;
    }
    initialize_command_queue(command_queue);

    // Test SQL commands
    char *test_sqls[] = {
            "insert into people (name, age) values ('John', '30');", // Valid INSERT
            "update people set age = '31' where name = 'John';",     // Valid UPDATE
            "delete from people where code = 10;",                   // Valid DELETE
            "select * from people order by age;",                    // Valid SELECT
            "insert into pets (name, type) values ('Buddy', 'Dog');", // Valid INSERT (different table)
            "update pets set type = 'Cat' where name = 'Buddy';",     // Valid UPDATE (different table)
            "delete from pets where code = 5;",                      // Valid DELETE (different table)
            "select * from pets order by name;",                     // Valid SELECT (different table)
            "insert into invalid_table (name) values ('Test');",     // Invalid table
            "update invalid_table set name = 'Test';",               // Invalid table
            "delete from invalid_table where code = 1;",             // Invalid table
            "select * from invalid_table;",                          // Invalid table
            "invalid SQL command;",                                  // Invalid SQL
            NULL // Sentinel to mark the end of the array
    };

    printf("Starting to queue commands...\n");

    // Queue and process all commands
    queue_commands(test_sqls, command_queue);

    printf("Finished queueing commands. Executing and freeing the queue...\n");

    // Execute and free the command queue
    execute_and_free_command_queue(command_queue);

    printf("Command queue freed. Exiting program.\n");

    // Free the command queue itself
    free(command_queue);

    return 0;
}
 */