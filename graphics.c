/**
 * @file graphics.c
 * @brief Képernyőn lévő megjelenítésekért felelős
 * 
 * 
 */
#include "graphics.h"
#include "debugmalloc.h"
#include <stdio.h>
#include "econio.h"
#include <stdbool.h>
#include "gameSpace.h"
#include "essentials.h"
#include "interpreter.h"
#include "loaderSystem.h"

const int WIDTH = 120;
const int HEIGHT = 30;

/**
 * @brief Egy menü element definiál
 * 
 */
typedef struct MenuItem {
    int x, y;
    char *text;
} MenuItem;

MenuItem items[4];

Room *rooms;
Object *objects;
State *states;

int roomCount;
int objectCount;
int stateCount;

int previousRoomId;
int currentRoomId;
int previousRoomState;
int currentRoomState;

/**
 * @brief Szöveg kirajzolása a képernyőre egy adott pozícióban
 * 
 * @param x X pozíció
 * @param y Y pozíció
 * @param text szöveg
 * @param selected kiválasztott elem sorszáma
 */
void DrawMenuItem(int x, int y, char *text, bool selected) {
    econio_gotoxy(x, y);
    if (selected) {
        econio_textbackground(COL_WHITE);
        econio_textcolor(COL_BLACK);
    }
    else {
        econio_textbackground(COL_BLACK);
        econio_textcolor(COL_WHITE);
    }
    printf(text);
    econio_textbackground(COL_BLACK);
    econio_textcolor(COL_WHITE);
}

/**
 * @brief Összes menü elem kirajzolása, a selected elem kijelölésével
 * 
 * @param selected A kiválasztott elem sorszáma
 */
void DrawMenu(int selected) {
    econio_clrscr();
    for (int i = 0; i < 4; i++)
    {
        DrawMenuItem(items[i].x, items[i].y, items[i].text, i == selected);
    }
}

/**
 * @brief Játéktér kirajzolása
 * 
 */
void DrawGameSpace() {
    econio_clrscr();
    econio_gotoxy(0, 20);
    for (int i = 0; i < WIDTH; i++)
    {
        printf("█");
    }
    econio_gotoxy(0, 29);
    for (int i = 0; i < WIDTH; i++)
    {
        printf("█");
    }
}

/**
 * @brief Játék elindítása
 * 
 * @param playerInventory A játékos eszköztára
 */
void PlayGame(List *playerInventory) {
    DrawGameSpace();
    econio_gotoxy(0,1);
    for (int i = 0; i < roomCount; i++)
    {
        if (rooms[i].id == currentRoomId && rooms[i].state == currentRoomState) {
            printf(rooms[i].entryText);
        }
    }
    econio_gotoxy(0, 30);

    char *command = SortBeolvasStd();
    while (strcmp(command, "save game") != 0) {
        char *s = Execute(&currentRoomId, command, objects, objectCount, rooms, roomCount, states, stateCount, playerInventory);
        int currentRoomState = WhatState(currentRoomId, states, stateCount);
        
        DrawGameSpace();
        econio_gotoxy(0,1);
        for (int i = 0; i < roomCount; i++)
        {
            if (rooms[i].id == currentRoomId && rooms[i].state == currentRoomState) {
                printf(rooms[i].entryText);
            }
        }
        econio_gotoxy(0, 30);

        if (s == NULL) {
            econio_gotoxy(0,21);
            printf("Sorry, I didn't understand that\n");
            econio_gotoxy(0,30);
        }
        else {
            econio_gotoxy(0,21);
            printf("%s", s);
            econio_gotoxy(0,30);
        }
        free(command);
        command = SortBeolvasStd();
        previousRoomId = currentRoomId;
        previousRoomState = currentRoomState;
    }

    //Játék mentése
    FILE* fp;
    fp = fopen("saveData.dat", "w");
    if (fp == NULL) exit(2);
    fprintf(fp, "%d\n", currentRoomId);
    for (int i = 0; i < playerInventory->len-1; i++)
    {
        fprintf(fp, "%d,", At(playerInventory, i));
    }
    fprintf(fp, "%d\n", At(playerInventory, playerInventory->len-1));
    
    for (int i = 0; i < stateCount; i++)
    {
        fprintf(fp, "%d:%d:%s\n", states[i].id, states[i].state, states[i].room ? "1" : "0");
    }
    fclose(fp);
    

    free(command);
    DestroyRooms(rooms, roomCount);
    DestroyObjects(objects, objectCount);
    DestroyList(playerInventory);
    free(rooms);
    free(objects);
    free(states);
}

