#include "queue.h"

/*
 * Implementation of queue needed for BFS in RegFriends 
*/

NodePtr CreateNode()
{
    NodePtr N = (NodePtr)malloc(sizeof(Node));
    assert(N != NULL && "Malloc returned NULL in CreateNode().");
    N->next = NULL;

    return N;
}

QueuePtr CreateQueue()
{
    QueuePtr Q = (QueuePtr)malloc(sizeof(Queue));
    assert(Q != NULL && "Malloc returned NULL in CreateQueue().");
    Q->front = NULL;
    Q->rear = NULL;

    return Q;
}

// Enqueues node at rear of queue
QueuePtr Enqueue(QueuePtr Q, int id)
{
    if (Q == NULL)
    {
        printf("Error occurred. Queue does not exist.\n");
        return NULL;
    }

    NodePtr newNode = CreateNode();
    newNode->id = id;

    if (Empty(Q))
    {
        Q->front = newNode;
        Q->rear = newNode;
    }
    else
    {
        Q->rear->next = newNode;
        Q->rear = newNode;
    }

    return Q;
}

// Dequeues node from front of queue
QueuePtr Dequeue(QueuePtr Q)
{
    if (Q == NULL)
    {
        printf("Error occurred. Queue does not exist.\n");
    }
    else if (Empty(Q))
    {
        printf("Error occurred. Queue is empty.\n");
    }
    else if (Q->front == Q->rear)
    {
        free(Q->front);
        Q->front = NULL;
        Q->rear = NULL;
    }
    else
    {
        NodePtr temp = Q->front;
        Q->front = temp->next;
        free(temp);
    }

    return Q;
}

// Checks is queue is empty
bool Empty(QueuePtr Q)
{
    return (Q->front == NULL && Q->rear == NULL);
}

// Frees whole queue
QueuePtr DeleteQueue(QueuePtr Q)
{
    if (Q == NULL)
    {
        printf("Error occurred. Queue does not exist.");
    }
    else
    {
        NodePtr temp = Q->front, holder;
        while (temp != NULL)
        {
            holder = temp;
            temp = temp->next;
            free(holder);
        }

        free(Q);
    }
    
    return NULL;
}