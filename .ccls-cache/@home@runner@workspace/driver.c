// hi josh here's what's left: 
/*
1. Driver fn that implements memory address accounting, see line 93/94 "todo @josh" for context
2. Test the file

optional --  gui if bored, hashmap for the names -> binary if bored
*/

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
#include "Queue.c"
#include "commandList.h"
#include "symbolicMap.h"

char* interface();
void file_parsing(char*);
int reg_num(char*);
void commandLinkedList(inputnode*, Queue*);
outputnode* assemble(inputnode*);

int main()
{
    char* filename = interface();
    file_parsing(filename); // add3numbers.s for now, should be Y86 file I'll add it now
    
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
void file_parsing(char *filename) {
    inputnode head;
    inputnode *curr = &head;
    Queue *lineQueue;
    queueCreation(lineQueue);
    
    // File-reading code
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("File open failed. Please try again.\n");
        return;
    }
    char line[4096];
    while (fgets(line, sizeof(line), file))
    {
        printf("I am here!");
        line[strcspn(line, "\n")] = '\0';
        printf("I am here!");
        printf("%s\n", line);
        printf("\n");
        printf("I am here!");
        enqueue(lineQueue, line);
        
        char *lineword = strtok(line, "\t"); // Tokenization of the line into words
        commandLinkedList(curr, lineQueue);
    }
fclose(file);
}

// Josh to-do
void driver(outputnode *list) {
    long memoryAddressCounter = 0;
    while (list != NULL) {
        
        // get the next command from the output list
        // if it's a directive, then add to memory address counter by the following rules:
        // .align: align to current memory address to *command->alignment (x) byte value
        // bool *long_or_quad is true: add 8 bytes to memory address counter
        // .pos -> simply just change memory address to given pos
        // LOOK AT asum.o -- we DONT need to deal with filename directive
        // Symbolic names -- just keep mem address same, but if it's a call, then you know you need to go (need to implement, see line 132)
    }
}

/*
    * Functions to create a linked list of commands.
    * @param: *head, pointer to the head of the desired input linked list (defined out of the function scope)
    * @param: *lineQueue, pointer to the queue of lines that'll be processed into a linked list of assembly commands
*/
void commandLinkedList(inputnode *list, Queue *lineQueue) {
    inputnode* ret = list; //pointer to first element
    // #pragma warning(suppress : [-Wdiscarded-qualifiers]):
    inputnode *curr = ret; //pointer to first element, will be changed
    command *newCommand; //pointer to new command object
    newCommand = malloc(sizeof(command));
    curr->data = newCommand; //modifies data of first element
    curr->next = NULL;
    // ^ @ josh this logic might be off sorry pointers trip me up check it pls
    // ^ changed it a little
    while (!emptyQueue(lineQueue)) {
        char *word = strtok(dequeue(lineQueue), "\t");
        if (word[sizeof(*word)/sizeof(word[0])-1] == ':') { // Symbolic name
            strncpy((newCommand->symbolicName)->name, word, sizeof(*word) - 1);
            newCommand->name = word;
            newCommand->symbol = true;
            // (TODO @ josh) Assumed that, when we do the driver program, address is then assigned to this element. Hence, we can then compare the name of the symbol we encounter in the future to then get a respective address.
            // @Josh implement this pls bc rn we'd need to iterate through maybe a list of symbolic names to get the address of the symbol we encounter, which isn't efficient though if you can easily hashmap that'd be good.
        }
        if (word[0] == '.') {
            newCommand->directive = true;
            if (!strcmp(word, ".long") || !strcmp(word, ".quad")) {
                newCommand->long_or_quad = true;
            }
            else if (!strcmp(word, ".pos")) {
                newCommand->name = word;
                newCommand->pos = true;
                newCommand->position = word[5]; // has 'x'
                // We'd simply shift memory address in driver program to x/position BTW.
            }
            else if (!strcmp(word, ".align")) {
                newCommand->name = word;
                newCommand->align = true;
                newCommand->alignment = word[5]; // has 'x'
                // We'd simply do modulo to align to the given alignment for the memory address.
            }
            else if (/* Implement call when you take care of fns*/false) {
                // todo
            }
            else {
                newCommand->directive = false; // Note this is unnecessary, just for clarity.
                // Here, we'd use "strtok(NULL, "\t"); to get each next word, i.e. do it each time you need a next word

                /*
               Clarified: the next would be like "rA," WITH THE COMMA BTW, and we'd extra rA usng strcpy, then rB/similar, we'd probably need to extract differently depending on the iniital word/instruction since it'd have different parameters to extract. But that's it. 
               
               Then, we have the list, and we have our outputnode list, and we can simply implement driver fn I put skeleton code for above to create the memory address counting (and we have everything we need for that now -- directives have the necessary shifts, and for commands just look at the size of the binary command to know how much to increase the mem address by)

               You'd simply need a way to flip around the output node list, or simply save the head somewhere (I might not have fully understood that BTW) and then you can traverse it one by one and print. Done

                */
            }
        }
        // move to next element
        inputnode *next;
        next = malloc(sizeof(inputnode));
        curr->next = next;
        curr = curr->next;
        newCommand = malloc(sizeof(command));
        curr->data = newCommand;
        curr->next = NULL;
    }   
}
// assuming there is some linked-list structure of commands (to-implement)

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
    else return 15;
}



    // need to double check this
outputnode* assemble(inputnode *list)
{
    outputnode* ret;
    ret = malloc(sizeof(outputnode));
    outputnode* curr = ret;
    curr->data= NULL;
    curr->next=NULL;
    while (list != NULL)
    {
        // add error handling?
            // no -surya jk we'll do it later
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
            sprintf(buff, "70%x", reg_num(comm.other));
        else if (!strcmp(comm.name, "jle"))
            sprintf(buff, "71%x", reg_num(comm.other));
        else if (!strcmp(comm.name, "jl"))
            sprintf(buff, "72%x", reg_num(comm.other));
        else if (!strcmp(comm.name, "je"))
            sprintf(buff, "73%x", reg_num(comm.other));
        else if (!strcmp(comm.name, "jne"))
            sprintf(buff, "74%x", reg_num(comm.other));
        else if (!strcmp(comm.name, "jge"))
            sprintf(buff, "75%x", reg_num(comm.other));
        else if (!strcmp(comm.name, "jg"))
            sprintf(buff, "76%x", reg_num(comm.other));
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
        else if (!strcmp(comm.name, "call"))
            sprintf(buff, "80%s", comm.other);
        else if (!strcmp(comm.name, "ret"))
            sprintf(buff, "90");
        else if (!strcmp(comm.name, "pushl"))
            sprintf(buff, "A0%xf", comm.rA);
        else if (!strcmp(comm.name, "popl"))
            sprintf(buff, "B0%xf", comm.rA);
        else
            exit(1);
        curr->data = buff;
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
