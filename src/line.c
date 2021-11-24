/** @file
  Plik zawiera implementację funkcji tworzenia i powiększania wiersza

  @author Maja Wiśniewska <mw429666.students.mimuw.edu.pl>
  @date 2021
*/

#include "line.h"
#include "mallocSafe.h"
#include <string.h>

line *createaLine(void) {
    line *Line = (line *) mallocSafe(sizeof(line));

    Line->letters = (char *) mallocSafe(sizeof(char));
    Line->letters[0] = 0;
    Line->sizeofArray = 1;
    Line->numberofLetters = 0;

    return Line;
}

/**
 * Prosta funkcja zwracająca w przybliżeniu podwojoną wartość.
 * @param[in] n : liczba całkowita
 * @return result
 */
static size_t more(size_t n) {
    size_t result = 2 * n + 1;

    return result;
}

void enlargeLine(line *Line) {
    Line->sizeofArray = more(Line->sizeofArray);
    Line->letters = (char *) realloc(Line->letters,
                                     Line->sizeofArray * sizeof(char));
    if (Line->letters == NULL) {
        exit(1);
    }
    memset(Line->letters + Line->numberofLetters, 0,
           Line->sizeofArray - Line->numberofLetters);
}
