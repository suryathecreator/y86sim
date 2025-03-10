/*
 * @authors Surya Duraivenkatesh, Josh Tittiranonda
 * @date: 03/2025
 * Header file to define a hashmap struct to help with ensuring symbolic names are mapped to their respective memory addresses.
 */

#ifndef SYMBOLICMAP_H
#define SYMBOLICMAP_H
#define MAX_HASHMAP_SIZE 1000 // Arbitrary.

typedef struct {
     char *name;
     long address;
} element;

typedef struct {
     
} map;

#endif