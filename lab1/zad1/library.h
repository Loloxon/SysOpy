#ifndef SYSOPY_LIBRARY_H
#define SYSOPY_LIBRARY_H

#include <bits/types/FILE.h>

typedef struct blockTable{
    char **info;
    int size;
}blockTable;
//    utworzenie tablicy wskaźników w której będą przechowywane wskaźniki na bloki pamięci zawierające wyniki
blockTable* createPointersTable(int size);

//    przeprowadzenie zliczenia lini, słów i znaków dla zadanych plików i zapisanie wyniku zliczania w pliku tymczasowym
int countLinesWordsChars(char* filePath[], int s, blockTable* table);

//    zarezerwowanie bloku pamięci o rozmiarze odpowiadającym rozmiarowi pliku tymczasowego i zapisanie w tej pamięci
//    jego zawartości, ustawienie w tablicy wskaźników wskazania na ten blok, funkcja powinna zwrócić indeks stworzonego
//    bloku w tablicy,
void saveToBlock();

//    usunięcie z pamięci bloku o zadanym indeksie
void freeMem(int index, blockTable* table);

#endif //SYSOPY_LIBRARY_H
