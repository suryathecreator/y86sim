/*
 * @authors Surya Duraivenkatesh, Josh Tittiranonda
 * @date: 03/2025
 * Sundry queue methods to help with creating the linked list of assembly and binary commands.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "string.h"
#include "symbolicMap.h"

map* mapCreation(int);
bool empty(map*);
bool full(map*);
bool add(map*, element*);
bool del(map*);
int findAddress(map*, char*);

// Constructor
map* mapCreation(int size)
{
    map* m = malloc(sizeof(map));
    m->head = 0;
    m->tail = 0;
    m->arr = malloc(sizeof(element*) * size);
    return m;
}

// Map modification functions
bool empty(map* m)
{
    return (m->head == m->tail);
}
bool full(map* m) {
    return (m->tail == MAX_QUEUE_SIZE);
}
bool add(map* m, element* data) {
    if (full(m)) {
        printf("Full queue. Try again.");
        return false;
    }
    //    printf("Enqueueing %s, with tail %d\n", data, q->tail);
    m->arr[m->tail] = malloc(sizeof(element));
    m->arr[m->tail]->name = malloc(strlen(data->name)*sizeof(char) + 1);
    strcpy(m->arr[m->tail]->name, data->name);

    m->arr[m->tail]->address = data->address;
    
    m->tail++;
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
    int counter = m->head;
    while (counter < m->tail) {
        if (!strcmp(m->arr[counter]->name, name)) {
            return (m->arr[counter]->address);
        }
        counter++;
    }
    return -1;
}
// Map-finding (hashmap better than this, but may take a while to implement, so this is future work)
    // Goal is not efficient, but rather to implement the emulator.

void printMap(map *m) {
    int counter = m->head;
    while (counter < m->tail) {
        printf("%s\t%i", m->arr[counter]->name, m->arr[counter]->address);
        counter++;
    }
}