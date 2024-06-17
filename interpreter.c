/**
 * @file interpreter.c
 * @brief A játékos által megadott parancs értelmezéséért felelős
 * 
 */
#include "debugmalloc.h"
#include "interpreter.h"
#include <stdlib.h>
#include "essentials.h"
#include <string.h>
#include "gameSpace.h"
#include <stdbool.h>

/**
 * @brief Visszaadja, hogy egy room vagy object jelenleg milyen állapotban van
 * 
 * @param id object id-ja
 * @param states Állapotokat tartalmazó tömb
 * @param stateCount Állapotokat tartalmazó tömb hossza
 * @return int Az object állapota
 */
int WhatState(int id, State states[], int stateCount) {
    for (int i = 0; i < stateCount; i++)
    {
        if (states[i].id == id) return states[i].state;
    }
    return -1;
}

/**
 * @brief Megmondja, hogy egy elem rendelkezik-e egy bizonyos eventel, ha igen akkor visszaadja, hogy hanyadik event, ha nem akkor -1-et ad vissza
 * 
 * @param l A tesztelendő elem event listája
 * @param string A tesztelendő event
 * @return int Az event sorszáma. -1 ha nem létezik
 */
int HasEvent(StringList *l, char *string) {
    for (int i = 0; i < l->len; i++)
    {
        StringList *split = Split(AtString(l, i), '/');
        int f = FindString(split, string);
        if (f != -1) {
            DestroyStringList(split);
            return f;
        }
        DestroyStringList(split);
    }
    return -1;
}

/**
 * @brief Értelmezi a játék által megadott parancsot. Ha szobát vált a játékos akkor megváltoztatja a jelenlegi szoba ID-ját, állapotokat átállít, játékos eszköztárat menedzsel.
 * 
 * @param currentRoomId Jelenlegi szoba azonosítója
 * @param command A játékos által megadott parancs
 * @param objects Objecteket tartalmazó tömb
 * @param objectCount Objecteket tartalmazó tömb elemszáma
 * @param rooms Szobákat tartalmazó tömb
 * @param roomCount Szobákat tartalmazó tömb elemszáma
 * @param states Állapotokat tartalmazó tömb
 * @param stateCount Állapotokat tartalmazó tömb elemszáma
 * @param playerInventory Játékos eszköztára
 * @return char* A parancs által kiváltott esemény szövege.
 */
