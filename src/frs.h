#ifndef FRS_H
#define FRS_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "queue.h"

// Required for NewFriends()
// Stores a matrix containing linked lists containing user IDs
// The linked list at grid[i][j] has a string parameter whose length is i + 1 and starts with the (j + 1)th letter of the alphabet
typedef struct StringHashtable
{
    Node *grid[13][26];
} StringHashtable;

// A normal hash table which stores integers
typedef struct IntHashtable
{
    Node *array[100];
} IntHashtable;

StringHashtable *CreateSHT();
void DeleteStringHashtable(StringHashtable *S);

// The information about each user is contained in a struct
typedef struct User
{
    int id;            // ID assigned to user
    char name[20];     // Name of user
    int age;           // Age of user
    char city[15];     // Name of city of user
    char school[15];   // Name of school user went to
    int numFriends;    // Number of friends user has
    Node **friendList; // List of friends (a hash table)
    int bucketNo;      // Number of buckets in friendList
} User;

/*
 * We implement our network as a graph.
 * The graph is represented as an array of hash tables.
 * Each element of the hash table is pointer to a linked list of struct Users.
 * That is, we use separate chaining in our graph.
*/
typedef struct Graph
{
    int numUsers;    // Number of users
    User **userList; // List of users
    int maxUserID;   // Stores the maximum user ID in userList
    int *minHeap;    // To retrieve minimum possible ID from deleted elements
    int heapSize;
    IntHashtable *Age; //
    StringHashtable *City; //
    StringHashtable *School; //
} Graph;

User *CreateUser();        // Initialise an empty user
User **CreateUserList();   // Initialise an empty userlist
Node **CreateFriendList(); // Initialise an empty integer array friendList
Graph *CreateGraph();      // Initialise an empty graph

void AddUser(Graph *G);            // To register a user
void RemoveUser(Graph *G, int id); // To unregister a user

void removefriendofuser(Graph *G, int num); // To remove user with ID num from friendList of users

void AddFriend(Graph *G, int userID, int friendID);    // To add a friend
void RemoveFriend(Graph *G, int userID, int friendID); // To unfriend someone
void IsFriend(Graph *G, int idA, int idB);             // To check if A is a friend of B

// To recommend friends to existing users
void RegFriends(Graph *G, int userID, int K);
// To recommend friends to new users
void NewFriends(Graph *G, int userID);

void DeleteFriendList(Graph *G, int userID); // To delete a user's friendList
void DeleteGraph(Graph *G);                  // To delete the graph at the end of the program

void delay(int number_of_seconds); // To cause a time delay, used for UI

//---------------NEWUSER.c FUNCTIONS---------------//

// Inserts ID into SHT
void SHTCity_Insert(Graph *G, StringHashtable *S, int id);
// Deletes ID from SHT
void SHTCity_Delete(Graph *G, StringHashtable *S, int id);
// Traverses linked list of S->grid corresponding to city and stores the IDs in A
void SHTCity_Traverse(Graph *G, StringHashtable *S, int A[], int *n, char city[]);

// The same functions as above, but for school
void SHTSchool_Insert(Graph *G, StringHashtable *S, int id);
void SHTSchool_Delete(Graph *G, StringHashtable *S, int id);
void SHTSchool_Traverse(Graph *G, StringHashtable *S, int A[], int *n, char city[]);

IntHashtable *CreateIHT();
void DeleteIHT(IntHashtable *I);

// Inserts ID into IHT
void IHT_Insert(Graph *G, IntHashtable *I, int id);
// Deletes ID from IHT
void IHT_Delete(Graph *G, IntHashtable *I, int id);
//Traverses linked list at I->array[age % 100] and stores the IDs matching age in A
void IHT_Traverse(Graph *G, IntHashtable *I, int A[], int *n, int age);

#endif
