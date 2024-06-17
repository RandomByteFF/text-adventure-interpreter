
/**
 * @file essentials.c
 * @brief Az összes máshová nem illő de szükséges függvények. Nagyrészt lista kezelő függvények
 * 
 */
#include "debugmalloc.h"
#include "essentials.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief Létrehoz egy int listát egy kezdőértékkel. A hívó felelőssége meghívni a listára a DestroyList() függvényt.
 * 
 * @param startValue Kezdőérték
 * @return List* Létrehozott lista
 */
List *CreateList(int startValue) {
    Node *m = malloc(sizeof(Node));
    if (m == NULL) exit(1);
    m->nextNode = NULL;
    m->value = startValue;

    List *ret = malloc(sizeof(List));
    if (ret == NULL) exit(1);
    ret->len = 1;
    ret->startNode = m;
    return ret;
}

/**
 * @brief Létrehoz egy String listát egy kezdőértékkel. A hívó felelőssége meghívni a listára a DestroyStringList() függvényt
 * 
 * @param start kezdő érték
 * @return StringList* Létrehozott lista
 */
StringList *CreateStringList(char *start) {
    int len = strlen(start);
    StringNode *m = malloc(sizeof(StringNode));
    if (m == NULL) exit(1);
    m->value = malloc(sizeof(char) * (len+1));
    if (m->value == NULL) exit(1); 
    m->nextNode = NULL;
    strcpy(m->value, start);

    StringList *ret = malloc(sizeof(StringList));
    if (ret == NULL) exit(1);
    ret->len = 1;
    ret->startNode = m;
    return ret;
}

/**
 * @brief Felszabadítja a teljes int listát
 * 
 * @param list Felszabadítandó lista
 */
void DestroyList(List *list) {
    Node *node = list->startNode;
    while (node != NULL)
    {
        Node *temp = node;
        node = node->nextNode;
        free(temp);
    }
    free(list);
}

/**
 * @brief Felszabadítja a teljes String listát
 * 
 * @param list Felszabadítandó lista
 */
void DestroyStringList(StringList *list) {
    StringNode *node = list->startNode;
    while (node != NULL)
    {
        StringNode *temp = node;
        node = node->nextNode;
        free(temp->value);
        free(temp);
    }
    free(list);
}

/**
 * @brief Hozzáad egy elemet egy int listához
 * 
 * @param list Lista
 * @param value Érték amit hozzáad a listához
 */
void Add(List *list, int value) {
    Node *node = list -> startNode;
    while (node->nextNode != NULL) {
        node = node->nextNode;
    }
    node->nextNode = malloc(sizeof(Node));
    if (node->nextNode == NULL) exit(1);
    node = node->nextNode;
    node->value = value;
    node->nextNode = NULL;
    list->len++;
}

/**
 * @brief Hozzáad egy stringet elemet egy string listához
 * 
 * @param list Lista
 * @param value Érték amit hozzáad a listához
 */
void AddString(StringList *list, char *item) {
    StringNode *node = list -> startNode;
    while (node->nextNode != NULL) {
        node = node->nextNode;
    }
    node->nextNode = malloc(sizeof(StringNode));
    if (node->nextNode == NULL) exit(1);
    node = node->nextNode;
    node->value = malloc(sizeof(char) * (strlen(item)+1));
    if (node->value == NULL) exit(1);
    strcpy(node->value, item);
    node->nextNode = NULL;
    list->len++;
}

/**
 * @brief Hozzáfűz egy String listához egy elemet aminek már le van foglalva a memória (csupán a kód gyorsításáért kell)
 * 
 * @param list Lista
 * @param item Pointer amit hozzá fog fűzni a listához
 */
void AddStringPointer(StringList *list, char *item) {
    StringNode *node = list -> startNode;
    while (node->nextNode != NULL) {
        node = node->nextNode;
    }
    node->nextNode = malloc(sizeof(StringNode));
    if (node->nextNode == NULL) exit(1);
    node = node->nextNode;
    node->value = item;
    node->nextNode = NULL;
    list->len++;
}

/**
 * @brief Visszaadja a tömb bizonyos indexénél található értéket
 * 
 * @param list Lista
 * @param index index
 * @return int Indexen található érték
 */
int At(List *list, int index) {
    Node *node = list->startNode;
    int i = 0;
    while (node != NULL) {
        if (i++ == index) return node->value;
        node = node->nextNode;
    }
    exit(3);
}

/**
 * @brief Visszaadja a tömb bizonyos indexénél található értéket
 * 
 * @param list Lista
 * @param index index
 * @return char* Indexen található érték
 */
char *AtString(StringList *list, int index) {
    StringNode *node = list->startNode;
    int i = 0;
    while (node != NULL) {
        if (i++ == index) return node->value;
        node = node->nextNode;
    }
    exit(3);
}

/**
 * @brief Beállítja a tömb bizonyos indexénél található értéket
 * 
 * @param list Lista
 * @param index Index
 * @param value Beállítandó érték
 */
