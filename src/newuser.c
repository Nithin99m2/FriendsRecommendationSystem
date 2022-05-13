#include "frs.h"

//-------------Linked List functions-------------//

Node *CreateLLNode()
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->next = NULL;

    return newNode;
}

Node *SLL_Insert(Node *head, int n)
{
    Node *newHead = CreateLLNode();

    newHead->next = head;
    newHead->id = n;

    return newHead;
}

Node *SLL_Delete(Node *head, int n)
{
    Node *temp = head, *previous = NULL;

    while (temp != NULL && temp->id != n)
    {
        previous = temp;
        temp = temp->next;
    }

    if (temp == NULL)
        return NULL;
    else if (previous == NULL)
    {
        head = temp->next;
        free(temp);
        return head;
    }
    else
    {
        previous->next = temp->next;
        free(temp);
        return head;
    }
}

void DeleteSLL(Node *head)
{
    Node *temp = head, *holder = NULL;

    while (temp != NULL)
    {
        holder = temp;
        temp = temp->next;
        free(holder);
    }
}

//-------------IntHashtable-------------//

// IHT -> Age

IntHashtable *CreateIHT()
{
    IntHashtable *I = (IntHashtable *)malloc(sizeof(IntHashtable));

    for (int i = 0; i < 100; i++)
        I->array[i] = NULL;

    return I;
}

void DeleteIHT(IntHashtable *I)
{
    for (int i = 0; i < 100; i++)
       DeleteSLL(I->array[i]);

    free(I);
}

void IHT_Insert(Graph *G, IntHashtable *I, int id)
{
    int i = G->userList[id]->age % 100;

    I->array[i] = SLL_Insert(I->array[i], id); // Inserting into linked list at array[i]
}

void IHT_Delete(Graph *G, IntHashtable *I, int id)
{
    int i = G->userList[id]->age;

    I->array[i] = SLL_Delete(I->array[i], id); // Deleting from linked list at array[i]
}

void IHT_Traverse(Graph *G, IntHashtable *I, int A[], int *n, int age)
{
    Node *temp = I->array[age];

    while (temp != NULL)
    {
        if (G->userList[temp->id]->age == age)
        {
            A[*n] = temp->id;
            ++(*n);
        }

        temp = temp->next;
    }
}

//-------------StringHashtable Functions-------------//

// SHT -> City

StringHashtable *CreateSHT()
{
    StringHashtable *S = (StringHashtable *)malloc(sizeof(StringHashtable));

    for (int i = 0; i < 13; i++)
        for (int j = 0; j < 26; j++)
            S->grid[i][j] = NULL; // Initialising every element of 2D matrix to NULL

    return S;
}

void DeleteStringHashtable(StringHashtable *S)
{
    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 26; j++)
            DeleteSLL(S->grid[i][j]); // Deleting each linked list
    }

    free(S);       // Freeing StringHashtable
}

void SHTCity_Insert(Graph *G, StringHashtable *S, int id)
{
    int i = strlen(G->userList[id]->city) - 1; // Checking length of city of user ID
    int j = G->userList[id]->city[0] - 'A';    // Checking first letter of city of user ID

    S->grid[i][j] = SLL_Insert(S->grid[i][j], id); // Inserting into linked list at grid[i][j]
}

void SHTCity_Delete(Graph *G, StringHashtable *S, int id)
{
    int i = strlen(G->userList[id]->city) - 1; // Checking length of city of user ID
    int j = G->userList[id]->city[0] - 'A';    // Checking first letter of city of user ID

    S->grid[i][j] = SLL_Delete(S->grid[i][j], id); // Deleting from linked list at grid[i][j]
}

void SHTCity_Traverse(Graph *G, StringHashtable *S, int A[], int *n, char city[])
{
    int i = strlen(city) - 1; // Checking length of city of user ID
    int j = city[0] - 'A';    // Checking first letter of city of user ID

    Node *temp = S->grid[i][j];

    while (temp != NULL) // Traverses through linked list at grid[i][j]
    {
        if (strcmp(G->userList[temp->id]->city, city) == 0)
        {
            A[*n] = temp->id; // Storing IDs of users who are from city
            ++(*n);
        }

        temp = temp->next;
    }
}

// SHT -> School

void SHTSchool_Insert(Graph *G, StringHashtable *S, int id)
{
    int i = strlen(G->userList[id]->school) - 1; // Checking length of city of user ID
    int j = G->userList[id]->school[0] - 'A';    // Checking first letter of city of user ID

    S->grid[i][j] = SLL_Insert(S->grid[i][j], id); // Inserting into linked list at grid[i][j]
}

void SHTSchool_Delete(Graph *G, StringHashtable *S, int id)
{
    int i = strlen(G->userList[id]->school) - 1; // Checking length of city of user ID
    int j = G->userList[id]->school[0] - 'A';    // Checking first letter of city of user ID

    S->grid[i][j] = SLL_Delete(S->grid[i][j], id); // Deleting from linked list at grid[i][j]
}

void SHTSchool_Traverse(Graph *G, StringHashtable *S, int A[], int *n, char school[])
{
    int i = strlen(school) - 1; // Checking length of city of user ID
    int j = school[0] - 'A';    // Checking first letter of city of user ID

    Node *temp = S->grid[i][j];

    while (temp != NULL) // Traverses through linked list at grid[i][j]
    {
        if (strcmp(G->userList[temp->id]->school, school) == 0)
        {
            A[*n] = temp->id; // Storing IDs of users who are from city
            ++(*n);
        }

        temp = temp->next;
    }
}
