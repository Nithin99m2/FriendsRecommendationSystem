#include "frs.h"
#include <assert.h>
#include "queue.h"
#include "hash.h"
#include "heap.h"

// #define NDEBUG

//-------------ALLOCATION-------------//

User *CreateUser()
{
    User *U = (User *)malloc(sizeof(User));
    assert(U != NULL && "Malloc returned NULL in CreateUser().");
    U->friendList = CreateFriendList();
    U->numFriends = 0;
    U->bucketNo = 2;

    return U;
}

User **CreateUserList()
{
    User **U = (User **)malloc(2 * sizeof(User *)); // Initial array size of 2
    assert(U != NULL && "Malloc returned NULL in CreateUserList().");
    U[0] = U[1] = NULL;

    return U;
}

Node **CreateFriendList()
{
    Node **FL = (Node **)malloc(2 * sizeof(Node *)); // Initial array size of 2
    assert(FL != NULL && "Malloc returned NULL in CreateFriendList().");
    FL[0] = FL[1] = NULL;

    return FL;
}

Graph *CreateGraph()
{
    Graph *G = (Graph *)malloc(sizeof(Graph));
    assert(G != NULL && "1st Malloc returned NULL in CreateGraph().");
    G->userList = CreateUserList();
    G->numUsers = 0;
    G->maxUserID = -1;
    G->minHeap = (int *)malloc(2 * sizeof(int));
    assert(G != NULL && "2nd Malloc returned NULL in CreateGraph().");
    G->heapSize = 0;
    G->Age = CreateIHT();    //
    G->City = CreateSHT();   //
    G->School = CreateSHT(); //

    return G;
}

//-------------DEALLOCATION-------------//

void DeleteFriendList(Graph *G, int userID)
{
    for (int i = 0; i < G->userList[userID]->bucketNo; i++)
    {
        Node *temp = G->userList[userID]->friendList[i];

        Node *holder;
        while (temp != NULL)
        {
            holder = temp;
            temp = temp->next;
            free(holder);
        }
    }
}

void DeleteGraph(Graph *G)
{
    for (int i = 0; i <= G->maxUserID; i++)
    {
        if (G->userList[i] != NULL)
        {
            DeleteFriendList(G, i);
            free(G->userList[i]->friendList);
            free(G->userList[i]);
        }
    }

    DeleteIHT(G->Age);                // Deleting integer hash table Age
    DeleteStringHashtable(G->City);   // Deleting string hash table City
    DeleteStringHashtable(G->School); // Deleting string hash table School
    free(G->minHeap);
    free(G->userList);
    free(G);
}

//-------------FUNCTIONALITIES-------------//

void AddUser(Graph *G)
{
    User *U = CreateUser();

    // Assign ID to new user
    if (G->heapSize == 0)
    {
        ++G->maxUserID; // If no elements in minHeap (no deleted IDs left), then User is stored at end of userList
        U->id = G->maxUserID;
    }
    else
    {
        U->id = G->minHeap[0]; // If there was a deleted user, then their ID is unused. We select the minimum of the deleted IDs
        Delete(G->minHeap, &G->heapSize);
    }

    ++G->numUsers; // Increasing number of users

    // Input user details
    printf("Enter your name: ");
    scanf(" %[^\n]", U->name);
    printf("Enter your age: ");
    scanf("%d", &U->age);
    printf("Enter your city: ");
    scanf(" %[^\n]", U->city);
    printf("Enter your school: ");
    scanf(" %[^\n]", U->school);

    G->userList[U->id] = U; // Putting user in the correct position in userList

    /*
     * We check if userList is full.
     * We do this by checking if numUsers is a power of 2
     * The capacity of userList is always successive powers of 2, and if numUsers == capacity, then it must be the next power of 2
     * If A is a power of 2, then A & (A - 1) = 0
     * We check if it's greater than 1 because 1 is a power of 2 but initial array size is 2
    */
    if (((G->numUsers & (G->numUsers - 1)) == 0) && (G->numUsers > 1))
    {
        G->userList = realloc(G->userList, 2 * G->numUsers * sizeof(User *)); // If full, we double the capacity
        for (int i = G->numUsers; i < 2 * G->numUsers; i++)
            G->userList[i] = NULL;
    }

    IHT_Insert(G, G->Age, U->id);          // Storing new user in corresponding group as users of same age
    SHTCity_Insert(G, G->City, U->id);     // Storing new user in corresponding group as users from same city
    SHTSchool_Insert(G, G->School, U->id); // Storing new user in corresponding group as users from same school

    printf("\nYou have been successfully registered with ID %d!\n\n", U->id);
}

