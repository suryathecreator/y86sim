/*
 * @authors Surya Duraivenkatesh, Josh Tittiranonda
 * @date: 03/2025
 * Driver function for y86sim's Assembler.
 * ref: https://spcs.instructure.com/courses/8654/files/1593217?module_item_id=212606
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initial();

int main()
{
    /* Working file-open code. Looked up how to do this. I am able to split the files
       line by line now. Will modify as needed as I work through fns. */
    FILE *file = fopen("add3numbers.s", "r");
    if (file == NULL)
    {
        printf("File open failed. Please try again.\n");
        return 1;
    }
    char line[4096];
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0';
        printf("%s\n", line);
        char *word = strtok(line, " ");
        while (word != NULL)
        {
            // printf("Word:%s\n", word);
            if (strcmp(word, "%eax") == 0)
            {
                //    printf ("hi!");
            }
            word = strtok(NULL, " \t");
        }
    }
    fclose(file);
}

void initial()
{
    printf("%s\n", "Welcome to a Y86-64 Assembler Simulator! For an inputted instruction, the resulting action on the machine will be shown (e.g. register changes, memory changes, condition code changes, etc.)");
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
long addq(long remainder)
{
    // I would find a way to, from the hexadecimal representation of
    // part after (space) in addq (space) %rA, %rB
    // concatenate 6 + 0 + %rA + %rB
    // through string slicing

    // Then return that final thing. This slicing/number differs whether its subq, or mov, etc.

    // Once we do this for each fn, we can do the rest.
    // ****IMPORTANT****
    // Explicitly make two variables to define %rA and %rB for later, when we need to do the simulator interface
    // showing what was modified and when.
}

// assuming there is some linked-list structure of commands (to-implement)

int reg_num(char *reg)
{
}

typedef struct
{
    char *name, *rA, *rB, *other;
} command;

typedef struct
{
    inputnode *next;
    command *data;
} inputnode;

typedef struct
{
    outputnode *next;
    char *data;
} outputnode;

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
            sprintf(buff, "20%d%d", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "irmovl"))
            sprintf(buff, "30F%d%s", reg_num(comm.rB), reg_num(comm.other));
        else if (!strcmp(comm.name, "rmmovl"))
            sprintf(buff, "40%d%d%s", reg_num(comm.rA), reg_num(comm.rB), comm.other);
        else if (!strcmp(comm.name, "mrmovl"))
            sprintf(buff, "50%d%d%s", reg_num(comm.rA), reg_num(comm.rB), comm.other);
        else if (!strcmp(comm.name, "addl"))
            sprintf(buff, "60%d%d", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "subl"))
            sprintf(buff, "61%d%d", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "andl"))
            sprintf(buff, "62%d%d", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "xorl"))
            sprintf(buff, "63%d%d", reg_num(comm.rA), reg_num(comm.rB));
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
            sprintf(buff, "21%d%d", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "cmovl"))
            sprintf(buff, "22%d%d", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "cmove"))
            sprintf(buff, "23%d%d", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "cmovne"))
            sprintf(buff, "24%d%d", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "cmovge"))
            sprintf(buff, "25%d%d", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "cmovg"))
            sprintf(buff, "26%d%d", reg_num(comm.rA), reg_num(comm.rB));
        else if (!strcmp(comm.name, "call"))
            sprintf(buff, "80%s", comm.other);
        else if (!strcmp(comm.name, "ret"))
            sprintf(buff, "90");
        else if (!strcmp(comm.name, "pushl"))
            sprintf(buff, "A0%dF", comm.rA);
        else if (!strcmp(comm.name, "popl"))
            sprintf(buff, "B0%dF", comm.rA);
        else
            exit(1);
        curr->data = buff;
        outputnode next;
        curr->next = &next;
        curr = &next;
        list = list->next;
    }
}

// 12 things we needa do
// I'll do OPq (add/sub/etc.), jxx, call, ret, push, pop (last 6 on slide 4)
// (first 6 on slide 4, see ref link top of code file)
// You can do halt, nop, cmov, irmovq, rmmov, mrmov
//(halt and nop are indeed just one line each tho, but mov seems a bit longer so it should even it out for us)