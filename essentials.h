#ifndef ESSENTIALS_H
#define ESSENTIALS_H
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Egy lista elem
 * 
 */
typedef struct Node {
    struct Node *nextNode;
    int value;
} Node;

/**
 * @brief Int lista
 * 
 */
typedef struct List
{
    Node *startNode; //Ha minden elromlana akkor ezt írd vissza void *-ra
    int len;
} List;

/**
 * @brief String lista elem
 * 
 */
typedef struct StringNode {
    struct StringNode *nextNode;
    char *value;
} StringNode;

/**
 * @brief String lista
 * 
 */
typedef struct StringList
{
    StringNode *startNode;
    int len;
} StringList;

List *CreateList(int startValue);
StringList *CreateStringList(char *start);

void DestroyList(List *list);
void DestroyStringList(StringList *list);

void Add(List *list, int value);
void AddString(StringList *list, char *item);
void AddStringPointer(StringList *list, char *item);

int At(List *list, int index);
char *AtString(StringList *list, int index);

void Set(List *list, int index, int value);
void SetString(StringList *list, int index, char *value);

void RemoveFirst(List *list);
void RemoveFirstString(StringList *list);
void RemoveAt(StringList *list, int index);
void RemoveAtInt(List *list, int index);

int Find(List *list, int value);
int FindString(StringList *list, char* value);

bool Contains(List *list, int value);
bool ContainsString(StringList *list, void *value);

StringList *Split(char *string, char split);
List *SplitInt(char *string, char split);

char *Alloc(int n, FILE *fp);
char *SortBeolvas(FILE *fp);

int ConvertToInt(char *str);
char *CpyStrPtr(char *str);
char *Merge(StringList *list, int start, int end);

char *SortBeolvasStd();
#endif
