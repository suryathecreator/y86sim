/*
 * @authors Surya Duraivenkatesh, Josh Tittiranonda
 * @date: 03/2025
 * Sundry queue methods to help with creating the linked list of assembly and binary commands.
 */

#include <stdio.h>
#include <stdbool.h>
#include "Queue.h"

bool emptyQueue(Queue*);
bool fullQueue(Queue*);
bool enqueue(Queue*, char*);
char* dequeue(Queue*);

// Queue implementation
void queueCreation(Queue* q)
{
    q->head = 0;
    q->tail = 0;
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
    q->arr[q->tail] = data;
    q->tail++;
    return true;
}
char* dequeue(Queue* q) {
    if (emptyQueue(q)) {
        printf("Empty queue. Try again.");
        return NULL;
    }
    q->head++;
    return q->arr[q->head-1];
}