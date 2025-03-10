// To-do: Debugging to get asum.s to run and produce asum.o. There's a print queue fn to use, alongside a print map fn to help.

/*
 * @authors Surya Duraivenkatesh, Josh Tittiranonda
 * @date: 03/2025
 * This is the OHS Y86-64 Emulator! In this program, assembly files are read and parsed into a queue of commands, which is ultimately put into a linked list of commands split by their struct elements. Finally, this queue is processed by the Y86-64 Emulator, which produces a linked list of machine code commands, which are ultimately printed to the user. Hope you enjoy!

[References]
Bryant, Randal E., and David R. O'Hallaron. Computer Systems: A Programmer's Perspective.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "Queue.h"
#include "Queue.c"
#include "commandList.h"
#include "symbolicMap.h"
#include "symbolicMap.c"

char* interface();
map* file_parsing(char*, inputnode*);
int reg_num(char*);
map* commandLinkedList(inputnode*, Queue*, map*);
outputnode* assemble(inputnode*, map*);

int main()
{   
    inputnode *startingList = malloc(sizeof(inputnode)); // To print in deassembler-style later
    outputnode *print;
    char* filename = interface();
    print = assemble(startingList, file_parsing(filename, startingList));

    while (print != NULL) {
        printf("0#%x\t", print->memoryAddress);
        printf("%s\n", print->data);
        print = print->next;
    }
}    

/*
 * Function for the user to input file name of the file they want to assemble.
 * @return: char* filename: file name, <= 50 characters
*/
char* interface()
{
    char *str = malloc(50);
    printf("%s\n", "Welcome to the OHS Y86-64 Emulator! This program will help convert your assembly instructions into machine code. Please enter the file you'd like to assemble: ");
    scanf("%49s", str);  // I'll assume the file name won't exceed 50 characters.
    return str;
}

/*
 * Function to parse the file into an array of lines.
 * @param: char* filename: file name, <= 50 characters
*/
map* file_parsing(char *filename, inputnode *head) {
    map *map = malloc(sizeof(map));
    inputnode *curr = head;
    Queue *lineQueue = malloc(sizeof(Queue));
    queueCreation(lineQueue);
    
    // File-reading code
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("File open failed. Please contact the developer to fix a logical error within the program.");
        // Would be for debugging only.
    }
    char line[4096];
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0';
        printf("%s\n", line);
        enqueue(lineQueue, line);
    }
    fclose(file);

    printQueue(lineQueue);
    printf("arrived before calling command list fn\n");
    return commandLinkedList(curr, lineQueue, map);
}

