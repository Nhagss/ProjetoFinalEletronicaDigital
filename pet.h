#ifndef PET_H
#define PET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "person.h"
#include "petType.h"
// Pet structure
typedef struct Pet {
    int code;               // Unique code for the pet
    int person_code;        // Code of the person who owns the pet
    char name[50];          // Name of the pet
    int type_code;          // Code of the pet type
    struct Pet *prev, *next; // Pointers for doubly linked list
} Pet;

// Pet list structure
typedef struct {
    Pet *head;
    Pet *tail;
    int count;
} PetList;

// Function prototypes
void initialize_list_pet(PetList *list);
void save_list_pet(PetList *list, const char *filename);
void load_list_pet(PetList *list, const char *filename);
Pet *search_pet(PetList *list, int code);
int check_code_pet(PetList list, int code);
Pet *insert_top_pet(PetList *list, PetTypeList *pt_list, PersonList *person_list, int code, int person_code, char *name, int type_code);
Pet *insert_bottom_pet(PetList *list, PetTypeList *pt_list, PersonList *person_list, int code, int person_code, char *name, int type_code);
int remove_pet(PetList *list, int code);
Pet *update_pet(PetList *list, int code, int new_code, int person_code, char *name, int type_code);
void print_list_pet(PetList list);
void free_list_pet(PetList *list);

#endif // PET_H