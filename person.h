#ifndef PERSON_H
#define PERSON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Person structure
typedef struct Person {
    int code;               // Unique code for the person
    char name[50];          // Name of the person
    char phone[15];         // Phone number (optional)
    char address[100];      // Address (optional)
    char birthdate[11];     // Birthdate in format dd/mm/yyyy (required)
    struct Person *prev, *next; // Pointers for doubly linked list
} Person;

// Person list structure
typedef struct PersonList{
    Person *head;
    Person *tail;
    int count;
} PersonList;


// Forward declaration para evitar ciclo
struct Pet;
struct PetList;

// Function prototypes
void initialize_list_person(PersonList *list);
void save_list_person(PersonList *list, const char *filename);
void load_list_person(PersonList *list, const char *filename);
Person *search_person(PersonList *list, int code);
int check_code_person(PersonList list, int code);
Person *insert_top_person(PersonList *list, int code, char *name, char *phone, char *address, char *birthdate);
Person *insert_bottom_person(PersonList *list, int code, char *name, char *phone, char *address, char *birthdate);
struct Pet *search_pet_by_pt(struct PetList *list, int pt_code);
int remove_pets_with_pt(struct PetList *list, int code);
int remove_person(PersonList *list, struct PetList *petList, int code);
Person *update_person(PersonList *list, int code, char *name, char *phone, char *address, char *birthdate);
void print_list_person(PersonList list);
void free_list_person(PersonList *list);

// Binary tree for ordering by name
typedef struct PersonNode {
    Person *person;
    struct PersonNode *left;
    struct PersonNode *right;
} PersonNode;

PersonNode *person_create_node(Person *person);
PersonNode *person_insert(PersonNode *root, Person *person);
void person_inorderTraversal(PersonNode *root);
void person_freeTree(PersonNode *root);
void person_print_order_by_name(PersonList *list);
#endif // PERSON_H