#ifndef COMMANDLIST_H
#define COMMANDLIST_H

typedef struct inputnode inputnode;
typedef struct outputnode outputnode;

typedef struct
{
    char *name, *rA, *rB, *other;
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