void Set(List *list, int index, int value) {
    Node *node = list->startNode;
    int i = 0;
    while (node != NULL) {
        if (i++ == index) {
            node->value = value;
            return;
        }
        node = node->nextNode;
    }
    exit(3);
}

/**
 * @brief Beállítja a tömb bizonyos indexénél található értéket
 * 
 * @param list Lista
 * @param index Index
 * @param value Beállítandó érték
 */
void SetString(StringList *list, int index, char *value) {
    StringNode *node = list->startNode;
    int i = 0;
    while (node != NULL) {
        if (i++ == index) {
            node->value = realloc(node->value, sizeof(char) * (strlen(value)+1));
            if (node->value == NULL) exit(1);
            strcpy(node->value, value);
            return;
        }
        node = node->nextNode;
    }
    exit(3);
}

/**
 * @brief Kitörli a lista első elemét
 * 
 * @param list Lista
 */
void RemoveFirst(List *list) {
    list->len -= 1;
    Node *tempNode = list->startNode;
    tempNode = tempNode->nextNode;
    free(list->startNode);
    list->startNode = tempNode;
}

/**
 * @brief Kitörli a lista első elemét
 * 
 * @param list Lista
 */
void RemoveFirstString(StringList *list) {
    list->len -= 1;
    StringNode *tempNode = list->startNode;
    tempNode = tempNode->nextNode;
    free(list->startNode->value);
    free(list->startNode);
    list->startNode = tempNode;
}

/**
 * @brief Kitörli a lista megadott elemét
 * 
 * @param list Lista
 * @param index Index
 */
void RemoveAt(StringList *list, int index){
    list->len -= 1;
    StringNode *node = list->startNode;
    if (index == 0) {
        list->startNode = node->nextNode;
        free(node->value);
        free(node);
        return;
    }

    for (int i = 0; i < index-1; i++)
    {
        node = node ->nextNode;
    }

    StringNode *temp = node->nextNode;
    node->nextNode = temp->nextNode;
    free(temp->value);
    free(temp);
}

/**
 * @brief Kitörli a lista megadott elemét
 * 
 * @param list Lista
 * @param index Index
 */
void RemoveAtInt(List *list, int index) {
    list->len -= 1;
    Node *node = list->startNode;
    if (index == 0) {
        list->startNode = node->nextNode;
        free(node);
        return;
    }

    for (int i = 0; i < index-1; i++)
    {
        node = node ->nextNode;
    }

    Node *temp = node->nextNode;
    node->nextNode = temp->nextNode;
    free(temp);
}

/**
 * @brief Megkeresi, hogy az érték hanyadik indexen található meg először. -1 ha az érték nem található
 * 
 * @param list Lista
 * @param value Keresendő érték
 * @return int index
 */
int Find(List *list, int value) {
    Node *node = list->startNode;
    int i = 0;
    while (node != NULL) {
        if (node->value == value) {
            return i;
        }
        i++;
        node = node->nextNode;
    }
    return -1;
}

/**
 * @brief Megkeresi, hogy az érték hanyadik indexen található meg először. -1 ha az érték nem található
 * 
 * @param list Lista
 * @param value Keresendő érték
 * @return int index
 */
int FindString(StringList *list, char* value) {
    StringNode *node = list->startNode;
    int i = 0;
    while (node != NULL) {
        if (strcmp(node->value, value) == 0) {
            return i;
        }
        i++;
        node = node->nextNode;
    }
    return -1;
}

/**
 * @brief Megnézi, hogy tartalmazza-e a lista az elemet
 * 
 * @param list Lista
 * @param value Érték
 * @return true Az elem benne van a listában
 * @return false Az elem nincs benne a listában
 */
bool Contains(List *list, int value) {
    return Find(list, value) != -1;
}


/**
 * @brief Megnézi, hogy tartalmazza-e a lista az elemet
 * 
 * @param list Lista
 * @param value Érték
 * @return true Az elem benne van a listában
 * @return false Az elem nincs benne a listában
 */
bool ContainsString(StringList *list, void *value) {
    return FindString(list, value) != -1;
}

/**
 * @brief Az elválasztó karakter alapján több elemre szed szét egy stringet. A hívó felelőssége meghívni a DestroyStringList()-et
 * 
 * @param string Felbontandó string
 * @param split Karakter ami alapján felbontja
 * @return StringList* Felbontott string lista
 */
StringList *Split(char *string, char split) {
    StringList *list = CreateStringList("");
    int i = 0;
    char *current = malloc(1);
    if (current == NULL) exit(1);
    while (string[i] != split && string[i] != '\0')
    {
        current = realloc(current, ++i+1);
        if (current == NULL) exit(1);
        current[i-1] = string[i-1];
    }
    current[i] = '\0';
    SetString(list, 0, current);
    while (string[i] != '\0')
    {
        int j = 0;
        current = realloc(current, 1);
        if (current == NULL) exit(1);
        while (string[++i] != split && string[i] != '\0') {
            current = realloc(current, ++j+1);
            if (current == NULL) exit(1);
            current[j-1] = string[i];
        }
        current[j] = '\0';
        AddString(list, current);
    }
    free(current);
    return list;
}

