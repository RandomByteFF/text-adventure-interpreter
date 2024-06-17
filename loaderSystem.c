/**
 * @file loaderSystem.c
 * @brief Felelős az értelmezendő szövegek betöltéséért
 * 
 */
#include "debugmalloc.h"
#include "essentials.h"
#include <stdio.h>
#include <stdlib.h>
#include "essentials.h"
#include <string.h>
#include <stdbool.h>
#include "gameSpace.h"

/**
 * @brief Beolvas egy fájlt és egy listába tördeli szét sorok szerint. A hívó felelőssége meghívni a DestroyStringList() a listára. 
 * 
 * @param file Fájl
 * @return StringList* Beolvasott sorok
 */
StringList *ReadFile(char *file) {
    FILE* fp;
    fp = fopen(file, "r");
    if (fp == NULL) exit(2);
    StringList *list = CreateStringList("");
    char *line = SortBeolvas(fp);

    while (strlen(line) > 1){
        AddStringPointer(list, line);
        line = SortBeolvas(fp);
    }
    free(line);
    RemoveFirstString(list);

    fclose(fp);
    return list;
}

/**
 * @brief Felszabadítja az összes room objectet
 */
void DestroyRooms(Room *room, int roomCount) {
    for (int i = 0; i < roomCount; i++)
    {
        free(room[i].name);
        free(room[i].lookAroundText);
        free(room[i].entryText);
        DestroyList(room[i].connectedItems);
        DestroyList(room[i].connectedRooms);
    }
}

/**
 * @brief felszabadítja az összes object objectet
 */
void DestroyObjects(Object *object, int objectCount) {
    for (int i = 0; i < objectCount; i++)
    {
        free(object[i].name);
        free(object[i].lookAtText);
        DestroyStringList(object[i].modifyEvent);
        DestroyStringList(object[i].modify);
        DestroyStringList(object[i].modifyString);
    }
}

/**
 * @brief Betölti a rooms.stry-t és az objects.stry-t. A hívó köteles meghívni mint a DestroyObjects és a DestroyRooms függvényeket
 * 
 * @param rooms 
 * @param objects 
 * @param roomCount 
 * @param objectCount 
 * @param states 
 * @param elementCount 
 */
void LoadStry(Room **rooms, Object **objects, int *roomCount, int *objectCount, State *states[], int *elementCount) {
    StringList *roomIn = ReadFile("rooms.stry");
    StringList *objectIn = ReadFile("objects.stry");
    *roomCount = roomIn->len;
    *objectCount = objectIn->len;
    
    Room *r = malloc(sizeof(Room) * roomIn->len);
    if (r == NULL) exit(1);
    Object *o = malloc(sizeof(Object) * objectIn->len);
    if (o == NULL) exit(1);

    for (int i = 0; i < roomIn->len; i++)
    {
        char *proc = AtString(roomIn, i);
        StringList *split = Split(proc, ';');
        r[i].id = ConvertToInt(AtString(split, 0));
        r[i].state = ConvertToInt(AtString(split, 1));
        r[i].name = CpyStrPtr(AtString(split, 2));
        r[i].lookAroundText = CpyStrPtr(AtString(split, 3));
        r[i].entryText = CpyStrPtr(AtString(split, 4));
        r[i].connectedItems = SplitInt(AtString(split, 5), ',');
        r[i].connectedRooms = SplitInt(AtString(split, 6), ',');
        DestroyStringList(split);
    }
    
    for (int i = 0; i < objectIn->len; i++)
    {
        char *proc = AtString(objectIn, i);
        StringList *split = Split(proc, ';');
        o[i].id = ConvertToInt(AtString(split, 0));
        o[i].state = ConvertToInt(AtString(split, 1));
        o[i].name = CpyStrPtr(AtString(split, 2));
        o[i].lookAtText = CpyStrPtr(AtString(split, 3));
        o[i].collectible = strcmp(AtString(split, 4), "true") == 0 ? true : false;
        o[i].modifyEvent = Split(AtString(split, 5), ',');
        o[i].modify = Split(AtString(split, 6),',');
        o[i].modifyString = Split(AtString(split, 7),'/');
        DestroyStringList(split);
    }
    
    *rooms = r;
    *objects = o;

    DestroyStringList(roomIn);
    DestroyStringList(objectIn);

    *elementCount = 1;
    *states = malloc(sizeof(State));
    if (states == NULL) exit(1);
    (*states)[0].room = true;
    (*states)[0].id = r[0].id;
    (*states)[0].state = r[0].state;

    for (int i = 0; i < *roomCount; i++)
    {
        if ((*states)[*elementCount-1].id != r[i].id) {
            (*elementCount)++;
            *states = realloc(*states, sizeof(State) * (*elementCount)); //TOFIX
            if (states == NULL) exit(1);
            (*states)[*elementCount-1].room = true;
            (*states)[*elementCount-1].id = r[i].id;
            (*states)[*elementCount-1].state = r[i].state;
        }
    }

    for (int i = 0; i < *objectCount; i++)
    {
        if ((*states)[*elementCount-1].id != o[i].id) {
            (*elementCount)++;
            *states = realloc(*states, sizeof(State) * (*elementCount)); //TOFIX
            if (states == NULL) exit(1);
            (*states)[*elementCount-1].room = false;
            (*states)[*elementCount-1].id = o[i].id;
            (*states)[*elementCount-1].state = o[i].state;
        }
    }
    
}