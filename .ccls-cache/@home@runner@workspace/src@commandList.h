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
    bool symbol, directive, pos, align, long_or_quad, usesSymbolicName; // Flag for directive, long and quad both are 8-bye values at the current memory address.
    int position, alignment; // For directives
    unsigned long value;
    element *symbolicName; // For symbolic-name commands
} command;

typedef struct inputnode
{
    inputnode *next;
    command *data;
} inputnode;

typedef struct outputnode
{
    inputnode *assembly; // Assembly version of the binary command for two-pass assembly/symbolic name resolution
    outputnode *next;
    char *data;
    unsigned long memoryAddress;
} outputnode;

#endif