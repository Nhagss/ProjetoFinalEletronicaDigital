#include "person.h"
#include "petType.h"
#include "pet.h"
// Initializes an empty list
void initialize_list_person(PersonList *list) {
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

// Save person list to file
void save_list_person(PersonList *list, const char *filename) {
    printf("saving");
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file for writing\n");
        return;
    }

    fwrite(&list->count, sizeof(int), 1, file);

    Person *current = list->head;
    while (current) {
        fwrite(current, sizeof(Person) - sizeof(Person*), 1, file); // Save without pointers
        current = current->next;
    }

    fclose(file);
}

// Read file for person list
void load_list_person(PersonList *list, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("File does not exist. Starting with an empty list.\n");
        return;
    }

    fread(&list->count, sizeof(int), 1, file);
    list->head = list->tail = NULL;

    for (int i = 0; i < list->count; i++) {
        Person *newNode = (Person *)malloc(sizeof(Person));
        fread(newNode, sizeof(Person) - sizeof(Person*), 1, file);
        newNode->prev = newNode->next = NULL;

        if (!list->head) {
            list->head = list->tail = newNode;
        } else {
            newNode->prev = list->tail;
            list->tail->next = newNode;
            list->tail = newNode;
        }
    }

    fclose(file);
}

// Searches for a person and returns it
Person *search_person(PersonList *list, int code) {
    if (!list) {
        printf("WARN: List is empty\n");
        return NULL;
    }

    Person *cursor = list->head;
    while (cursor->code != code && cursor->next) {
        cursor = cursor->next;
    }
    if (cursor->code != code) {
        printf("WARN: Person not found\n");
        return NULL;
    }
    printf("INFO: Found person with code %d\n", code);
    return cursor;
}

// Checks if code already exists in the list
int check_code_person(PersonList list, int code) {
    if (search_person(&list, code)) {
        return 0;
    }
    return 1;
}

// Inserts a person at the top of the list
Person *insert_top_person(PersonList *list, int code, char *name, char *phone, char *address, char *birthdate) {
    Person *new = malloc(sizeof(Person));
    if (!new) {
        printf("ERROR: Failed to allocate memory\n");
        return NULL;
    }

    new->code = code;
    strcpy(new->name, name);
    strcpy(new->phone, phone);
    strcpy(new->address, address);
    strcpy(new->birthdate, birthdate);

    if (!list->head || !list->tail) {
        list->head = new;
        list->tail = new;
        list->head->prev = NULL;
        list->tail->next = NULL;
        printf("CREATION: Created new person list!\n");
    } else {
        if (!check_code_person(*list, code)) {
            printf("WARN: A person with this code already exists!\n");
            return NULL;
        }
        new->prev = NULL;
        new->next = list->head;
        list->head->prev = new;
        list->head = new;
        printf("CREATION: Added new person on top!\n");
    }
    list->count++;
    return new;
}

// Inserts a person at the bottom of the list
Person *insert_bottom_person(PersonList *list, int code, char *name, char *phone, char *address, char *birthdate) {
    Person *new = malloc(sizeof(Person));
    if (!new) {
        printf("ERROR: Failed to allocate memory\n");
        return NULL;
    }

    new->code = code;
    strcpy(new->name, name);
    strcpy(new->phone, phone);
    strcpy(new->address, address);
    strcpy(new->birthdate, birthdate);

    if (!list->head || !list->tail) {
        list->head = new;
        list->tail = new;
        list->head->prev = NULL;
        list->tail->next = NULL;
        printf("CREATION: Created new person list!\n");
    } else {
        if (!check_code_person(*list, code)) {
            printf("WARN: A person with this code already exists!\n");
            return NULL;
        }
        new->next = NULL;
        new->prev = list->tail;
        list->tail->next = new;
        list->tail = new;
        printf("CREATION: Added new person on bottom!\n");
    }
    list->count++;
    return new;
}

Pet *search_pet_by_person(PetList *list, int person_code) {
    if (!list || !list->head) {
        printf("WARN: Pet list is empty\n");
        return NULL;
    }

    Pet *cursor = list->head;
    while (cursor && cursor->person_code != person_code) {
        cursor = cursor->next;
    }

    if (!cursor) {
        printf("WARN: Pet not found\n");
        return NULL;
    }

    printf("INFO: Found pet with code %d\n", person_code);
    return cursor;
}

