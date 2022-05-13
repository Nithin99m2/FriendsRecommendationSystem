#ifndef HASH_H
#define HASH_H

#include "queue.h"
#include "frs.h"

void H_Insert(Graph *G, int userID, int friendID);
void H_Delete_With_Text(Graph *G, int userID, int friendID);    // Prints messages when deleting friends
void H_Delete_Without_Text(Graph *G, int userID, int friendID); // Does not print any message while deleting friends
bool H_Search(Graph *G, int userID, int friendID);
void H_Rehash(Graph *G, int userID);

#endif