#include "heap.h"

void swap(pair *A, pair *B)
{
    pair temp = *A;
    *A = *B;
    *B = temp;
}

//-------------------MIN HEAP---------------------//

void MinHeapify(int A[], int heapSize, int i)
{
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    int smallest = i;

    if (l < heapSize && A[l] < A[i])
        smallest = l;
    if (r < heapSize && A[r] < A[smallest])
        smallest = r;

    if (smallest != i)
    {
        int temp = A[i];
        A[i] = A[smallest];
        A[smallest] = temp;
        MinHeapify(A, heapSize, smallest);
    }
}

// Deletes from minHeap and decrements heapSize
void Delete(int A[], int *heapSize)
{
    --(*heapSize);

    int temp = A[*heapSize];
    A[*heapSize] = A[0];
    A[0] = temp;

    MinHeapify(A, *heapSize, 0);
}

// Inserts into minHeap and increments heapSize
void Insert(int A[], int *heapSize, int elem)
{
    ++(*heapSize);
    A[*heapSize - 1] = elem;

    for (int i = *heapSize - 1; i >= 0 && A[i] < A[(i - 1) / 2]; i = (i - 1) / 2)
    {
        int temp = A[i];
        A[i] = A[(i - 1) / 2];
        A[(i - 1) / 2] = temp;
    }
}

//-------------------MAX HEAP/PRIORITY QUEUE---------------------//

void MaxHeapify(pair A[], int heapSize, int i)
{
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    int largest = i;

    if (l < heapSize && A[l].count > A[i].count)
        largest = l;
    if (r < heapSize && A[r].count > A[largest].count)
        largest = r;

    if (largest != i)
    {
        swap(&A[i], &A[largest]);
        MaxHeapify(A, heapSize, largest);
    }
}

// Deletes from maxHeap and decrements heapSize
void MH_Delete(pair A[], int *heapSize)
{
    --(*heapSize);

    swap(&A[0], &A[*heapSize]);

    MaxHeapify(A, *heapSize, 0);
}

// Inserts into maxHeap and increments heapSize
void MH_Insert(pair A[], int *heapSize, int elem1, short int elem2)
{
    ++(*heapSize);
    A[*heapSize - 1].index = elem1;
    A[*heapSize - 1].count = elem2;

    for (int i = *heapSize - 1; i >= 0 && A[i].count > A[(i - 1) / 2].count; i = (i - 1) / 2)
        swap(&A[i], &A[(i - 1) / 2]);
}