void RemoveUser(Graph *G, int id)
{
    // User does not exist
    if (G->userList[id] == NULL || id > G->maxUserID || id < 0)
    {
        printf("\n\nUser with ID %d does not exist.\n\n", id);
        return;
    }

    // Removing deleted user from friendList of all users
    removefriendofuser(G, id);

    // Inserting deleted ID into the minHeap
    Insert(G->minHeap, &G->heapSize, id);
    --G->numUsers; // Decreasing number of users

    if (((G->heapSize) & (G->heapSize - 1) == 0) && (G->heapSize != 1))
        G->minHeap = realloc(G->minHeap, 2 * G->heapSize);

    IHT_Delete(G, G->Age, id);          // Deleting users from the age group they are in
    SHTCity_Delete(G, G->City, id);     // Deleting users from the city group they are in
    SHTSchool_Delete(G, G->School, id); // Deleting users from the school group they are in

    DeleteFriendList(G, id);
    free(G->userList[id]->friendList);
    free(G->userList[id]);
    G->userList[id] = NULL;
}

void removefriendofuser(Graph *G, int num)
{
    // Iterating through userList
    for (int i = 0; i <= G->maxUserID; i++)
    {
        if (G->userList[i] == NULL || i == num)
            continue;

        // Deleting userID num from friendList of user with ID i
        H_Delete_Without_Text(G, i, num);
    }
}

void AddFriend(Graph *G, int userID, int friendID)
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

    if (H_Search(G, userID, friendID))
    {
        printf("\n\n%s is already a friend of %s.\n\n", G->userList[friendID]->name, G->userList[userID]->name);
        return;
    }

    // Inserting it into the hash table friendList of user
    H_Insert(G, userID, friendID);

    // If load factor of friendList is 1, then we rehash
    if (G->userList[userID]->numFriends / G->userList[userID]->bucketNo == 1)
        H_Rehash(G, userID);

    printf("\n%s is now a friend of %s!\n", G->userList[friendID]->name, G->userList[userID]->name);
}

void RemoveFriend(Graph *G, int userID, int friendID)
{
    if (friendID == userID)
    {
        printf("\n\nA user cannot be their own friend.\n\n");
        return;
    }

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

    // Calling H_Delete_With_Text which deleted friend from friendList using hashing
    H_Delete_With_Text(G, userID, friendID);
}

void IsFriend(Graph *G, int idA, int idB)
{
    if (idA == idB)
    {
        printf("\n\nA user cannot be their own friend.\n\n");
        return;
    }

    // User does not exist
    if (G->userList[idA] == NULL || idA > G->maxUserID || idA < 0)
    {
        printf("\n\nUser with ID %d does not exist.\n\n", idA);
        return;
    }

    // Friend does not exist
    if (G->userList[idB] == NULL || idB > G->maxUserID || idB < 0)
    {
        printf("\n\nUser with ID %d does not exist.\n\n", idB);
        return;
    }

    // H_Search searches the hash table friendList for idB, and returns 1 if found and 0 if not found
    if (H_Search(G, idA, idB))
        printf("\n%s is a friend of %s.\n", G->userList[idA]->name, G->userList[idB]->name);
    else
        printf("\n%s is not a friend of %s.\n", G->userList[idA]->name, G->userList[idB]->name);
}

