/** @file
  Plik zawiera implementację funkcji zapisywania wielomianu

  @author Maja Wiśniewska <mw429666.students.mimuw.edu.pl>
  @date 2021
*/

#include "mallocSafe.h"
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#include "line.h"

static void savePolyHelp(size_t start, size_t stop, const line *Line, Poly *p, bool *correctPoly);

/**
 * Funkcja szuka indeksu, który odróżnia dwie współrzędne jednomianu.
 * @param[in] start : indeks początkowy
 * @param[in] stop : indeks końcowy
 * @param[in] Line : wiersz
 * @return indeks przecinka
 */
static size_t searchComma(size_t start, size_t stop, const line *Line) {
    size_t opening = 0;
    size_t closing = 0;

    for (size_t i = start; i <= stop; ++i) {
        if (Line->letters[i] == '(') {
            ++opening;
        }
        if (Line->letters[i] == ')') {
            ++closing;
        }
        if (Line->letters[i] == ',' && closing == opening) {
            return i;
        }
    }
    return 0;
}

/**
 * Funkcja sprawdza, czy wczytana wartość wykładnika jest poprawna.
 * @param[in] Line : wiersz
 * @param[in] x : wartość wykładnika
 * @param[in] start : indeks początkowy
 * @param[in] stop : indeks końcowy
 * @return Czy poprawna wartość?
 */
static bool correctExp(const line *Line, long int x, size_t start, size_t stop) {
    bool correct = Line->letters[start] >= '0' && Line->letters[start] <= '9';

    if (x < 0 || x > INT_MAX || 
    (x == 0 && !(Line->letters[start] == '0' && stop - start == 0))) {
        correct = false;
    }

    if (strcmp(&(Line->letters[start]), "2147483647") == 0) {
        correct = true;
    }

    return correct;
}

/**
 * Funkcja zwraca wartośc wykładnika jednomianu, w przypadku błędu
 * zapisuje ten fakt na zmiennej correct.
 * @param[in] Line : wiersz
 * @param[in] start : indeks początkowy
 * @param[in] stop : indeks końcowy
 * @param[out] correct : czy poprawny wielomian?
 * @return wartość wykładnika jednomianu
 */
static poly_exp_t saveExp(const line *Line, size_t start, size_t stop, bool *correct) {
    poly_exp_t k;

    char *end;

    long int x = strtol(&(Line->letters[start]), &end, 10);

    if (correctExp(Line, x, start, stop) && end[0] == ')') {
        k = (poly_exp_t) x;
    } else {
        *correct = false;
        k = 0;
    }

    return k;
}

/**
 * Zapisuje w tablicy jednomianów monos jednomian, który znajduje się między
 * podanymi indeksami w wierszu.
 * @param[in] start : indeks początkowy w wierszu
 * @param[in] stop : indeks końcowy w wierszu
 * @param[in] i : indeks w tablicy jednomianów
 * @param[out] monos: tablica jednomianów
 * @param[in] Line : wiersz znaków
 * @param[in,out] correctPoly : Czy poprawny wielomian?
 */
static void saveMono(size_t start, size_t stop, size_t i, Mono *monos, 
        const line *Line, bool *correctPoly) {
    if (Line->letters[start] != '(') {
        *correctPoly = false;
        Poly p = PolyZero();
        monos[i] = MonoFromPoly(&p, 1);
    } else {
        monos[i].p = PolyZero();
        size_t comma = searchComma(start + 1, stop - 1, Line);
        if (comma != 0) {
            savePolyHelp(start + 1, comma - 1, Line, &(monos[i].p), correctPoly);
            monos[i].exp = saveExp(Line, comma + 1, stop - 1, correctPoly);
        } else {
            *correctPoly = false;
            monos[i].exp = 1;
        }
    }
}

/**
 * Funkcja sprawdza, czy wczytana wartość współczynnika jest poprawna
 * @param[in] Line : wiersz
 * @param[in] x : wartość współczynnika
 * @param[in] start : indeks początkowy
 * @param[in] stop : indeks końcowy
 * @return Czy poprawna wartość?
 */
static bool correctCoeff(const line *Line, llint x, size_t start, size_t stop) {
    bool correct = Line->letters[start] == '-' ||
    (Line->letters[start] >= '0' && Line->letters[start] <= '9');
    for (size_t i = start + 1; i <= stop; ++i) {
        if (!(Line->letters[i] >= '0' && Line->letters[i] <= '9')) {
            correct = false;
        }
    }

    if (x == LLONG_MIN || x == LLONG_MAX ||
    (x == 0 && !(Line->letters[start] == '0' && stop - start == 0))) {
        correct = false;
    }

    if (stop - start == 1 && Line->letters[start] == '-' && Line->letters[stop] == '0') {
        correct = true; 
    }

    if (strcmp(&(Line->letters[start]), "-9223372036854775808") == 0 ||
        strcmp(&(Line->letters[start]), "9223372036854775807") == 0) {
        correct = true;
    }

    return correct;
}

/**
 * Zapisuje wielomian stały, który znajduje się między podanymi indeksami w wierszu.
 * Jeśli podczas wczytywania wielomianu występuje błąd to zapisuje 
 * ten fakt na zmiennej correctPoly.
 * @param[in] Line : wiersz
 * @param[in] start : indeks początkowy
 * @param[in] stop : indeks końcowy
 * @param[out] correctPoly : Czy poprawny wielomian?
 * @param[out] p : wielomian
 */
