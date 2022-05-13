#include "hash.h"
#include <assert.h>

void H_Insert(Graph *G, int userID, int friendID)
{
    // User does not exist
    if (G->userList[userID] == NULL || userID > G->maxUserID || userID < 0)
    {
        printf("\n\nUser with ID %d does not exist.\n\n", userID);
        return;
    }

    // Friend does not exist
    if (G->userList[friendID] == NULL || friendID > G->maxUserID || friendID < 0)
    {
        printf("\n\nUser with ID %d does not exist.\n\n", friendID);
        return;
    }

    // Points to friendList of corresponding user at userList[pos]
    int pos = friendID % G->userList[userID]->bucketNo;

    Node *temp = G->userList[userID]->friendList[pos], *newNode;

    newNode = CreateNode(); // malloc-ing a new node
    newNode->id = friendID;

    if (temp == NULL)
        G->userList[userID]->friendList[pos] = newNode;
    else
    {
        newNode->next = temp->next;
        temp->next = newNode;
    }

    ++(G->userList[userID]->numFriends); // Incrementing number of friends of user
}

// Prints useful messages to analyse the reason for failure of function
void H_Delete_With_Text(Graph *G, int userID, int friendID)
{
    // User does not exist
    if (G->userList[userID] == NULL || userID > G->maxUserID || userID < 0)
    {
        printf("\n\nUser with ID %d does not exist.\n\n", userID);
        return;
    }

    // Friend does not exist
    if (G->userList[friendID] == NULL || friendID > G->maxUserID || friendID < 0)
    {
        printf("\n\nUser with ID %d does not exist.\n\n", friendID);
        return;
    }

    int pos = friendID % G->userList[userID]->bucketNo;

    // Points to corresponding bucket number of friendList of userID
    Node *temp = G->userList[userID]->friendList[pos];

    if (temp == NULL)
    {
        printf("\n\n%s is not a friend of %s.\n\n", G->userList[friendID]->name, G->userList[userID]->name);
        return;
    }

    Node *previous = NULL;
    while (temp != NULL && temp->id != friendID) // Iterating through linked list
    {
        previous = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("\n\n%s is not a friend of %s.\n\n", G->userList[friendID]->name, G->userList[userID]->name);
        return;
    }

    if (previous == NULL)
        G->userList[userID]->friendList[pos] = temp->next;
    else
        previous->next = temp->next;

    printf("\nYou have unfriended %s.\n\n", G->userList[friendID]->name);

    --(G->userList[userID]->numFriends); // Decrementing number of friends of user

    free(temp);
}

// Does not print any messages even after failing. Required in places where text could mess up the UI
void H_Delete_Without_Text(Graph *G, int userID, int friendID)
{
    // User does not exist
    if (G->userList[userID] == NULL || userID > G->maxUserID || userID < 0)
        return;

    // Friend does not exist
    if (G->userList[friendID] == NULL || friendID > G->maxUserID || friendID < 0)
        return;

    // Points to corresponding bucket number of friendList of userID
    int pos = friendID % G->userList[userID]->bucketNo;

    Node *temp = G->userList[userID]->friendList[pos];

    if (temp == NULL)
        return;

    Node *previous = NULL;
    while (temp != NULL && temp->id != friendID)
    {
        previous = temp;
        temp = temp->next;
    }

    if (temp == NULL)
        return;

    if (previous == NULL)
        G->userList[userID]->friendList[pos] = temp->next;
    else
        previous->next = temp->next;

    --(G->userList[userID]->numFriends); // Decrementing number of friends of user

    free(temp);
}

// Searches for the friendID in friendList of the user and returns 1 if found, else 0
bool H_Search(Graph *G, int userID, int friendID)
{
    // Points to corresponding bucket number of friendList of userID
    int pos = friendID % G->userList[userID]->bucketNo;
    bool found = false;

    // User does not exist (we check for a negative friendID which is impossible)
    if (pos < 0)
        return false;

    Node *temp = G->userList[userID]->friendList[pos];

    if (temp == NULL)
    {
        return found;
    }

    while (temp != NULL)
    {
        if (temp->id == friendID)
        {
            found = true;
            break;
        }

        temp = temp->next;
    }

    return found;
}

void H_Rehash(Graph *G, int userID)
{
    // Creating a new friendList dynamically
    Node **newFriendList = (Node **)malloc(2 * G->userList[userID]->bucketNo * sizeof(Node *));

    for (int i = 0; i < 2 * G->userList[userID]->bucketNo; i++)
        newFriendList[i] = NULL;

    for (int i = 0; i < G->userList[userID]->bucketNo; i++)
    {
        Node *temp = G->userList[userID]->friendList[i];

        Node *holder = NULL;
        while (temp != NULL)
        {
            // Inserting each node into the new friendList
            int pos = temp->id % (2 * G->userList[userID]->bucketNo);
            Node *newNode = CreateNode();
            newNode->id = temp->id;

            if (newFriendList[pos] == NULL)
                newFriendList[pos] = newNode;
            else
            {
                newNode->next = newFriendList[pos]->next;
                newFriendList[pos]->next = newNode;
            }

            // Deleting the node from the old friendList
            holder = temp;
            temp = temp->next;
            free(holder);
        }
    }

    free(G->userList[userID]->friendList);
    G->userList[userID]->bucketNo *= 2;              // Double number of buckets of user's new friendList
    G->userList[userID]->friendList = newFriendList; // Assign new friendList to user
}
