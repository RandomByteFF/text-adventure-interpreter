#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <stdbool.h>
#include "gameSpace.h"
#include "essentials.h"

char *Execute(int *currentRoomId, char *command, Object objects[], int objectCount, Room rooms[], int roomCount, State states[], int stateCount, List *playerInventory);
int WhatState(int id, State states[], int stateCount);

#endif