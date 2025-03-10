/*
 * @authors Surya Duraivenkatesh, Josh Tittiranonda
 * @date: 03/2025
 * Sundry queue methods to help with creating the linked list of assembly and binary commands.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "Queue.h"

bool emptyQueue(Queue*);
bool fullQueue(Queue*);
bool enqueue(Queue*, char*);
char* dequeue(Queue*);

// Queue implementation
Queue* queueCreation(int size)
{
    Queue* q = malloc(sizeof(Queue));
    q->head = 0;
    q->tail = 0;
    q->arr = malloc(sizeof(char*) * size);
    return q;
}
bool emptyQueue(Queue* q)
{
    return (q->head == q->tail);
}
bool fullQueue(Queue* q) {
    return (q->tail == MAX_LINE_COUNT);
}
bool enqueue(Queue* q, char* data) {
    if (fullQueue(q)) {
        printf("Full queue. Try again.");
        return false;
    }
//    printf("Enqueueing %s, with tail %d\n", data, q->tail);
    q->arr[q->tail] = malloc(strlen(data)*sizeof(char) + 1);
    strcpy(q->arr[q->tail], data);
    q->tail++;
    return true;
}
char* dequeue(Queue* q) {
    if (emptyQueue(q)) {
        printf("Empty queue. Try again.");
        return NULL;
    }
    
    char* ret = q->arr[q->head];
    q->head++;
    return ret;
}

void printQueue(Queue *q) {
    int counter = q->head;
//    printf("Head: %d\n", q->head);
//    printf("Tail: %d\n", q->tail);
    while (counter < q->tail) {
//        printf("Counter in-loop: %d\n", counter);
//        printf("arr[counter]: %s\n", q->arr[counter]);
        printf("%s\n", q->arr[counter]);
        counter++;
    }
}