#include "debugmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include "essentials.h"
#include "loaderSystem.h"
#include "gameSpace.h"
#include "interpreter.h"
#include "graphics.h"
#include <windows.h>

/**
 * @return 0 Sikeres működés
 * @return 1 Memória foglalás hiba
 * @return 2 Fájlkezelés hiba
 * @return 3 Túlindexelés hiba
 */
int main(int argc, char const *argv[])
{
    SetConsoleOutputCP(65001); //UTF-8 karakterek kezelése
    MenuMode(); //A program azzal indul, hogy menu módba helyezzük
    return 0;
}
