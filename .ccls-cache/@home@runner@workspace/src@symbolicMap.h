/*
 * @authors Surya Duraivenkatesh, Josh Tittiranonda
 * @date: 03/2025
 * Header file to define a queue-derived symbolic map struct to help with ensuring symbolic names are mapped to their respective memory addresses. Can make into hashmap later.
 */

#ifndef SYMBOLICMAP_H
#define SYMBOLICMAP_H
#define MAX_QUEUE_SIZE 1000 // Arbitrary.

typedef struct element element; // Forward declaration required by C

typedef struct element {
     char *name;
     unsigned long address;
     element *next;
} element;

typedef struct {
     element** arr;
     int tail;
     int head;
} map;

#endif