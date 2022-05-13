#ifndef HEAP_H
#define HEAP_H

// Required to store index-count pairs in NewFriends()
typedef struct pair
{
    int index;
    short int count;
} pair;

void MinHeapify(int A[], int heapSize, int i);
void Insert(int A[], int *heapSize, int elem);
void Delete(int A[], int *heapSize);

void MaxHeapify(pair A[], int heapSize, int i);
void MH_Delete(pair A[], int *heapSize);
void MH_Insert(pair A[], int *heapSize, int elem1, short int elem2);

#endif