char *Execute(int *currentRoomId, char *command, Object objects[], int objectCount, Room rooms[], int roomCount, State states[], int stateCount, List *playerInventory) {
    //translating command
    StringList *words = Split(command, ' ');
    char *dirtyWords[] = {"the", "a", "an", "my", "your"}; //Kréta reference
    Room currentRoom;
    int state = WhatState(*currentRoomId, states, stateCount);
    for (int i = 0; i < roomCount; i++) {
        if (rooms[i].id == *currentRoomId && rooms[i].state == state) {
            currentRoom = rooms[i];
            break;
        }
    }

    for (int i = 0; i < words->len; i++)
    {
        char *cmp = AtString(words, i);
        for (int j = 0; j < 3; j++)
        {
            if (strcmp(cmp, dirtyWords[j]) == 0) {
                RemoveAt(words, i--);
                break;
            }
        }
    }
    //general event
    if (strcmp(AtString(words, 0), "use") != 0 || words->len > 1){
        for (int i = 0; i < objectCount; i++) {
            int objectState = WhatState(objects[i].id, states, stateCount);
            if (objectState == objects[i].state) {
                StringNode *node = objects[i].modifyEvent->startNode;
                for (int j = 0; j < objects[i].modifyEvent->len; j++) {
                    StringList *s = Split(node->value, '/');
                    for (int k = 0; k < s->len; k++) {
                        bool xd = Contains(currentRoom.connectedItems, objects[i].id);
                        char *shortened = Merge(words, 0, words->len-1);
                        if(strcmp(AtString(s, k), shortened) == 0 && xd) {
                            ChangeStates(states, stateCount, AtString(objects[i].modify, j));
                            DestroyStringList(s);
                            DestroyStringList(words);
                            free(shortened);
                            return AtString(objects[i].modifyString, j);
                        }
                        free(shortened);
                    }
                    DestroyStringList(s);
                    node = node->nextNode;
                }
            }
        }
    }

    if (words->len >= 2) {
        //look around
        char *c = Merge(words, 0, 1);
        if (strcmp(c, "look around") == 0) {
            DestroyStringList(words);
            free(c);
            return currentRoom.lookAroundText;
        }
        free(c);
        //enter
        if (strcmp(AtString(words, 0), "enter") == 0) {
            char *location = Merge(words, 1, words->len-1);
            for (int i = 0; i < roomCount; i++)
            {
                if (strcmp(rooms[i].name, location) == 0 && Contains(currentRoom.connectedRooms, rooms[i].id)) {
                    *currentRoomId = rooms[i].id;
                    free(location);
                    DestroyStringList(words);
                    return "";
                }
            }
            free(location);
        }
        //use
        if (strcmp(AtString(words, 0), "use") == 0 && !ContainsString(words, "on")) {
            char *targetObject = Merge(words, 1, words->len-1);
            for (int i = 0; i < objectCount; i++)
            {
                int objectState = WhatState(objects[i].id, states, stateCount);
                if (objectState == objects[i].state) {
                    int at = HasEvent(objects[i].modifyEvent, "use");
                    if (strcmp(objects[i].name, targetObject) == 0 && at != -1 && Contains(currentRoom.connectedItems, objects[i].id)) {
                        free(targetObject);
                        DestroyStringList(words);
                        ChangeStates(states, stateCount, AtString(objects[i].modify, at));
                        return AtString(objects[i].modifyString, at);
                    }
                }
            }
            free(targetObject);
        }
    }

    if (words->len >= 3) {
        char *c = Merge(words, 0, 1);
        //go to
        if (strcmp(c, "go to") == 0) {
            char *location = Merge(words, 2, words->len-1);
            for (int i = 0; i < roomCount; i++)
            {
                if (strcmp(rooms[i].name, location) == 0 && Contains(currentRoom.connectedRooms, rooms[i].id)) {
                    *currentRoomId = rooms[i].id;
                    free(location);
                    free(c);
                    DestroyStringList(words);
                    return "";
                }
            }
            free(location);
        }

        //pick up
        if (strcmp(c, "pick up") == 0) {
            char *targetItem = Merge(words, 2, words->len-1);
            for (int i = 0; i < currentRoom.connectedItems->len; i++)
            {
                int obj = At(currentRoom.connectedItems, i);
                int objectState = WhatState(obj, states, stateCount);
                for (int j = 0; j < objectCount; j++)
                {
                    if (strcmp(objects[j].name, targetItem) == 0 && objects[j].id == obj && objects[j].state == objectState && objects[j].collectible) {
                        free(targetItem);
                        free(c);
                        DestroyStringList(words);
                        Add(playerInventory, objects[j].id);
                        RemoveAtInt(currentRoom.connectedItems, i);
                        return "You picked up the object.";
                    }
                }
                
            }
            free(targetItem);
        }

        //look at
        if (strcmp(c, "look at") == 0) {
            char *targetItem = Merge(words, 2, words->len-1);
            for (int i = 0; i < stateCount; i++)
            {
                if (!states[i].room) {
                    for (int j = 0; j < objectCount; j++)
                    {
                        if (strcmp(objects[j].name, targetItem) == 0 && objects[j].id == states[i].id 
                        && objects[j].state == states[i].state
                        && Contains(currentRoom.connectedItems, states[i].id)) {
                            free(targetItem);
                            free(c);
                            DestroyStringList(words);
                            return objects[j].lookAtText;
                        }
                    }
                    
                }
            }
            free(targetItem);
        }
        free(c);
    }

    if (words->len >=4) {
        //use object on object
        if (strcmp(AtString(words, 0), "use") == 0) { 
            int f = FindString(words, "on");//correct command
            if (f != -1 && f < words->len-1) {
                char *item1 = Merge(words, 1, f-1);
                char *item2 = Merge(words, f+1, words->len-1);
                for (int i = 0; i < playerInventory->len; i++)
                {
                    int obj = At(playerInventory, i);
                    int objectState = WhatState(obj, states, stateCount);
                    for (int j = 0; j < objectCount; j++)
                    {
                        if (objects[j].id == obj && strcmp(objects[j].name, item1) == 0 && objects[j].state == objectState) { //in player inventory
                            //looking through events
                            char event[50];
                            sprintf(event, "%d", objects[j].id);
                            char ev[50] = "use ";
                            strcat(ev, event); //ev -> item in player inventory, item2 contains this string
                            for (int k = 0; k < currentRoom.connectedItems->len; k++)
                            {
                                int o = At(currentRoom.connectedItems, k);
                                int s = WhatState(*currentRoomId, states, stateCount);
                                for (int l = 0; l < objectCount; l++)
                                { //in current room - correct state - name checks out - has use event
                                    if (objects[l].id == o && objects[l].state == state) { //connected
                                        for (int z = 0; z < objects[l].modifyEvent->len; z++)
                                        {
                                            StringList *list = Split(AtString(objects[l].modifyEvent, z), '/');
                                            int at = FindString(list, ev);
                                            if (at != -1) {
                                                free(item1);
                                                free(item2);
                                                DestroyStringList(list);
                                                DestroyStringList(words);
                                                ChangeStates(states, stateCount, AtString(objects[l].modify, z)); //if something breaks, then here z should be at
                                                
                                                return AtString(objects[l].modifyString, z);
                                            }
                                            DestroyStringList(list);
                                        }
                                        
                                    } 
                                }
                                
                            }
                            
                        }
                    }
                    
                }
                free(item1);
                free(item2);
            }
        }
    }

    DestroyStringList(words);
    return NULL;
}