/*
    * Functions to create a linked list of commands.
    * @param: *head, pointer to the head of the desired input linked list (defined out of the function scope)
    * @param: *lineQueue, pointer to the queue of lines that'll be processed into a linked list of assembly commands
*/
map* commandLinkedList(inputnode *list, Queue *lineQueue, map* map) {
    inputnode* ret = list; // Pointer to first element.
    inputnode *curr = ret; // Pointer to first element, will be changed.
    command *newCommand; // Pointer to new command object.
    newCommand = malloc(sizeof(command));
    curr->data = newCommand; // Modifies data of first element
    curr->next = NULL;

    printf("arrived before calling empty queue while lp\n"); // Debugging purposes

    while (!emptyQueue(lineQueue)) {
        char *word = strtok(dequeue(lineQueue), "\t"); // Tokenize line into words
        printf("arrived before calling if statement for dir\n"); // Debugging purposes
        printf("%s\n", word);
        if (word[0] == '.') { // Assembler directive case
            newCommand->directive = true;
            if (!strcmp(word, ".long") || !strcmp(word, ".quad")) {
                newCommand->long_or_quad = true;
            }
            else if (!strcmp(word, ".pos")) {
                newCommand->name = word;
                newCommand->pos = true;
                newCommand->position = word[5]; // Shifts memory address in driver program to the given position.
            }
            else if (!strcmp(word, ".align")) {
                newCommand->name = word;
                newCommand->align = true;
                newCommand->alignment = word[5]; // Aligns to a alignment-byte boundary in driver program.
            }
            else {
                continue; // The directive is then not important (e.g. .file or .globl). Note, there exists cases like ".LFB22:", so we can't allow it to hit the next elif.
            }
        }
        else if (word[sizeof(*word)/sizeof(word[0])-1] == ':') { // Symbolic name case
            strncpy((newCommand->symbolicName)->name, word, sizeof(*word) - 1);
            newCommand->name = word;
            newCommand->symbol = true;
            add(map, newCommand->symbolicName); // Adding to hashmap
        }
        else { // Non-directive command case
            newCommand->directive = false; // Note this is unnecessary, just for clarity.
            // Here, we'd use "strtok(NULL, "\t"); to get each next word, i.e. do it each time you need a next word
            if (!strcmp(word, "halt") || !strcmp(word, "nop")) {
                newCommand->name = word; // All that's needed is to set the name of the command.
            }
            else if (word[0] == 'j') {
                newCommand->name = word;
                newCommand->other = strtok(NULL, "\t"); // Gets the next word
            }
            else if (!strcmp(word, "cmov") || !strcmp(word, "cmovle") || !strcmp(word, "cmovl") || !strcmp(word, "cmove") || !strcmp(word, "cmovne") || !strcmp(word, "cmovge") || !strcmp(word, "cmovg")) {
                newCommand->name = word;
                newCommand->rA = strtok(NULL, "\t");
                strncpy(newCommand->rA, word, sizeof(*word) - 1);
                newCommand->rB = strtok(NULL, "\t");
            }
            else if (!strcmp(word, "addl") || !strcmp(word, "subl") || !strcmp(word, "andl") || !strcmp(word, "xorl")) { // Same as above, but written out for clarity.
                newCommand->name = word;
                newCommand->rA = strtok(NULL, "\t");
                strncpy(newCommand->rA, word, sizeof(*word) - 1);
                newCommand->rB = strtok(NULL, "\t");
            }
            else if (!strcmp(word, "pushl") || !strcmp(word, "popl")) {
                newCommand->name = word;
                newCommand->rA = strtok(NULL, "\t");
            }
            else if (!strcmp(word, "call")) {
                newCommand->name = word;
                newCommand->other = strtok(NULL, "\t");
            }
            else if (!strcmp(word, "ret")) {
                newCommand->name = word;
            }
            else if (!strcmp(word, "irmovl") || !strcmp(word, "rmmovl")) {
                newCommand->name = word;
                newCommand->rA = strtok(NULL, "\t");
                strncpy(newCommand->rA, word, sizeof(*word) - 1);
                newCommand->rB = strtok(NULL, "\t");
            }
            else if (!strcmp(word, "rrmovl")) {
                newCommand->name = word;
                newCommand->rA = strtok(NULL, "\t");
                strncpy(newCommand->rA, word, sizeof(*word) - 1);
                newCommand->rB = strtok(NULL, "\t");
            }
            else if (!strcmp(word, "irmovl")) {
                newCommand->name = word;
                newCommand->other = strtok(NULL, "\t");
                strncpy(newCommand->other, word, sizeof(*word) - 1);
                newCommand->rB = strtok(NULL, "\t");
            }
            else if (!strcmp(word, "rmmovl")) {
                newCommand->name = word;
                newCommand->rA = strtok(NULL, "\t");
                strncpy(newCommand->rA, word, sizeof(*word) - 1);
                newCommand->other = strtok(NULL, "\t()"); // Three separate delimeters (https://cplusplus.com/reference/cstring/strtok/)
                newCommand->rB = strtok(NULL, "\t");
            }
            else if (!strcmp(word, "mrmovl")) {
                newCommand->name = word;
                newCommand->other = strtok(NULL, "\t(),"); // Four separate delimeters
                newCommand->rB = strtok(NULL, "\t(),");
                newCommand->rA = strtok(NULL, "\t(),");
            } 
        }
        inputnode *next;
        next = malloc(sizeof(inputnode));
        curr->next = next;
        curr = curr->next;
        curr->data = newCommand;
        curr->next = NULL;
    }
    return map;
}

/*
    Converts register to register's machine-code representation.
    @param *reg register to convert
    @return: char* register's binary representation
*/
int reg_num(char *reg)
{
    if (strcmp(reg, "%eax") == 0) return 0;
    else if (strcmp(reg, "%ecx") == 0) return 1;
    else if (strcmp(reg, "%edx") == 0) return 2;
    else if (strcmp(reg, "%ebx") == 0) return 3;
    else if (strcmp(reg, "%esp") == 0) return 4;
    else if (strcmp(reg, "%ebp") == 0) return 5;
    else if (strcmp(reg, "%esi") == 0) return 6;
    else if (strcmp(reg, "%edi") == 0) return 7;
    else if (strcmp(reg, "%e8") == 0) return 8;
    else if (strcmp(reg, "%e9") == 0) return 9;
    else if (strcmp(reg, "%e10") == 0) return 10;
    else if (strcmp(reg, "%e11") == 0) return 11;
    else if (strcmp(reg, "%e12") == 0) return 12;
    else if (strcmp(reg, "%e13") == 0) return 13;
    else if (strcmp(reg, "%e14") == 0) return 14;
    else if (reg[0] == '$') { // Constants/Immediate values
        return (int)strtol(reg + 1, NULL, 10);
    }
    else return 15; // No register case
}

