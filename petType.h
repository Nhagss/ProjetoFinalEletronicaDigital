#ifndef PETTYPE_H
#define PETTYPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declaration para evitar ciclo
struct Pet;
struct PetList;

// Pet Type structure
typedef struct PetType {
    int code;
    char name[50];
    struct PetType *prev, *next;
} PetType;

typedef struct PetTypeList{
    PetType *head;
    PetType *tail;
    int count;
} PetTypeList;

// List initialization
void initialize_list_pt(PetTypeList *list);

// File functions
void save_list_pt(PetTypeList *list, const char *filename);
void load_list_pt(PetTypeList *list, const char *filename);

// CRUD functions
PetType *search_pt(PetTypeList *list, int code);
int check_code_pt(PetTypeList list, int code);
PetType *insert_top_pt(PetTypeList *list, int code, char *name);
PetType *insert_bottom_pt(PetTypeList *list, int code, char *name);
struct Pet *search_pet_by_pt(struct PetList *list, int pt_code);
int remove_pets_with_pt(struct PetList *list, int code);
int remove_pt(PetTypeList *list, struct PetList *petList, int code);
PetType *update_pt(PetTypeList *list, int code, char *name);

// Supplementary functions
void print_list_pt(PetTypeList list);
void free_list_pt(PetTypeList *list);

// Binary tree for ordering by name
typedef struct PTNode {
    PetType *pet;
    struct PTNode *left;
    struct PTNode *right;
} PTNode;

PTNode *pt_create_node(PetType *pet);
PTNode *pt_insert(PTNode *root, PetType *pet);
void pt_inorderTraversal(PTNode *root);
void pt_freeTree(PTNode *root);
void pt_print_order_by_name(PetTypeList *list);

#endif // PETTYPE_H
