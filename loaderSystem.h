#ifndef LOADERSYSTEM_H
#define LOADERSYSTEM_H
#include "gameSpace.h"

void DestroyRooms(Room *room, int roomCount);
void DestroyObjects(Object *object, int objectCount);
void LoadStry(Room **rooms, Object **objects, int *roomCount, int *objectCount, State *states[], int *elementCount);

#endif