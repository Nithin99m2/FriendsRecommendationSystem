#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "frs.h"
#include <stdio.h>

void PrintTitle();
void PrintMenu();
void PrintLoginMenu();
void Login(Graph *G, int *currentID);
void Signup(Graph *G, int *currentID);

#endif