/*
 * @authors Surya Duraivenkatesh, Josh Tittiranonda
 * @date: 03/2025
 * Structs defined within y86sim's Assembler.
 * ref: https://spcs.instructure.com/courses/8654/files/1593217?module_item_id=212606
 */
typedef struct {
     char* arr[MAX_LINE_COUNT];
     int tail;
     int head;
} Queue;

// Forward declaration needed in C
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