// We perform BFS to find second friends, third friends and so on
void RegFriends(Graph *G, int userID, int K)
{
    // User does not exist
    if (G->userList[userID] == NULL || userID > G->maxUserID || userID < 0)
    {
        printf("\n\nUser with ID %d does not exist.\n\n", userID);
        return;
    }

    printf("\nWould you like to make some friends? Here is a list of recommendations!\n\n");

    bool visited[G->numUsers];
    QueuePtr Q = CreateQueue();

    // We subtract the number of friends of user from total number of users
    int n = G->numUsers - G->userList[userID]->numFriends - 1;
    K = (n <= K ? n : K);

    int recFriends[K + 2]; // To store recommended users
    int recFC = 0;         // Iterator through reFriends

    for (int i = 0; i < G->numUsers; i++)
        visited[i] = false;
    visited[userID] = true;
    // Marking user and user's friends as already visited
    // Enqueueing friends
    for (int i = 0; i < G->userList[userID]->bucketNo; i++)
    {
        Node *temp = G->userList[userID]->friendList[i];

        while (temp != NULL)
        {
            visited[temp->id] = true;
            Enqueue(Q, temp->id);
            temp = temp->next;
        }
    }

    printf("ID\t|Name\t\t\t|Age\t|City\t\t\t|School\t\t\t|Number of Friends\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n");

    while (!Empty(Q))
    {
        int u = Q->front->id; // u -> current node in queue
        Dequeue(Q);

        for (int i = 0; i < G->userList[u]->bucketNo; i++) // Traversing through hash table
        {
            Node *temp = G->userList[u]->friendList[i];

            while (temp != NULL) // Traversing through each linked list in hash table
            {
                if (!visited[temp->id])
                {
                    visited[temp->id] = true;
                    Enqueue(Q, temp->id);
                }

                temp = temp->next;
            }
        }

        // Checking if user with ID u is a friend of user and printing
        if (!H_Search(G, userID, u))
        {
            printf("%d\t", G->userList[u]->id);
            printf("|%s\t\t", G->userList[u]->name);
            if (strlen(G->userList[u]->name) < 7)
                printf("\t");
            printf("|%d\t", G->userList[u]->age);
            printf("|%s\t\t", G->userList[u]->city);
            if (strlen(G->userList[u]->city) < 7)
                printf("\t");
            printf("|%s\t\t", G->userList[u]->school);
            if (strlen(G->userList[u]->school) < 7)
                printf("\t");
            printf("|%d friends\n", G->userList[u]->numFriends);

            recFriends[recFC] = u;
            ++recFC;

            --K;
            if (!K)       // We check if K = 0
                goto end; // If K = 0, then we terminate the process
        }
    }

    // To visit any leftover users if K != 0
    for (int i = 0; i < G->numUsers && K > 0; i++)
    {
        if (!visited[i])
        {
            printf("%d\t", G->userList[i]->id);
            printf("|%s\t\t", G->userList[i]->name);
            if (strlen(G->userList[i]->name) < 7)
                printf("\t");
            printf("|%d\t", G->userList[i]->age);
            printf("|%s\t\t", G->userList[i]->city);
            if (strlen(G->userList[i]->city) < 7)
                printf("\t");
            printf("|%s\t\t", G->userList[i]->school);
            if (strlen(G->userList[i]->school) < 7)
                printf("\t");
            printf("|%d friends\n", G->userList[i]->numFriends);

            recFriends[recFC] = i;
            ++recFC;

            --K; // If K = 0, then we terminate the process
        }
    }

end:
    printf("\n");

    // Allowing user to choose and add friends
    printf("Enter IDs of users from above whom you would like to add as friends (press any character to stop): ");

    int friendID, count = 0;

    while (scanf("%d", &friendID) > 0)
    {
        if (G->userList[friendID] == NULL || friendID > G->maxUserID || friendID < 0)
        {
            printf("\nUser with ID %d does not exist.\n", friendID);
            continue;
        }

        if (H_Search(G, userID, friendID))
        {
            printf("\n%s is already a friend of %s.\n", G->userList[friendID]->name, G->userList[userID]->name);
            continue;
        }

        int i;
        for (i = 0; i < K; i++)
            if (friendID == recFriends[i])
                break;

        if (i == K)
        {
            printf("\nUser with ID %d was not among those recommended.\n", friendID);
            continue;
        }

        H_Insert(G, userID, friendID);
        count++;
    }

    printf("\n%d users added as friends of %s (ID %d).\n\n", count, G->userList[userID]->name, userID);

    char dispose; // To take the random character entered to end input
    scanf("%c", &dispose);

    DeleteQueue(Q);
}

