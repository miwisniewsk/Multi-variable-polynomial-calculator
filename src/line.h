/** @file
  Plik zawiera strukturę wiersza i interfejs funkcji tworzenia i powiększania wiersza

  @author Maja Wiśniewska <mw429666.students.mimuw.edu.pl>
  @date 2021
*/

#ifndef __LINE_H__
#define __LINE_H__

#include <stdio.h>

/**
 * To jest struktura przechowująca wiersz wejścia.
 */
typedef struct{
    char *letters;           ///< tablica znaków
    size_t numberofLetters;  ///< ilość znaków
    size_t sizeofArray;      ///< długość tablicy
} line;

/**
 * Funkcja tworzy pusty wiersz.
 * @return wiersz
 */
line *createaLine(void);

/**
 * Funkcja powiększa wiersz.
 * @param[in,out] Line : wiersz
 */
void enlargeLine(line *Line);

#endif /* __LINE_H__ */
