
#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#define INITIAL_CAPACITY 10
typedef struct {
    int *data;        // Array to store queue elements
    int front;        // Index of the front element
    int rear;         // Index of the rear element
    int capacity;     // Current capacity of the queue
    int size;         // Number of elements in the queue
} queue;
queue* queue_build();
int queue_isempty(queue *q) ;
void queue_increament(queue *q) ;
void queue_push(queue *q, int value) ;
int queue_pop(queue *q) ;
void queue_display(queue *q) ;
#endif
