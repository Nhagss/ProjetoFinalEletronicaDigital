#ifndef PETTYPE_H
#define PETTYPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Pet Type structure
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
int remove_pt(PetTypeList *list, int code);
PetType *update_pt(PetTypeList *list, int code, int new_code, char *name);

// Supplementary functions
void print_list_pt(PetTypeList list);
void free_list_pt(PetTypeList *list);

// Binary tree for ordering by name
typedef struct PTNode {
    PetType *pet;
    struct PTNode *left;
    struct PTNode *right;
} PTNode;

PTNode *create_node_pt(PetType *pet);
PTNode *insert(PTNode *root, PetType *pet);
void inorderTraversal(PTNode *root);
void freeTree(PTNode *root);
void print_pt_order_by_name(PetTypeList *list);

#endif // PETTYPE_H