/**
 * @brief Új játék előkészítése
 * 
 */
void NewGame() {
    List *playerInventory = CreateList(0);
    LoadStry(&rooms, &objects, &roomCount, &objectCount, &states, &stateCount);

    previousRoomId = 1;
    currentRoomId = 1;
    previousRoomState = WhatState(currentRoomId, states, stateCount);
    currentRoomState = previousRoomState;

    PlayGame(playerInventory);
}

/**
 * @brief Előző játék betöltése
 * 
 */
void LoadGame() {
    FILE* fp;
    fp = fopen("saveData.dat", "r");
    if (fp == NULL) { 
        NewGame();
        return;
    }
    char *sor = SortBeolvas(fp);
    currentRoomId = ConvertToInt(sor);
    previousRoomId = currentRoomId;

    free(sor);
    sor = SortBeolvas(fp);
    List *playerInventory = SplitInt(sor, ',');
    free(sor);
    LoadStry(&rooms, &objects, &roomCount, &objectCount, &states, &stateCount);
    for (int i = 0; i < stateCount; i++)
    {
        sor = SortBeolvas(fp);
        List *l = SplitInt(sor, ':');
        states[i].state = At(l, 1);
        DestroyList(l);
        free(sor);
    }
    previousRoomState = WhatState(currentRoomId, states, stateCount);
    currentRoomState = previousRoomState;
    fclose(fp);
    for (int i = 0; i < roomCount; i++)
    {
        for (int j = 0; j < playerInventory->len; j++)
        {
            int at = Find(rooms[i].connectedItems, At(playerInventory, j));
            if (at != -1) {
                RemoveAtInt(rooms[i].connectedItems, at);
            }
        }
    }
    
    PlayGame(playerInventory);
}

/**
 * @brief How to play menüpont kezelése
 * 
 */
void HowToPlay() {
    FILE *fp;
    fp = fopen("howToPlay.txt", "r");
    if (fp == NULL) exit(2);
    char *sor = SortBeolvas(fp);
    econio_clrscr();
    econio_gotoxy(0,0);
    printf("%s", sor);
    free(sor);
    econio_rawmode();
    econio_getch();
    econio_normalmode();
}

/**
 * @brief Kezeli a program menü módját
 * 
 */
void MenuMode() {
    items[0] = (MenuItem){56, 5, "New game"};
    items[1] = (MenuItem){56, 10, "Load game"};
    items[2] = (MenuItem){55, 15, "How to play"};
    items[3] = (MenuItem){56, 20, "Exit game"};
    while (true) {
        econio_rawmode();
        int key = 0;
        int selected = 0;
        do {
            switch (key)
            {
            case KEY_DOWN:
                selected++;
                break;

            case KEY_UP:
                selected--;
                break;
            
            default:
                break;
            }

            if (selected < 0) selected = 0;
            else if (selected > 3) selected = 3;
            DrawMenu(selected);
            key = econio_getch();
        } while(key != KEY_ENTER);
        econio_normalmode();
        switch (selected)
        {
        case 0:
            NewGame();
            break;
        case 1:
            LoadGame();
            break;
        case 2:
            HowToPlay();
            break;
        case 3:
            return;
        default:
            break;
        }
    }
}