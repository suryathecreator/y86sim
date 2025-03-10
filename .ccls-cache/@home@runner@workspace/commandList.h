/*
 * @authors Surya Duraivenkatesh, Josh Tittiranonda
 * @date: 03/2025
 * Header file to define the nodes and command structs to help with creating the linked list of assembly and binary commands.
 */

#ifndef COMMANDLIST_H
#define COMMANDLIST_H
#include "symbolicMap.h"

// Forward declaration needed in C
typedef struct inputnode inputnode;
typedef struct outputnode outputnode;

typedef struct
{
    char *name, *rA, *rB, *other;
    bool symbol, directive, pos, align, long_or_quad; // Flag for directive, long and quad both are 8-bye values at the current memory address.
    long position, alignment; // For directives
    element *symbolicName ; // For symbolic-name commands
} command;

typedef struct inputnode
{
    inputnode *next;
    command *data;
} inputnode;

typedef struct outputnode
{
    outputnode *next;
    char *data;
} outputnode;

#endif