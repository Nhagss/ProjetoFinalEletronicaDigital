#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Estrutura para Tipo de Pet
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
void inicializarTiposPet(PetTypeList *list) { list->head = NULL; list->tail = NULL; list->count = 0; }

// Insert Functions
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
        new->prev = NULL;
        new->next = list->head;
        list->head->prev = new;
        list->head = new;
        printf("CREATION: Added new pet type on top!\n");
    }
    list->count++;
    return new;
}

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
        new->next = NULL;
        new->prev = list->tail;
        list->tail->next = new;
        list->tail = new;
        printf("CREATION: Added new pet type on bottom!\n");
    }
    list->count++;
    return new;
}

// Search Function
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
    printf("INFO: Found pet type with code %d", code);
    return cursor;


}
// Delete Function
int removePetList(PetTypeList *list, int code) {
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

// Outras funções CRUD para TipoPet e Pet seguem o mesmo padrão...
void checkPetList(PetTypeList list) {
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

int main() {
    PetTypeList petTypeList;
    inicializarTiposPet(&petTypeList);
    insert_top_pt(&petTypeList, 11, "Rodolf2");
    insert_top_pt(&petTypeList, 10, "Rodolfo1");
    insert_bottom_pt(&petTypeList, 12, "Rodolf3");
    insert_bottom_pt(&petTypeList, 13, "Rodolf4");
    removePetList(&petTypeList, 13);
    checkPetList(petTypeList);
    return 0;
}