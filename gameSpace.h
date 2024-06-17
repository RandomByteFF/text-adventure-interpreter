#ifndef GAMESPACE_H
#define GAMESPACE_H
#include "essentials.h"
#include <stdbool.h>

/**
 * @brief Szobát definiáló struct
 * 
 */
typedef struct Room
{
    int id;
    int state;
    char *name;
    char *lookAroundText;
    char *entryText;
    List *connectedItems;
    List *connectedRooms;
} Room;

/**
 * @brief Objectet definiáló struct
 * 
 */
typedef struct Object {
    int id;
    int state;
    char *name;
    char *lookAtText;
    bool collectible;
    StringList *modifyEvent;
    StringList *modify;
    StringList *modifyString;
} Object;

/**
 * @brief Állapotot definiáló struct
 * 
 */
typedef struct State {
    bool room; //A false azt jelenti, hogy object
    int id;
    int state;
} State;

void ChangeStates(State states[], int stateCount, char *list);
#endif