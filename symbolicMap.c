/*
 * @authors Surya Duraivenkatesh, Josh Tittiranonda
 * @date: 03/2025
 * Sundry queue methods to help with creating the linked list of assembly and binary commands.
 */

#include <stdio.h>
#include <stdbool.h>
#include "string.h"
#include "symbolicMap.h"

void mapCreation(map*);
bool empty(map*);
bool full(map*);
bool add(map*, element*);
bool del(map*);
int findAddress(map*, char*);


void mapCreation(map* q)
{
    q->head = 0;
    q->tail = 0;
}
bool empty(map* m)
{
    return (m->head == m->tail);
}
bool full(map* m) {
    return (m->tail == MAX_QUEUE_SIZE);
}

bool add(map* q, element* data) {
    if (full(q)) {
        printf("Full queue. Please contact the developer to fix a logical error within the program.");
        // Would be for debugging only.
        return false;
    }
    q->arr[q->tail] = data;
    q->tail++;
    return true;
}

bool del(map* m) {
    if (empty(m)) {
        printf("Empty queue. Please contact the developer to fix a logical error within the program.");
        // Would be for debugging only.
        return false;
    }
    m->head++;
    return true;
}

int findAddress(map *m, char *name) {
    while (m->head < m->tail) {
        if (!strcmp(m->arr[m->head]->name, name)) {
            return (m->arr[m->head]->address);
        }
    }
    return -1;
}
// Map-finding (hashmap better than this, but may take a while to implement, so this is future work)
    // Goal is not efficient, but rather to implement the emulator.

void printmap(map *m) {
    int counter = m->head;
    while (counter < m->tail) {
        printf("%s\t%i", m->arr[counter]->name, m->arr[counter]->address);
        counter++;
    }
}