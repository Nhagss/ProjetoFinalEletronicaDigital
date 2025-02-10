
#include "commandQueue.h"
#include "petType.h"
#include "person.h"
#include "pet.h"

/*
 typedef struct {
    char method[10];           // Method: insert, update, delete, search
    char keys[MAX_KEYS][MAX_LEN];  // Table fields
    char values[MAX_KEYS][MAX_LEN]; // Corresponding values
    char extra[MAX_LEN];        // Extra command part (ORDER BY, WHERE, etc.)
    int key_count;              // Number of keys
} Command;
 */
void execute_command(Command command) {

}

int mainMain() {
    // Example usage of the implemented functions
    PersonList personList;
    PetTypeList petTypeList;
    PetList petList;
    initialize_list_person(&personList);
    initialize_list_pt(&petTypeList);
    initialize_list_pet(&petList);

    load_list_person(&personList, "people.dat");
    load_list_pet(&petList, "pets.dat");
    load_list_pt(&petTypeList, "petTypes.dat");



    insert_top_person(&personList, 1, "John Doe", "123456789", "123 Main St", "01/01/1990");
    insert_bottom_person(&personList, 2, "Jane Smith", "987654321", "456 Elm St", "02/02/1985");
    print_list_person(personList);

    insert_top_pt(&petTypeList, 1, "Dog");
    insert_bottom_pt(&petTypeList, 2, "Cat");
    print_list_pt(petTypeList);


    insert_top_pet(&petList, 1, 1, "Buddy", 1);
    insert_bottom_pet(&petList, 2, 2, "Whiskers", 2);
    print_list_pet(petList);


    save_list_person(&personList, "people.dat");
    save_list_pet(&petList, "pets.dat");
    save_list_pt(&petTypeList, "petTypes.dat");
    // Free all lists
    free_list_person(&personList);
    free_list_pt(&petTypeList);
    free_list_pet(&petList);

    return 0;
}