/*
    Converts the linked list of assembly commands into a linked list of binary commands.
    @param: *list, input linked list
*/
outputnode* assemble(inputnode *list, map *names)
{
    long memoryAddress; // Added as field for outputnode
    outputnode* ret;
    ret = malloc(sizeof(outputnode));
    outputnode* curr = ret;
    curr->data= NULL;
    curr->next=NULL;
    while (list != NULL)
    {
        if ((list->data)->directive == true) {
            if ((list->data)->long_or_quad == true) {
                memoryAddress += 8;
                // print long/quad value, need to save it when making input list
            }
            else if ((list->data)->pos == true) {
                memoryAddress = (list->data)->position;
                // would move on afterward
            }
            else if ((list->data)->align == true) {
                memoryAddress = memoryAddress + (memoryAddress % (list->data)->alignment);
                // would move on afterward
            }
            else if ((list->data)->symbol == true) {
                (list->data)->symbolicName->address = memoryAddress;
            }
        } 
        // If not directive, then it's a command, and we can just calculate its 
        curr->memoryAddress = memoryAddress + sizeof(curr->data); // Case for if not directive, and it'd assumably set to a value after all directives complete at beginning of prorgram (for our case, .pos 0 -> mem starts @ 0)
        char *buff;
        command comm = *(list->data);
        if (!strcmp(comm.name, "halt"))
            buff = "00";
        else if (!strcmp(comm.name, "nop"))
            buff = "10";
        else if (!strcmp(comm.name, "rrmovl"))
            sprintf(buff, "20%x%x", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "irmovl"))
            sprintf(buff, "30f%x%x", reg_num(comm.rB), reg_num(comm.other));
        else if (!strcmp(comm.name, "rmmovl"))
            sprintf(buff, "40%x%x%s", reg_num(comm.rA), reg_num(comm.rB), comm.other);
        else if (!strcmp(comm.name, "mrmovl"))
            sprintf(buff, "50%x%x%s", reg_num(comm.rA), reg_num(comm.rB), comm.other);
        else if (!strcmp(comm.name, "addl"))
            sprintf(buff, "60%x%x", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "subl"))
            sprintf(buff, "61%x%x", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "andl"))
            sprintf(buff, "62%x%x", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "xorl"))
            sprintf(buff, "63%x%x", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "jmp"))
            sprintf(buff, "70%x", findAddress(names, comm.other));
        else if (!strcmp(comm.name, "jle"))
            sprintf(buff, "71%x", findAddress(names, comm.other));
        else if (!strcmp(comm.name, "jl"))
            sprintf(buff, "72%x", findAddress(names, comm.other));
        else if (!strcmp(comm.name, "je"))
            sprintf(buff, "73%x", findAddress(names, comm.other));
        else if (!strcmp(comm.name, "jne"))
            sprintf(buff, "74%x", findAddress(names, comm.other));
        else if (!strcmp(comm.name, "jge"))
            sprintf(buff, "75%x", findAddress(names, comm.other));
        else if (!strcmp(comm.name, "jg"))
            sprintf(buff, "76%x", findAddress(names, comm.other));
        else if (!strcmp(comm.name, "cmovle"))
            sprintf(buff, "21%x%x", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "cmovl"))
            sprintf(buff, "22%x%x", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "cmove"))
            sprintf(buff, "23%x%x", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "cmovne"))
            sprintf(buff, "24%x%x", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "cmovge"))
            sprintf(buff, "25%x%x", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "cmovg"))
            sprintf(buff, "26%x%x", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "call")) // Hashtable for symbolic names helps here
            sprintf(buff, "80%x", findAddress(names, comm.other));
            
        else if (!strcmp(comm.name, "ret"))
            sprintf(buff, "90");
        else if (!strcmp(comm.name, "pushl"))
            sprintf(buff, "A0%xf", reg_num(comm.rA));
        else if (!strcmp(comm.name, "popl"))
            sprintf(buff, "B0%xf", reg_num(comm.rA));
        curr->data = buff;
        memoryAddress = memoryAddress + sizeof(buff); // For case of not directive, this is how the memory address updates
        
        outputnode *next;
        next = malloc(sizeof(outputnode));
        curr->next = next;
        curr = curr->next;
        curr->data = NULL;
        curr->next=NULL;
        list = list->next;
    }
    return ret;
}