void NewFriends(Graph *G, int newUserID)
{
    int recFriends[12]; // To store recommended friends
    int recFC = 0;

    for (int i = 0; i < 12; i++)
        recFriends[i] = -1;

    if (G->numUsers <= 11)
    {
        printf("ID\t|Name\t\t\t|Age\t|City\t\t\t|School\t\t\t|Number of Friends\n");
        printf("------------------------------------------------------------------------------------------------------------------------\n");
        for (int i = 0; i < G->maxUserID; i++)
        {
            if (G->userList[i] != NULL && i != newUserID)
            {
                printf("%d\t", G->userList[i]->id);
                printf("|%s\t\t", G->userList[i]->name);
                if (strlen(G->userList[i]->name) < 7)
                    printf("\t");
                printf("|%d\t", G->userList[i]->age);
                printf("|%s\t\t", G->userList[i]->city);
                if (strlen(G->userList[i]->city) < 7)
                    printf("\t");
                printf("|%s\t\t", G->userList[i]->school);
                if (strlen(G->userList[i]->school) < 7)
                    printf("\t");
                printf("|%d friends\n", G->userList[i]->numFriends);

                recFriends[recFC] = i;
                ++recFC;
            }
        }
    }

    else
    {
        int age[G->numUsers], nA = 0;    // nA -> number of user IDs in age[]
        int city[G->numUsers], nC = 0;   // nC -> number of user IDs in city[]
        int school[G->numUsers], nS = 0; // nS -> number of user IDs in school[]

        IHT_Traverse(G, G->Age, age, &nA, G->userList[newUserID]->age);                //
        SHTCity_Traverse(G, G->City, city, &nC, G->userList[newUserID]->city);         //
        SHTSchool_Traverse(G, G->School, school, &nS, G->userList[newUserID]->school); //

        // Frequency table storing number of common parameters of each user with newUser
        short int *count = (short int *)calloc(G->numUsers, sizeof(short int));
        int cnt = 0; // Keeps track of users who have been recommended

        // Stores index-count pairs, i.e. ID-count pairs with topmost element having largest count
        // This will give us the users with most common parameters at each stage
        pair *maxHeap = (pair *)malloc(3 * (G->numUsers + 2) * sizeof(pair));
        int maxHeapSize = 0; // Keeps track of size of maxHeap

        for (int i = 0; i < nA; i++)
        {
            ++count[age[i]];
            if (i != newUserID)
                MH_Insert(maxHeap, &maxHeapSize, age[i], count[age[i]]); // Insert into maxHeap
        }
        for (int i = 0; i < nC; i++)
        {
            ++count[city[i]];
            if (i != newUserID)
                MH_Insert(maxHeap, &maxHeapSize, city[i], count[city[i]]); // Insert into maxHeap
        }
        for (int i = 0; i < nS; i++)
        {
            ++count[school[i]];
            if (i != newUserID)
                MH_Insert(maxHeap, &maxHeapSize, school[i], count[school[i]]); // Insert into maxHeap
        }

        printf("ID\t|Name\t\t\t|Age\t|City\t\t\t|School\t\t\t|Number of Friends\n");
        printf("------------------------------------------------------------------------------------------------------------------------\n");

        while (cnt < 10 && maxHeapSize > 0)
        {
            // Polling topmost index-count pair from maxHeap (the one with highest count)
            int maxIndex = maxHeap[0].index;
            MH_Delete(maxHeap, &maxHeapSize);

            // Checking if index has been included already or is equal to newUserID and printing
            if (count[maxIndex] > 0 && G->userList[maxIndex] != NULL && maxIndex != newUserID)
            {
                printf("%d\t", G->userList[maxIndex]->id);
                printf("|%s\t\t", G->userList[maxIndex]->name);
                if (strlen(G->userList[maxIndex]->name) < 7)
                    printf("\t");
                printf("|%d\t", G->userList[maxIndex]->age);
                printf("|%s\t\t", G->userList[maxIndex]->city);
                if (strlen(G->userList[maxIndex]->city) < 7)
                    printf("\t");
                printf("|%s\t\t", G->userList[maxIndex]->school);
                if (strlen(G->userList[maxIndex]->school) < 7)
                    printf("\t");
                printf("|%d friends\n", G->userList[maxIndex]->numFriends);

                recFriends[recFC] = maxIndex;
                ++recFC;

                ++cnt;
                count[maxIndex] = -1; //  Setting it to -1 to mark that it has been included
            }
        }

        int nU = G->numUsers;

        // For the case where less than 10 users have common parameters with new User
        for (int i = 0; i < nU / 2 && cnt < 10; i++)
        {
            // Checking from beginning of frequency table
            if (count[i] == 0 && G->userList[i] != NULL && i != newUserID)
            {
                printf("%d\t", G->userList[i]->id);
                printf("|%s\t\t", G->userList[i]->name);
                if (strlen(G->userList[i]->name) < 7)
                    printf("\t");
                printf("|%d\t", G->userList[i]->age);
                printf("|%s\t\t", G->userList[i]->city);
                if (strlen(G->userList[i]->city) < 7)
                    printf("\t");
                printf("|%s\t\t", G->userList[i]->school);
                if (strlen(G->userList[i]->school) < 7)
                    printf("\t");
                printf("|%d friends\n", G->userList[i]->numFriends);

                recFriends[recFC] = i;
                ++recFC;

                ++cnt;
                count[i] = -1; //  Setting it to -1 to mark that it has been included
            }

            if (cnt >= 10)
                break;

            // Checking from end of frequency table
            if (count[nU - 1 - i] == 0 && G->userList[i] != NULL && (nU - 1 - i) != newUserID)
            {
                printf("%d\t", G->userList[nU - 1 - i]->id);
                printf("|%s\t\t", G->userList[nU - 1 - i]->name);
                if (strlen(G->userList[nU - 1 - i]->name) < 7)
                    printf("\t");
                printf("|%d\t", G->userList[nU - 1 - i]->age);
                printf("|%s\t\t", G->userList[nU - 1 - i]->city);
                if (strlen(G->userList[nU - 1 - i]->city) < 7)
                    printf("\t");
                printf("|%s\t\t", G->userList[nU - 1 - i]->school);
                if (strlen(G->userList[nU - 1 - i]->school) < 7)
                    printf("\t");
                printf("|%d friends\n", G->userList[nU - 1 - i]->numFriends);

                recFriends[recFC] = nU - 1 - i;
                ++recFC;

                ++cnt;
                count[nU - 1 - i] = -1; //  Setting it to -1 to mark that it has been included
            }
        }

        free(count);
        free(maxHeap);
    }

    //----------------------------------------------------------------------------//
    // Allowing user to choose and add friends

    printf("\n");

    printf("Enter IDs of users from above whom you would like to add as friends (press any character to stop): ");

    int friendID, count = 0;

    while (scanf("%d", &friendID) > 0)
    {
        if (G->userList[friendID] == NULL || friendID > G->maxUserID || friendID < 0)
        {
            printf("\nUser with ID %d does not exist.\n", friendID);
            continue;
        }

        if (H_Search(G, newUserID, friendID))
        {
            printf("\n%s is already a friend of %s.\n", G->userList[friendID]->name, G->userList[newUserID]->name);
            continue;
        }

        int i;
        for (i = 0; i < 12; i++)
            if (friendID == recFriends[i])
                break;

        if (i == 12)
        {
            printf("\nUser with ID %d was not among those recommended.\n", friendID);
            continue;
        }

        // Inserting friendID into new user's friendList
        H_Insert(G, newUserID, friendID);
        count++;
    }

    printf("\n%d users added as friends of %s (ID %d).\n\n", count, G->userList[newUserID]->name, newUserID);

    char dispose; // To take the random character entered to end input
    scanf("%c", &dispose);
}

/*
 * Time delay function, taken from GeeksForGeeks
 * https://www.geeksforgeeks.org/time-delay-c/
*/
void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // Looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}
