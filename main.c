#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Pet Type structur
typedef struct PetType {
    int code;
    char name[50];
    struct PetType *prev, *next;
} PetType;

typedef struct {
    PetType *head;
    PetType *tail;
    int count;
} PetTypeList;

// Initializes an empty list
void initialize_list_pt(PetTypeList *list) { list->head = NULL; list->tail = NULL; list->count = 0; }

//FILE FUNCTIONS
//Save pet list to file
void save_list_pt(PetTypeList *list, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file for writing\n");
        return;
    }

    fwrite(&list->count, sizeof(int), 1, file);

    PetType *current = list->head;
    while (current) {
        fwrite(current, sizeof(PetType) - sizeof(PetType*), 1, file); // Save without pointers
        current = current->next;
    }

    fclose(file);
}

//Read file for pet list
void load_list_pt(PetTypeList *list, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("File does not exist. Starting with an empty list.\n");

        return;
    }

    fread(&list->count, sizeof(int), 1, file);
    list->head = list->tail = NULL;

    for (int i = 0; i < list->count; i++) {
        PetType *newNode = (PetType *)malloc(sizeof(PetType));
        fread(newNode, sizeof(PetType) - sizeof(PetType*), 1, file);
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


// CRUD FUNCTIONS
// Searches for a pet and returns it
PetType *search_pt(PetTypeList *list, int code) {
    if(!list) {
        printf("WARN: List is empty \n");
        return NULL;
    }

    PetType *cursor = list->head;
    while(cursor->code != code && cursor->next) {
        cursor = cursor->next;
    }
    if(cursor->code != code) {
        printf("WARN: Type of pet not found\n");
        return NULL;
    }
    printf("INFO: Found pet type with code %d\n", code);
    return cursor;
}

//Checks if code already exists on a list
int check_code_pt(PetTypeList list, int code) {
    PetType *arrow = list.head;
    if(search_pt(&list, code)) {
        return 0;
    }
    return 1;
}

// INSERT FUNCTIONS
// Inserts on top (returns new pet type in case it is needed)
PetType *insert_top_pt(PetTypeList *list, int code, char *name) {
    PetType *new = malloc(sizeof(PetType));
    if(!new) {
        printf("ERROR: Failed to allocate memory\n");
        return NULL;
    }

    new->code = code;
    strcpy(new->name, name);

    if (!list->head || !list->tail) {
        list->head = new;
        list->tail = new;
        list->head->prev = NULL;
        list->tail->next = NULL;
        printf("CREATION: Created new pet type list!\n");
    } else {
        if(!check_code_pt(*list, code)) {
            printf("WARN: A pet type with this code already exists!\n");
            return NULL;
        }
        new->prev = NULL;
        new->next = list->head;
        list->head->prev = new;
        list->head = new;
        printf("CREATION: Added new pet type on top!\n");
    }
    list->count++;
    return new;
}

// Inserts on bottom (returns new pet type in case it is needed)
PetType *insert_bottom_pt(PetTypeList *list, int code, char *name) {
    PetType *new = malloc(sizeof(PetType));
    if(!new) {
        printf("ERROR: Failed to allocate memory\n");
        return NULL;
    }

    new->code = code;
    strcpy(new->name, name);

    if (!list->head || !list->tail) {
        list->head = new;
        list->tail = new;
        list->head->prev = NULL;
        list->tail->next = NULL;
        printf("CREATION: Created new pet type list!\n");
    } else {
        if(!check_code_pt(*list, code)) {
            printf("WARN: A pet type with this code already exists!\n");
            return NULL;
        }
        new->next = NULL;
        new->prev = list->tail;
        list->tail->next = new;
        list->tail = new;
        printf("CREATION: Added new pet type on bottom!\n");
    }
    list->count++;
    return new;
}

// Delete Function (returns 0 if it fails and 1 if it succesess)
int remove_pt(PetTypeList *list, int code) {
    if (!list) {
        printf("WARN: Empty list\n");
        return 0;
    }

    PetType *toDelete = NULL;
    if (code == list->head->code) {
        toDelete = list->head;
        toDelete->next->prev = NULL;

        list->head = toDelete->next;

        printf("DELETION: Deleted pet type on top");
    }else if (code == list->tail->code && list->tail->code != list->head->code) {
        toDelete = list->tail;
        toDelete->prev->next = NULL;

        list->tail = toDelete->prev;
        printf("DELETION: Deleted pet type on bottom");
    } else {
        toDelete = search_pt(list, code);
        if(!toDelete) {
            printf("WARN: Pet type not found");
            return 0;
        }
        toDelete->next->prev = toDelete->prev;
        toDelete->prev->next = toDelete->next;

        printf("DELETION: Deleted pet type on middle");
    }
    free(toDelete);
    list->count--;
    return 1;
}

PetType *update_pt(PetTypeList *list, int code, int new_code, char *name) {
    PetType *target = search_pt(list, code);
    if(!target) {
        printf("WARN: Pet Type not found\n");
        return NULL;
    }

    if(!check_code_pt(*list, new_code)) {
        printf("WARN: Code already exists\n");
        return NULL;
    }
    target->code = new_code;
    strcpy(target->name, name);
    return target;
}

// SUPPLEMENTARY FUNCTIONS
// prints the entire list, its head, tail and count
void print_list_pt(PetTypeList list) {
    printf("<PRINTING PET TYPE LIST [%d]> \n\n", list.count);
    if(!list.head) {
        printf("Lista Vazia\n");
    printf("\n<FINISHED PRINTING PET TYPE LIST/>\n\n");
        return;
    }

    PetType *aux = list.head;
    int count = 0;
    printf("[Head: (%d) - Count: (%d)\n", list.head->code, list.count);
    printf("--------------------------------\n");
    while(aux) {
        printf("[%d] %d:  '%s'\n", count, aux->code, aux->name);
        count++;
        aux = aux->next;
    }
    count--;
    printf("--------------------------------\n\n");
    printf("[Tail: (%d)]\n", list.tail->code);

    printf("\n<FINISHED PRINTING PET TYPE LIST/>\n\n");
}

// Free all nodes in the list
void free_list_pt(PetTypeList *list) {
    PetType *current = list->head;
    while (current) {
        PetType *next = current->next;
        free(current);
        current = next;
    }
    list->head = list->tail = NULL;
    list->count = 0;
}

//Using binary tree to order pet type by name
// PERGUNTAR AO AJALMAR SE PODE HAVER USUARIOS COM NOME IGUAL

typedef struct PTNode {
    PetType *pet;
    struct PTNode *left;
    struct PTNode *right;
} PTNode;

PTNode *create_node_pt(PetType *pet) {
    PTNode* new_pt_node = (PTNode*)malloc(sizeof(PTNode));
    new_pt_node->pet = pet;
    new_pt_node->left = new_pt_node->right = NULL;
    return new_pt_node;
}

PTNode *insert(PTNode *root, PetType *pet) {
    if (!root) {
        return create_node_pt(pet);
    }

    if (strcmp(pet->name, root->pet->name) < 0) {
        root->left = insert(root->left, pet);
    } else if (strcmp(pet->name, root->pet->name) > 0) {
        root->right = insert(root->right, pet);
    }
    return root;
}

void inorderTraversal(PTNode *root) {
    if (!root) return;
    inorderTraversal(root->left);
    printf("%s ", root->pet->name);
    inorderTraversal(root->right);
}

void freeTree(PTNode *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void print_pt_order_by_name(PetTypeList *list) {
    PTNode *root = NULL;
    PetType *current = list->head;
    while (current) {
        root = insert(root, current);
        current = current->next;
    }

    printf("In-order traversal: ");
    inorderTraversal(root);
    printf("\n");
    freeTree(root);
}


int main() {
    PetTypeList petTypeList;
    initialize_list_pt(&petTypeList);
    load_list_pt(&petTypeList, "petTypes.dat");

    insert_top_pt(&petTypeList, 1, "g");
    insert_top_pt(&petTypeList, 2, "h");
    insert_top_pt(&petTypeList, 3, "c");
    insert_top_pt(&petTypeList, 4, "v");
    insert_top_pt(&petTypeList, 5, "b");
    insert_top_pt(&petTypeList, 6, "n");
    insert_top_pt(&petTypeList, 7, "m");
    print_list_pt(petTypeList);
    print_pt_order_by_name(&petTypeList);
/*
    PetType *teste = petTypeList.head;
    PTNode *root = NULL;
    while (teste) {
        insert(root, *teste);
    }

    printf("In order:");
    inorderTraversal(root);
    printf("\n");
    freeTree(root);
*/
    //update_pt(&petTypeList, 11, 14, "Rodolfake");
    //remove_pt(&petTypeList, 11);


    //print_list_pt(petTypeList);
    save_list_pt(&petTypeList, "petTypes.dat");

    free_list_pt(&petTypeList);
    return 0;
}