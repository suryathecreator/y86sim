/*
 * @authors Surya Duraivenkatesh, Josh Tittiranonda
 * @date: 03/2025
 * Header file to define a queue-derived symbolic map struct to help with ensuring symbolic names are mapped to their respective memory addresses. Can make into hashmap later.
 */

#ifndef SYMBOLICMAP_H
#define SYMBOLICMAP_H
#define MAX_QUEUE_SIZE 1000 // Arbitrary.

typedef struct element element;

typedef struct element {
     char *name;
     long address;
     element *next;
} element;

typedef struct {
     element* arr[MAX_QUEUE_SIZE];
     int tail;
     int head;
} map;



#endif