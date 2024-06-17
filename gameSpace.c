/**
 * @file gameSpace.c
 * @brief Játék állapotát kezeli és a játéktérben elhelyezkedő elemeket definiálja
 * 
 */
#include "gameSpace.h"
#include "essentials.h"
#include <string.h>

/**
 * @brief Megváltoztatja egy event string alapján az object/room állapotát
 * 
 * @param states Állapotok tömb
 * @param stateCount Állapotok tömb elemszáma
 * @param list Event lista
 */
void ChangeStates(State states[], int stateCount, char *list) {
    StringList *s = Split(list, '-');
    for (int i = 0; i < s->len; i++)
    {
        int id;
        int state;

        sscanf(AtString(s, i), "%d:%d", &id, &state);
        int j = 0;
        while (states[j].id != id) {
            j++;
        }
        states[j].state = state;
    }
    DestroyStringList(s);
}