/**
 * @brief Egy stringben megadott integereket szétszed az elválasztó karakter alapján. A hívó felelőssée meghívni a DestroyList()-et
 * 
 * @param string Felbontandó string
 * @param split Karakter ami alapján felbontja
 * @return List* Felbontott int lista
 */
List *SplitInt(char *string, char split) {
    List *list = CreateList(0);
    int i = 0;
    char *current = malloc(1);
    while (string[i] != split && string[i] != '\0')
    {
        current = realloc(current, ++i+1);
        current[i-1] = string[i-1];
    }
    current[i] = '\0';
    int kezd = ConvertToInt(current);
    Set(list, 0, kezd);
    while (string[i] != '\0')
    {
        int j = 0;
        current = realloc(current, 1);
        while (string[++i] != split && string[i] != '\0') {
            current = realloc(current, ++j+1);
            current[j-1] = string[i];
        }
        current[j] = '\0';
        Add(list, ConvertToInt(current));
    }
    free(current);
    return list;
}

/**
 * @brief SortBeolvas segédfüggvénye
 */
char *Alloc(int n, FILE *fp) {
    char c;
    if (fscanf(fp, "%c", &c) == 1 && c != '\n') {
        char *ret = Alloc(n+1, fp);
        ret[n] = c;
        return ret;
    }
    char *alloc = malloc((n+1)*sizeof(char));
    if (alloc == NULL) exit(1);
    alloc[n] = '\0';
    return alloc;
}

/**
 * @brief SortBeolvasStd segédfüggvénye
 */
char *AllocStd(int n) {
    char c;
    if (scanf("%c", &c) == 1 && c != '\n') {
        char *ret = AllocStd(n+1);
        ret[n] = c;
        return ret;
    }
    char *alloc = malloc((n+1)*sizeof(char));
    if (alloc == NULL) exit(1);
    alloc[n] = '\0';
    return alloc;
}

/**
 * @brief A függvény beolvas egy teljes sort (enterig vagy fájl vége jelig) a megadott
 * fájlból, és visszaadja egy dinamikusan foglalt sztringben. A sztring
 * nullával van lezárva, az entert nem tartalmazza. A hívó felelőssége
 * a free()-t meghívni a kapott pointerre.
 * 
 * @param fp Fájl pointer
 * @return char* Beolvasott string
 */
char *SortBeolvas(FILE *fp) {
    return Alloc(0, fp);
}

/**
 * @brief A függvény beolvas egy teljes sort (enterig vagy fájl vége jelig)
 * , és visszaadja egy dinamikusan foglalt sztringben. A sztring
 * nullával van lezárva, az entert nem tartalmazza. A hívó felelőssége
 * a free()-t meghívni a kapott pointerre.
 * 
 * @return char* Beolvasott string
 */
char *SortBeolvasStd() {
    return AllocStd(0);
}

/**
 * @brief Egy stringet int-é alakít
 * 
 * @param str Átalakítandó string
 * @return int Átalakított int
 */
int ConvertToInt(char *str) {
    int x;
    sscanf(str, "%d", &x);
    return x;
    return 0;
}

/**
 * @brief Készít egy másolatot egy stringből. A hívó felelőssége meghívni a free-t
 * 
 * @param str Másolandó string
 * @return char* Másolt string
 */
char *CpyStrPtr(char *str) {
    char *x = malloc((strlen(str)+1) * sizeof(char));
    if (x == NULL) exit(1);
    strcpy(x, str);
    return x;
}

/**
 * @brief Lista elemeit összeolvasztja egy stringbe, whitespace karakterrel elválasztva. A hívó felelőssége meghívni a free-t
 * 
 * @param list Lista
 * @param start Kezdő index (inclusive)
 * @param end Végső index (inclusive)
 * @return char* Összeolvasztott string
 */
char *Merge(StringList *list, int start, int end) {
    StringNode *node = list->startNode;
    for (int i = 0; i < start; i++)
    {
        node = node->nextNode;
    }
    int len = 0;
    StringNode *startNode = node;
    for (int i = 0; i < end-start+1; i++)
    {
        len += strlen(node->value);
        node = node->nextNode;
    }
    char *ret = malloc((len+(end-start)+1) * sizeof(char));
    if (ret == NULL) exit(1);
    int copied = 0;
    for (int i = 0; i < end-start+1; i++)
    {
        strcpy(ret + copied, startNode->value);
        copied += strlen(startNode->value);
        ret[copied++] = ' ';
        startNode = startNode->nextNode;
    }
    ret[copied-1] = '\0';
    return ret;
}
