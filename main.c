
#include "commandQueue.h"
#include "petType.h"
#include "person.h"
#include "pet.h"
#include "commandQueue.h"

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

int mainWOCommandList() {
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


    insert_top_pet(&petList, &petTypeList, &personList, 1, 1, "Buddy", 1);
    insert_bottom_pet(&petList, &petTypeList, &personList, 4, 3, "Whiskers", 2);
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
/*
int main1() {

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
    update_person(&personList, 2, "Jane smoth", "932932992", "4343 ksks 223434", "02/03/1985");
    print_list_person(personList);

    insert_top_pt(&petTypeList, 1, "Dog");
    insert_bottom_pt(&petTypeList, 2, "Cat");
    print_list_pt(petTypeList);


    insert_top_pet(&petList, &petTypeList, &personList, 1, 1, "Buddy", 2);
    insert_bottom_pet(&petList, &petTypeList, &personList, 4, 3, "Whiskers", 2);

    remove_person(&personList, &petList, 1);

    print_list_pet(petList);

    print_list_person(personList);
    print_list_pt(petTypeList);
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

int main() {
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


    print_list_person(personList);
    print_list_pt(petTypeList);


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
 */


int main() {


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

    CommandQueue *command_queue = malloc(sizeof(CommandQueue));
    if (!command_queue) {
        printf("Error allocating memory for command queue\n");
        return 1;
    }
    initialize_command_queue(command_queue);

    char *content = read_file_to_string("base.txt");
    if (!content) {  // <<< MUST CHECK HERE
        printf("Error reading file!\n");
        free(command_queue);
        return 1;
    }

    char **test_sqls = split_sql(content);

    if (test_sqls) {
        queue_commands(test_sqls, command_queue);
    }

    execute_and_free_command_queue(&personList, &petTypeList, &petList, command_queue);
// 4. FINALLY clean up IN THIS ORDER:
    free(content);          // Free original content
    if (test_sqls) {
        for (int i = 0; test_sqls[i]; i++) free(test_sqls[i]);
        free(test_sqls);
    }
    save_list_person(&personList, "people.dat");
    save_list_pet(&petList, "pets.dat");
    save_list_pt(&petTypeList, "petTypes.dat");
    // Free all lists
    free_list_person(&personList);
    free_list_pt(&petTypeList);
    free_list_pet(&petList);

    return 0;

}
