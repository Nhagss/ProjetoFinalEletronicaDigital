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
} PetTypeList;

// Initializes an empty list
void inicializarTiposPet(PetTypeList *list) { list->head = NULL; list->tail = NULL; }

// Insert Functions
void insert_top_pt(PetTypeList *list, int code, char *name) {
    PetType *new = malloc(sizeof(PetType));
    if(!new) {
        printf("Error Allocating memory\n");
        return;
    }

    new->code = code;
    strcpy(new->name, name);

    if (!list->head) {
        list->head = new;
        list->tail = new;
        list->head->prev = NULL;
        list->tail->next = NULL;
    } else {
        new->prev = NULL;
        new->next = list->head;
        list->head->prev = new;
        list->head = new;
    }
}

// Outras funções CRUD para TipoPet e Pet seguem o mesmo padrão...
void checkPetList(PetTypeList list) {

    if(!list.head) {
        printf("Lista Vazia");
    }
    PetType *aux = list.head;
    int count = 0;
    printf("(Head: %d [%d])\n", list.head->code, count);
    printf("--------------------------------\n");
    while(aux) {
        printf("[%d] %d:  '%s'\n", count, aux->code, aux->name);
        count++;
        aux = aux->next;
    }

    printf("--------------------------------\n");
    printf("(Tail: %d [%d])\n", list.tail->code, count);
}
int main() {
    PetTypeList petTypeList;
    checkPetList(petTypeList);
    inicializarTiposPet(&petTypeList);
    checkPetList(petTypeList);
    insert_top_pt(&petTypeList, 10, "Rodolfo");
    insert_top_pt(&petTypeList, 11, "Rodolf2");
    checkPetList(petTypeList);

    return 0;
}