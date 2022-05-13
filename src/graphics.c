#include "graphics.h"
#include <stdbool.h>
#include <string.h>

// Prints the title of the social network
void PrintTitle()
{
    printf("\n");
    printf("        ________________________\n");
    printf("       |                        |\n");
    printf("       |        SPACEBOOK       |\n");
    printf("       |________________________|\n\n");
}

// Prints a neat menu presenting options to the currently logged in user
void PrintMenu()
{
    printf(" ________________________________________\n");
    printf("|                                        |\n");
    printf("|  1. Add Friend                         |\n");
    printf("|  2. Unfriend                           |\n");
    printf("|  3. Check Friend                       |\n");
    printf("|  4. Recommend Friends                  |\n");
    printf("|  5. Log Out                            |\n");
    printf("|  6. Delete Account                     |\n");
    printf("|  7. Exit                               |\n");
    printf("|________________________________________|\n\n");
}

// Prints a neat login menu
void PrintLoginMenu()
{
    printf(" ________________________________________\n");
    printf("|                                        |\n");
    printf("|  1. Sign Up                            |\n");
    printf("|  2. Log In                             |\n");
    printf("|  3. Exit                               |\n");
    printf("|________________________________________|\n\n");
}

// Prompts user to enter name and ID to login
void Login(Graph *G, int *currentID)
{
    char name[20];

    // Looping until the user enters a valid ID and the username matching it
    while (true)
    {
        // Entering name and ID of user
        printf("Enter name: ");
        scanf(" %[^\n]", name);
        printf("Enter password (ID): ");
        scanf("%d", currentID);

        // Tocheck if user is present in graph
        if (G->userList[*currentID] != NULL && *currentID <= G->maxUserID && *currentID >= 0)
        {
            // To check if name matches ID
            if (strcmp(name, G->userList[*currentID]->name)== 0)
            {
                printf("\nSuccessfully logged in as %s (%d)!\n\n", name, *currentID);
                break;
            }
        }

        printf("\nUnsuccessful login. Please enter the correct credentials.\n\n");
        delay(2000);
        system("clear");
    }
}

// Prompts the user to enter details to signup
void Signup(Graph *G, int *currentID)
{
    printf("\nWelcome to Spacebook!\n\n");

    // Updating currentID to that of the added user
    if (G->heapSize == 0)
        *currentID = G->maxUserID + 1; 
    else
        *currentID = G->minHeap[0]; 

    AddUser(G);
}