static void saveCoeff(const line *Line, size_t start, size_t stop, bool *correctPoly, Poly *p) {
    char* end;
    llint x = strtoll(&(Line->letters[start]), &end, 10);

    if (correctCoeff(Line, x, start, stop) && (end[0] == 0 || end[0] == ',')) {
        *p = PolyFromCoeff(x);
    } else {
        *correctPoly = false;
    }
}

/**
 * Funkcja szuka końca danego jednomianu.
 * @param[in] start : indeks początkowy
 * @param[in] stop : indeks końcowy
 * @param[in] Line : wiersz
 * @return indeks nawiasu zamykającego.
 */
static size_t searchClosePar(size_t start, size_t stop, const line *Line) {
    size_t opening = 0;
    size_t closing = 0;
    for (size_t i = start; i <= stop; ++i) {
        if (Line->letters[i] == '(') {
            ++opening;
        }
        if (Line->letters[i] == ')') {
            closing++;
            if (closing == opening) {
                return i;
            }
        }
    }
    return 0;
}

/**
 * Funkcja liczy z ilu jednomianów skłąda się dany wielomian.
 * @param[in] Line : wiersz
 * @param[in] start : indeks początkowy
 * @param[in] stop : indeks końcowy
 * @return liczba jednomianów
 */
static size_t countMonos(const line *Line, size_t start, size_t stop) {
    size_t opening = 0;
    size_t closing = 0;
    size_t monos = 1;

    for (size_t i = start; i <= stop; ++i) {
        if (Line->letters[i] == '(') {
            ++opening;
        }
        if (Line->letters[i] == ')') {
            ++closing;
        }
        if (Line->letters[i] == '+' && closing == opening) {
            ++monos;
        }
    }

    return monos;
}

/**
 * Funkcja zapisuje jednomiany między nawiasami i sprawdza, czy są poprawnie zapisane.
 * Jeśli podczas wczytywania wielomianu występuje błąd to zapisuje
 * ten fakt na zmiennej correctPoly.
 * @param[in] start : indeks początkowy
 * @param[in] stop : indeks końcowy
 * @param[in] numberofMonos : liczba jednomianów w tablicy
 * @param[out] monos : tablica jednomianów
 * @param[in] Line : wiersz znaków
 * @param[out] correctPoly : Czy poprawny wielomian?
 */
static void PolySaveMonos(size_t start, size_t stop, size_t numberofMonos, Mono *monos,
        const line *Line, bool *correctPoly) {
    size_t openPar = start;
    for (size_t i = 0; i < numberofMonos; ++i) {
        size_t closePar = searchClosePar(openPar, stop, Line);
        if (closePar > openPar + 1) {
            saveMono(openPar, closePar, i, monos, Line, correctPoly);
        } else {
            *correctPoly = false;
            Poly p = PolyZero();
            monos[i] = MonoFromPoly(&p, 1);
        }
        if (stop > closePar + 1) {
            if (Line->letters[closePar + 1] == '+') {
                if (Line->numberofLetters == closePar + 2) {
                    *correctPoly = false;
                }
            } else {
                *correctPoly = false;
            }
        }
        openPar = closePar + 2;
    }
}

/**
 * Zapisuje wielomian, który znajduje się między podanymi indeksami w wierszu.
 * Jeśli podczas wczytywania wielomianu występuje błąd to zapisuje
 * ten fakt na zmiennej correctPoly.
 * @param[in] start : indeks początkowy
 * @param[in] stop : indeks końcowy
 * @param[in] Line : wiersz znaków
 * @param[out] p : wielomian
 * @param[in,out] correctPoly : Czy poprawny wielomian?
 */
static void savePolyHelp(size_t start, size_t stop, const line *Line, Poly *p, bool *correctPoly) {
    if (Line->letters[start] == '(') {
        size_t numberofMonos = countMonos(Line, start, stop);
        Mono *monos = (Mono *) mallocSafe(numberofMonos * sizeof(Mono));
        PolySaveMonos(start, stop, numberofMonos, monos, Line, correctPoly);
        if (*correctPoly) {
            *p = PolyAddMonos(numberofMonos, monos);
        } else {
            for (size_t i = 0; i < numberofMonos; ++i) {
                MonoDestroy(&(monos[i]));
            }
        }
        free(monos);
    } else {
        saveCoeff(Line, start, stop, correctPoly, p);
    }
}

void savePoly(const line *Line, stack *Stack, size_t numberofLine) {
    bool correctPoly = true;
    Poly p = PolyZero();

    if (Line->letters[0] == '(') {
        size_t end = searchClosePar(0, Line->numberofLetters - 1, Line);
        if (end != Line->numberofLetters - 1) {
            correctPoly = false;
        }
    }

    savePolyHelp(0, Line->numberofLetters - 1, Line, &p, &correctPoly);
    if (correctPoly) {
        Push(Stack, p);
    } else {
        fprintf(stderr, "ERROR %ld WRONG POLY\n", numberofLine);
        PolyDestroy(&p);
    }
}