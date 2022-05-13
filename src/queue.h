#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

// #define NDEBUG

// Node of a queue
typedef struct Node
{
    int id;
    struct Node* next; 
} Node;

typedef Node* NodePtr;

// The ADT for a queue, using linked list
typedef struct Queue
{
    NodePtr front; // Points to front
    NodePtr rear; // Points to rear
} Queue;

typedef Queue* QueuePtr;

NodePtr CreateNode(); // Creates a new node
QueuePtr CreateQueue(); // To create a queue
QueuePtr Enqueue(QueuePtr Q, int id); // To enqueue an element
QueuePtr Dequeue(QueuePtr Q); // To dequeue an element
bool Empty(QueuePtr Q); // To check whether queue is empty or not
QueuePtr DeleteQueue(QueuePtr Q); // To delete queue

#endif