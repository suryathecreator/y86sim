// All that's left/To-do 03/09: 
/*
1. quick assembler directives/symbolic names (see Line 87), 
2. creation of a quick overall driver fn. to manage the overall program/memory counter (see Line 87 for logic, super quick), 
3. conversion of practice prob file of y86/typing it up, testing/debugging -- probably should do this first
so that the overall driver fn./assembly directives are done properly w.r.t. the way my input converts into a command linked list (see Line 87 for the explanation of the symbolic name -> memory address idea)

*See file_parsing_example.pdf for how it's parsed -- the file_parsing gets it into those lines (ex. movl\t$0\t%eax (i'm putting \t so it's clear that it's a tab split)), then the command linked list function does a NULL, \t splicing to get each word (that's each word being printed). See the fns and this will make sense

I can do 1/2/3^ in a bit but I'm putting it here in case you have time

optional -- hashmap & gui if bored
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

char* interface();
void file_parsing(char*);
int reg_num(char*);
void commandLinkedList(inputnode*, Queue*);
outputnode assemble(inputnode*);

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
    
    // File-reading code
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("File open failed. Please try again.\n");
        //return;
    }
    char line[4096];
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0';
        printf("%s\n", line);
        printf("\n");
        enqueue(lineQueue, line);
        
        char *lineword = strtok(line, "\t"); // Tokenization of the line into words
        commandLinkedList(curr, lineQueue);
        // Linked-list creation
   /*     while (word != NULL)
        {
            printf("Word:%s\n", word);
            word = strtok(NULL, "\t"); // Traversing through the line, word-by-word
//            printf("%s", word);
            printf("\n");
        }
    }*/
    }
fclose(file);
}

/*
    * Functions to create a linked list of commands.
    * @param: *head, pointer to the head of the desired input linked list (defined out of the function scope)
    * @param: *lineQueue, pointer to the queue of lines that'll be processed into a linked list of assembly commands
*/
    // TO-DO (surya): Finishing up with assembler directives and similar
void commandLinkedList(inputnode *head, Queue *lineQueue) {
    inputnode *curr = head;
    while (!emptyQueue(lineQueue)) {
        char *word = strtok(dequeue(lineQueue), "\t");
// regex code to detect whether its a a) symbolic name (regex with ":"), compiler directive (regex with "."), and if neither its gonna be an actual command.
        // Would simply just -- for labels -- create hashmap for the labels' locations.
            // for the compiler directives, usually modifies the line counter to align with a boundary.
                // 3 of them, .pos x starts next lines @ x, .align x means align to x-byte boundary, .long/.quad means we put a 4/8-byte value at the current memory address where the inst. is
        // so we need to also keep count of how many bytes the inst. is (tho we can just get the size) for the counter
// Note, if there is a init: for example, we need to like push that to hashmap and keep moving -- but if it's a directive, we'd do a different route (we'd follow out the directive's effects). 
    }   
}

// You will assume to receive in remainder: a hex rep of "%rA, %rB"

/*
ASCII character #32. Char SP - Space
Integer ASCII code:	32
Binary code:	0010 0000
Octal code:	40
Hexadecimal code:	20
Group:	punctuation
Reference: https://www.asciihex.com/character/punctuation/32/0x20/sp-space

If it helps.

We can make sure it compiles later.

For now, no need to find out which of the registers it is, just need
to separate the first and second arg. We can map after with a quick helper
*/


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
outputnode assemble(inputnode *list)
{
    outputnode ret;
    outputnode* curr = &ret;
    while (list != NULL)
    {
        // add error handling?
        char *buff;
        command comm = *(list->data);
        // figure out better way to do ts later, temp solution but hash table probs best. or trie + switchcase
        if (!strcmp(comm.name, "halt"))
            buff = "00";
        else if (!strcmp(comm.name, "nop"))
            buff = "10";
        else if (!strcmp(comm.name, "rrmovl"))
            sprintf(buff, "20%x%x", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "irmovl"))
            sprintf(buff, "30f%x%s", reg_num(comm.rB), reg_num(comm.other));
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
            sprintf(buff, "70%s", reg_num(comm.other));
        else if (!strcmp(comm.name, "jle"))
            sprintf(buff, "71%s", reg_num(comm.other));
        else if (!strcmp(comm.name, "jl"))
            sprintf(buff, "72%s", reg_num(comm.other));
        else if (!strcmp(comm.name, "je"))
            sprintf(buff, "73%s", reg_num(comm.other));
        else if (!strcmp(comm.name, "jne"))
            sprintf(buff, "74%s", reg_num(comm.other));
        else if (!strcmp(comm.name, "jge"))
            sprintf(buff, "75%s", reg_num(comm.other));
        else if (!strcmp(comm.name, "jg"))
            sprintf(buff, "76%s", reg_num(comm.other));
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
        outputnode next;
        curr->next = &next;
        curr = &next;
        list = list->next;
    }
    return ret;
}
