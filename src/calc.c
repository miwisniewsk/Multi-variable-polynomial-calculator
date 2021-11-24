/** @file
  Implementacja kalkulatora wielomianów rzadkich wielu zmiennych.

  @author Maja Wiśniewska <mw429666.students.mimuw.edu.pl>
  @date 2021
*/

#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "savePoly.h"

/**
 * Funkcja rozpoznaje komendę. W przypadku gdy wiersz nie jest
 * żadną z komend wypisuje komunikat błędu.
 * @param[in] Line : wiersz
 * @param[in,out] Stack : stos
 * @param[in] numberofLine : numer wiersza
 */
static void command(const line *Line, stack *Stack, size_t numberofLine) {
    bool done = false;
    if (strcmp(Line->letters, "ZERO") == 0 && Line->numberofLetters == strlen("ZERO")) {
        ZERO(Stack);
        done = true;
    }
    if (strcmp(Line->letters, "IS_COEFF") == 0 && Line->numberofLetters == strlen("IS_COEFF")) {
        IS_COEFF(Stack, numberofLine);
        done = true;
    }
    if (strcmp(Line->letters, "IS_ZERO") == 0 && Line->numberofLetters == strlen("IS_ZERO")) {
        IS_ZERO(Stack, numberofLine);
        done = true;
    }
    if (strcmp(Line->letters, "CLONE") == 0 && Line->numberofLetters == strlen("CLONE")) {
        CLONE(Stack, numberofLine);
        done = true;
    }
    if (strcmp(Line->letters, "ADD") == 0 && Line->numberofLetters == strlen("ADD")) {
        ADD(Stack, numberofLine);
        done = true;
    }
    if (strcmp(Line->letters, "MUL") == 0 && Line->numberofLetters == strlen("MUL")) {
        MUL(Stack, numberofLine);
        done = true;
    }
    if (strcmp(Line->letters, "NEG") == 0 && Line->numberofLetters == strlen("NEG")) {
        NEG(Stack, numberofLine);
        done = true;
    }
    if (strcmp(Line->letters, "SUB") == 0 && Line->numberofLetters == strlen("SUB")) {
        SUB(Stack, numberofLine);
        done = true;
    }
    if (strcmp(Line->letters, "IS_EQ") == 0 && Line->numberofLetters == strlen("IS_EQ")) {
        IS_EQ(Stack, numberofLine);
        done = true;
    }
    if (strcmp(Line->letters, "DEG") == 0 && Line->numberofLetters == strlen("DEG")) {
        DEG(Stack, numberofLine);
        done = true;
    }
    if (strncmp(Line->letters, "DEG_BY", strlen("DEG_BY"))) {
        DEG_BY(Stack, numberofLine, Line);
        done = true;
    }
    if (strncmp(Line->letters, "AT", strlen("AT"))) {
        AT(Stack, numberofLine, Line);
        done = true;
    }
    if (strcmp(Line->letters, "PRINT") == 0 && Line->numberofLetters == strlen("PRINT")) {
        PRINT(Stack, numberofLine);
        done = true;
    }
    if (strcmp(Line->letters, "POP") == 0 && Line->numberofLetters == strlen("POP")) {
        POP(Stack, numberofLine);
        done = true;
    }
    if (strncmp(Line->letters, "COMPOSE", strlen("COMPOSE"))) {
        COMPOSE(Stack, numberofLine, Line);
        done = true;
    }
    if (!done) {
        fprintf(stderr, "ERROR %ld WRONG COMMAND\n", numberofLine);
    }
}

/**
 * Funkcja rozpoznaje, czy wiersz zawiera komendę czy wielomian i dalej
 * zapisuje wielomian lub wykonuje komendę. W przypadku błędu wypisuje
 * odpowiedni komunikat, a puste wiersze ignoruje. Potem zwalnia wiersz.
 * @param[in,out] Line : wiersz
 * @param[in,out] Stack : stos
 * @param[in] numberofLine : numer wiersza
 */
static void recognize(line *Line, stack *Stack, size_t numberofLine) {
    if (Line->numberofLetters != 0) {
        if (!(Line->numberofLetters == 1 && Line->letters[0] == '\n')) {
            if ((Line->letters[0] >= 'A' && Line->letters[0] <= 'Z') ||
            (Line->letters[0] >= 'a' && Line->letters[0] <= 'z') ) {
                command(Line, Stack, numberofLine);
            } else {
                savePoly(Line, Stack, numberofLine);
            }
        }
    }
    free(Line->letters);
    free(Line);
}

/**
 * Funkcja wczytuje wiersze znak po znaku i wykonuje odpowiednie operacje.
 * Ignoruje wiersze zaczynające się znakiem "#".
 * @param[in,out] Stack : stos
 */
static void readInput(stack *Stack) {
    int k = getchar();
    size_t numberofLine = 0;
    while (k != EOF) {
        ++numberofLine;
        line *Line = createaLine();
        if (k == '#') {
            while (k != '\n' && k != EOF) {
                k = getchar();
            }
        }
        while (k != '\n' && k != EOF) {
            if (Line->sizeofArray == Line->numberofLetters + 1) {
                enlargeLine(Line);
            }
            Line->letters[Line->numberofLetters] = (char) k;
            ++Line->numberofLetters;
            k = getchar();
        }
        recognize(Line, Stack, numberofLine);
        k = getchar();
    }
}

/**
 * Funkcja wtworzy pusty stos, wczytuje wiersze i wykonuje komendy, 
 * a potem czyści stos.
 * @return 0 w przypadku poprawnego wykonania, w przeciwnym przypadku kod błędu.
 */
int main(void) {
    stack Stack = Init();

    readInput(&Stack);

    Clear(&Stack);
    
    return 0;
}