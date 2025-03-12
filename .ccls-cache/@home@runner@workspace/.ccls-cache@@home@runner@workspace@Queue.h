/*
 * @authors Surya Duraivenkatesh, Josh Tittiranonda
 * @date: 03/2025
 * Header file to define a queue struct to help with creating the linked list of assembly and binary commands.
 */

#ifndef QUEUE_H
#define QUEUE_H
#define MAX_LINE_COUNT 1000

typedef struct {
     char** arr;
     int tail;
     int head;
} Queue;

#endif