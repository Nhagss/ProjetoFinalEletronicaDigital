#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants
#define MAX_KEYS 10
#define MAX_LEN 256

// Structure to store a SQL command
typedef struct {
    char *method;           // Method: insert, update, delete, search
    char *table;            // Table selected
    char **keys;            // Table fields
    char **values;          // Corresponding values
    char *extra;            // Extra command part (ORDER BY, WHERE, etc.)
    char *error;            // Error message
    int key_count;          // Number of keys
} Command;

// Command Node
typedef struct CommandNode {
    Command *cmd;
    struct CommandNode *next;
} CommandNode;

// Command Stack
typedef struct {
    CommandNode *head;
    CommandNode *tail;
} CommandQueue;



struct PetType;
struct PetTypeList;
struct Person;
struct PersonList;
struct Pet;
struct PetList;


// Splits a SQL string into individual commands
char **split_sql(char *sql);

// Initializes a Command structure
void initialize_command(Command *cmd);

// Removes extra spaces from a string
void trim(char *str);

// Extracts values between parentheses
void extract_between_parentheses(const char *str, char *dest);

// Splits a string by commas and stores the result in an array
int split_by_comma(const char *str, char **arr);

// Parses an SQL command and fills it into the Command struct
int parse_command(char *sql, Command *cmd);

// Prints a formatted command
void print_command(const Command *cmd);

// Adds a command to the stack
int stackCommand(char *sql, CommandQueue *command_stack);

// Initializes the command stack
void initialize_command_stack(CommandQueue *command_stack);

// Adds multiple commands to the stack
void stackCommands(char **sql, CommandQueue *command_stack);

// Frees the memory used by the command stack
void free_command_stack(CommandQueue *command_stack);

char *read_file_to_string(const char *filename);

void initialize_command_queue(CommandQueue *command_queue);

void queue_commands(char **sql, CommandQueue *command_queue);

void execute_and_free_command_queue(struct PersonList *person_list,struct  PetTypeList *pt_list,struct  PetList *pet_list, CommandQueue *command_queue);
#endif // COMMANDQUEUE_H