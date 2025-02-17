#include "person.h"
#include "petType.h"
#include "pet.h"
// Initializes an empty list
void initialize_list_pet(PetList *list) {
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

// Save pet list to file
void save_list_pet(PetList *list, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file for writing\n");
        return;
    }

    fwrite(&list->count, sizeof(int), 1, file);

    Pet *current = list->head;
    while (current) {
        fwrite(current, sizeof(Pet) - sizeof(Pet*), 1, file); // Save without pointers
        current = current->next;
    }

    fclose(file);
}

// Read file for pet list
void load_list_pet(PetList *list, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("File does not exist. Starting with an empty list.\n");
        return;
    }

    fread(&list->count, sizeof(int), 1, file);
    list->head = list->tail = NULL;

    for (int i = 0; i < list->count; i++) {
        Pet *newNode = (Pet *)malloc(sizeof(Pet));
        fread(newNode, sizeof(Pet) - sizeof(Pet*), 1, file);
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

// Searches for a pet and returns it
Pet *search_pet(PetList *list, int code) {
    if (!list) {
        printf("WARN: List is empty\n");
        return NULL;
    }

    Pet *cursor = list->head;
    while (cursor->code != code && cursor->next) {
        cursor = cursor->next;
    }
    if (cursor->code != code) {
        printf("WARN: Pet not found\n");
        return NULL;
    }
    printf("INFO: Found pet with code %d\n", code);
    return cursor;
}

// Checks if code already exists in the list
int check_code_pet(PetList list, int code) {
    if (search_pet(&list, code)) {
        return 0;
    }
    return 1;
}

// Inserts a pet at the top of the list
Pet *insert_top_pet(PetList *list, PetTypeList *pt_list, PersonList *person_list, int code, int person_code, char *name, int type_code) {
    Pet *new = malloc(sizeof(Pet));
    if (!new) {
        printf("ERROR: Failed to allocate memory\n");
        return NULL;
    }

    new->code = code;
    new->person_code = person_code;
    strcpy(new->name, name);
    new->type_code = type_code;

    if (!list->head || !list->tail) {
        list->head = new;
        list->tail = new;
        list->head->prev = NULL;
        list->tail->next = NULL;
        printf("CREATION: Created new pet list!\n");
    } else {
        if (!check_code_pet(*list, code)) {
            printf("WARN: A pet with this code already exists!\n");
            return NULL;
        }
        if (check_code_person(*person_list, person_code)) {
            printf("WARN: A person with this code does not exists!\n");
            return NULL;
        }
        if (check_code_pt(*pt_list, type_code)) {
            printf("WARN: A pet type  with this code does not exists!\n");
            return NULL;
        }
        new->prev = NULL;
        new->next = list->head;
        list->head->prev = new;
        list->head = new;
        printf("CREATION: Added new pet on top!\n");
    }
    list->count++;
    return new;
}

// Inserts a pet at the bottom of the list
Pet *insert_bottom_pet(PetList *list, PetTypeList *pt_list, PersonList *person_list, int code, int person_code, char *name, int type_code) {
    Pet *new = malloc(sizeof(Pet));
    if (!new) {
        printf("ERROR: Failed to allocate memory\n");
        return NULL;
    }

    new->code = code;
    new->person_code = person_code;
    strcpy(new->name, name);
    new->type_code = type_code;

    if (!list->head || !list->tail) {
        list->head = new;
        list->tail = new;
        list->head->prev = NULL;
        list->tail->next = NULL;
        printf("CREATION: Created new pet list!\n");
    } else {
        if (!check_code_pet(*list, code)) {
            printf("WARN: A pet with this code already exists!\n");
            return NULL;
        }
        if (check_code_person(*person_list, person_code)) {
            printf("WARN: A person with this code does not exists!\n");
            return NULL;
        }
        if (check_code_pt(*pt_list, type_code)) {
            printf("WARN: A pet type  with this code does not exists!\n");
            return NULL;
        }
        new->next = NULL;
        new->prev = list->tail;
        list->tail->next = new;
        list->tail = new;
        printf("CREATION: Added new pet on bottom!\n");
    }
    list->count++;
    return new;
}

// Deletes a pet from the list
int remove_pet(PetList *list, int code) {
    if (!list || !list->head) {  // Verifica se a lista está vazia
        printf("WARN: Empty list\n");
        return 0;
    }

    Pet *toDelete = NULL;

    // Caso o pet esteja no início da lista
    if (code == list->head->code) {
        toDelete = list->head;
        list->head = toDelete->next;

        if (list->head) {  // Evita acesso inválido caso haja só um nó
            list->head->prev = NULL;
        } else {
            list->tail = NULL;  // Lista ficou vazia
        }

        printf("DELETION: Deleted pet on top\n");

        // Caso o pet esteja no final da lista
    } else if (code == list->tail->code) {
        toDelete = list->tail;
        list->tail = toDelete->prev;

        if (list->tail) {  // Evita acesso inválido caso haja só um nó
            list->tail->next = NULL;
        } else {
            list->head = NULL;  // Lista ficou vazia
        }

        printf("DELETION: Deleted pet on bottom\n");

        // Caso o pet esteja no meio da lista
    } else {
        toDelete = search_pet(list, code);
        if (!toDelete) {
            printf("WARN: Pet not found\n");
            return 0;
        }

        if (toDelete->next) {  // Verifica antes de acessar prev
            toDelete->next->prev = toDelete->prev;
        }
        if (toDelete->prev) {  // Verifica antes de acessar next
            toDelete->prev->next = toDelete->next;
        }

        printf("DELETION: Deleted pet in the middle\n");
    }

    free(toDelete);
    list->count--;
    return 1;
}


// Updates a pet's information
Pet *update_pet(PetList *list, int code, int person_code, char *name, int type_code) {
    Pet *target = search_pet(list, code);
    if (!target) {
        printf("WARN: Pet not found\n");
        return NULL;
    }

    target->person_code = person_code;
    strcpy(target->name, name);
    target->type_code = type_code;
    return target;
}

// Prints the entire list
void print_list_pet(PetList list) {
    printf("<PRINTING PET LIST [%d]>\n\n", list.count);
    if (!list.head) {
        printf("Empty List\n");
        printf("\n<FINISHED PRINTING PET LIST/>\n\n");
        return;
    }

    Pet *aux = list.head;
    int count = 0;
    printf("[Head: (%d) - Count: (%d)]\n", list.head->code, list.count);
    printf("--------------------------------\n");
    while (aux) {
        printf("[%d] %d: '%s' | Owner Code: %d | Type Code: %d\n", count, aux->code, aux->name, aux->person_code, aux->type_code);
        count++;
        aux = aux->next;
    }
    printf("--------------------------------\n\n");
    printf("[Tail: (%d)]\n", list.tail->code);
    printf("\n<FINISHED PRINTING PET LIST/>\n\n");
}

// Free all nodes in the list
void free_list_pet(PetList *list) {
    Pet *current = list->head;
    while (current) {
        Pet *next = current->next;
        free(current);
        current = next;
    }
    list->head = list->tail = NULL;
    list->count = 0;
}

PetNode *pet_create_node(Pet *pet) {
    PetNode* new_pet_node = (PetNode*)malloc(sizeof(PetNode));
    new_pet_node->pet = pet;
    new_pet_node->left = new_pet_node->right = NULL;
    return new_pet_node;
}

PetNode *pet_insert(PetNode *root, Pet *pet) {
    if (!root) {
        return pet_create_node(pet);
    }

    if (strcmp(pet->name, root->pet->name) < 0) {
        root->left = pet_insert(root->left, pet);
    } else if (strcmp(pet->name, root->pet->name) > 0) {
        root->right = pet_insert(root->right, pet);
    }
    return root;
}

void pet_inorderTraversal(PetNode *root) {
    if (!root) return;
    pet_inorderTraversal(root->left);
    printf("%s ", root->pet->name);
    pet_inorderTraversal(root->right);
}

void pet_freeTree(PetNode *root) {
    if (!root) return;
    pet_freeTree(root->left);
    pet_freeTree(root->right);
    free(root);
}

void pet_print_order_by_name(PetList *list) {
    PetNode *root = NULL;
    Pet *current = list->head;
    while (current) {
        root = pet_insert(root, current);
        current = current->next;
    }

    printf("In-order traversal: ");
    pet_inorderTraversal(root);
    printf("\n");
    pet_freeTree(root);
}