int remove_pets_from_person(PetList *list, int code) {
    int count = 0;
    while(1) {
        Pet *found_pet = search_pet_by_person(list, code);
        if(found_pet) {
            remove_pet(list, found_pet->code);
            count++;
        } else {
            break;
        }
    }
    printf("Removed %d pets from person: %d", count, code);
}


// Deletes a person from the list
int remove_person(PersonList *list, PetList *petList, int code) {
    if (!list) {
        printf("WARN: Empty list\n");
        return 0;
    }

    Person *toDelete = NULL;
    if (code == list->head->code) {
        toDelete = list->head;
        if(toDelete->next) {
            toDelete->next->prev = NULL;
            list->head = toDelete->next;
        }

        printf("DELETION: Deleted person on top\n");
        remove_pets_from_person(petList, code);
    } else if (code == list->tail->code && list->tail->code != list->head->code) {
        toDelete = list->tail;
        toDelete->prev->next = NULL;
        list->tail = toDelete->prev;
        printf("DELETION: Deleted person on bottom\n");
        remove_pets_from_person(petList, code);
    } else {
        toDelete = search_person(list, code);
        if (!toDelete) {
            printf("WARN: Person not found\n");
            return 0;
        }
        toDelete->next->prev = toDelete->prev;
        toDelete->prev->next = toDelete->next;
        printf("DELETION: Deleted person in the middle\n");
    }
    free(toDelete);
    list->count--;
    return 1;
}

// Updates a person's information
Person *update_person(PersonList *list, int code, char *name, char *phone, char *address, char *birthdate) {
    Person *target = search_person(list, code);
    if (!target) {
        printf("WARN: Person not found\n");
        return NULL;
    }
    strcpy(target->name, name);
    strcpy(target->phone, phone);
    strcpy(target->address, address);
    strcpy(target->birthdate, birthdate);
    return target;
}

// Prints the entire list
void print_list_person(PersonList list) {
    printf("<PRINTING PERSON LIST [%d]>\n\n", list.count);
    if (!list.head) {
        printf("Empty List\n");
        printf("\n<FINISHED PRINTING PERSON LIST/>\n\n");
        return;
    }

    Person *aux = list.head;
    int count = 0;
    printf("[Head: (%d) - Count: (%d)]\n", list.head->code, list.count);
    printf("--------------------------------\n");
    while (aux) {
        printf("[%d] %d: '%s' | Phone: %s | Address: %s | Birthdate: %s\n", count, aux->code, aux->name, aux->phone, aux->address, aux->birthdate);
        count++;
        aux = aux->next;
    }
    printf("--------------------------------\n\n");
    printf("[Tail: (%d)]\n", list.tail->code);
    printf("\n<FINISHED PRINTING PERSON LIST/>\n\n");
}

// Free all nodes in the list
void free_list_person(PersonList *list) {
    Person *current = list->head;
    while (current) {
        Person *next = current->next;
        free(current);
        current = next;
    }
    list->head = list->tail = NULL;
    list->count = 0;
}

PersonNode *person_create_node(Person *person) {
    PersonNode* new_person_node = (PersonNode*)malloc(sizeof(PersonNode));
    new_person_node->person = person;
    new_person_node->left = new_person_node->right = NULL;
    return new_person_node;
}

PersonNode *person_insert(PersonNode *root, Person *person) {
    if (!root) {
        return person_create_node(person);
    }

    if (strcmp(person->name, root->person->name) < 0) {
        root->left = person_insert(root->left, person);
    } else if (strcmp(person->name, root->person->name) > 0) {
        root->right = person_insert(root->right, person);
    }
    return root;
}

void person_inorderTraversal(PersonNode *root) {
    if (!root) return;
    person_inorderTraversal(root->left);
    printf("%s ", root->person->name);
    person_inorderTraversal(root->right);
}

void person_freeTree(PersonNode *root) {
    if (!root) return;
    person_freeTree(root->left);
    person_freeTree(root->right);
    free(root);
}

void person_print_order_by_name(PersonList *list) {
    PersonNode *root = NULL;
    Person *current = list->head;
    while (current) {
        root = person_insert(root, current);
        current = current->next;
    }

    printf("In-order traversal: ");
    person_inorderTraversal(root);
    printf("\n");
    person_freeTree(root);
}