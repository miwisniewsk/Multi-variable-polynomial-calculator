/** @file
  Implementacja komend zawartych w pliku command.h

  @author Maja Wiśniewska <mw429666.students.mimuw.edu.pl>
  @date 2021
*/

#include "command.h"
#include "mallocSafe.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>

void ZERO(stack *Stack) {
    Poly p = PolyZero();
    Push(Stack, p);
}

void IS_COEFF(const stack *Stack, size_t numberofLine) {
    if (Empty(Stack)) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", numberofLine);
    } else {
        Poly p = Top(Stack);
        if (PolyIsCoeff(&p)) {
            printf("1\n");
        } else {
            printf("0\n");
        }
    }
}

void IS_ZERO(const stack *Stack, size_t numberofLine) {
    if (Empty(Stack)) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", numberofLine);
    } else {
        Poly p = Top(Stack);
        if (PolyIsZero(&p)) {
            printf("1\n");
        } else {
            printf("0\n");
        }
    }
}

void CLONE(stack *Stack, size_t numberofLine) {
    if (Empty(Stack)) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", numberofLine);
    } else {
        Poly p = Top(Stack);
        Poly q = PolyClone(&p);
        Push(Stack, q);
    }
}

void ADD(stack *Stack, size_t numberofLine) {
    if (Stack->top < 2) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", numberofLine);
    } else {
        Poly p = Pop(Stack);
        Poly q = Pop(Stack);
        Poly r = PolyAdd(&p, &q);
        Push(Stack, r);
        PolyDestroy(&p);
        PolyDestroy(&q);
    }
}

void MUL(stack *Stack, size_t numberofLine) {
    if (Stack->top < 2) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", numberofLine);
    } else {
        Poly p = Pop(Stack);
        Poly q = Pop(Stack);
        Poly r = PolyMul(&p, &q);
        Push(Stack, r);
        PolyDestroy(&p);
        PolyDestroy(&q);
    }
}

void NEG(stack *Stack, size_t numberofLine) {
    if (Empty(Stack)) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", numberofLine);
    } else {
        Poly p = Pop(Stack);
        Poly r = PolyNeg(&p);
        Push(Stack, r);
        PolyDestroy(&p);
    }
}

void SUB(stack *Stack, size_t numberofLine) {
    if (Stack->top < 2) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", numberofLine);
    } else {
        Poly p = Pop(Stack);
        Poly q = Pop(Stack);
        Poly r = PolySub(&p, &q);
        Push(Stack, r);
        PolyDestroy(&p);
        PolyDestroy(&q);
    }
}

void IS_EQ(stack *Stack, size_t numberofLine) {
    if (Stack->top < 2) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", numberofLine);
    } else {
        Poly p = Pop(Stack);
        Poly q = Top(Stack);
        Push(Stack, p);
        if (PolyIsEq(&p, &q)) {
            printf("1\n");
        } else {
            printf("0\n");
        }
    }
}

void DEG(const stack *Stack, size_t numberofLine) {
    if (Empty(Stack)) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", numberofLine);
    } else {
        Poly p = Top(Stack);
        poly_exp_t i = PolyDeg(&p);
        printf("%d\n", i);
    }
}

/**
 * Funkcja sprawdza, czy wczytana liczba jest poprawna.
 * @param[in] Line : wiersz
 * @param[in] var_idx : wczytana liczba
 * @param[in] start : indeks w wierszu, w którym zaczyna się liczba
 * @return Czy poprawna liczba?
 */
static bool correctIdx(const line *Line, ullint var_idx, size_t start) {
    bool correct = var_idx < ULLONG_MAX &&
                    Line->letters[start] >= '0' &&
                    Line->letters[start] <= '9';

    if (var_idx == 0 && !(Line->letters[start] == '0' &&
                          Line->numberofLetters == start + 1)) {
        correct = false;
    }

    if (strcmp(&(Line->letters[start]),
     "18446744073709551615") == 0) {
        correct = true;
    }

    return correct;
}

/**
 * Funkcja wczytuje indeks z wiersza, sprawdza, czy jest poprawny, 
 * jeśli tak to wypisuje na standardowe wyjście stopień wielomianu 
 * ze względu na zmienną o numerze idx. Jeśli nie to wypisuje odpowiedni komunikat.
 * @param[in] Line : wiersz
 * @param[in] Stack : stos
 * @param[in] numberofLine : numer wiersza
 */
static void DEG_BY_Help(const line *Line, const stack *Stack, size_t numberofLine) {
    char *end;
    
    ullint var_idx = strtoull(&(Line->letters[strlen("DEG_BY") + 1]), &end, 10);

    if (correctIdx(Line, var_idx, strlen("DEG_BY") + 1) && end[0] == 0) {
        if (Empty(Stack)) {
            fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n",numberofLine);
        } else {
            Poly p = Top(Stack);
            size_t idx = (size_t) var_idx;
            poly_exp_t x = PolyDegBy(&p, idx);
            printf("%d\n", x);
        }
    } else {
        fprintf(stderr, "ERROR %ld DEG BY WRONG VARIABLE\n",numberofLine);
    }
}

void DEG_BY(const stack *Stack, size_t numberofLine, const line *Line) {
    if (Line->numberofLetters == strlen("DEG_BY")) {
        fprintf(stderr, "ERROR %ld DEG BY WRONG VARIABLE\n", numberofLine);
    } else {
        if (Line->letters[strlen("DEG_BY")] != ' ') {
            fprintf(stderr, "ERROR %ld WRONG COMMAND\n", numberofLine);
        } else {
            if (Line->numberofLetters > strlen("DEG_BY") + 1) {
                DEG_BY_Help(Line, Stack, numberofLine);
            } else {
                fprintf(stderr, "ERROR %ld DEG BY WRONG VARIABLE\n", numberofLine);
            }
        }
    }
}

/**
 * Funkcja sprawdza, czy wczytana wartość jest poprawna.
 * @param[in] Line : wiersz
 * @param[in] variable : wartość
 * @return Czy poprawna wartość?
 */
static bool correctVariable(const line *Line, llint variable) {
    bool correct = (variable < LLONG_MAX) && (variable > LLONG_MIN) &&
                      ((Line->letters[strlen("AT ")] >= '0' && Line->letters[strlen("AT ")] <= '9') ||
                       Line->letters[strlen("AT ")] == '-');

    if (variable == 0 && !(Line->letters[strlen("AT ")] == '0' 
    && Line->numberofLetters == strlen("AT 0") )) {
        correct = false;
    }

    if (strcmp(&(Line->letters[3]), "-9223372036854775808") == 0 ||
    strcmp(&(Line->letters[3]), "9223372036854775807") == 0) {
        correct = true;
    }

    return correct;
}

void AT(stack *Stack, size_t numberofLine, const line *Line) {
    if (Line->numberofLetters == strlen("AT")) {
        fprintf(stderr, "ERROR %ld AT WRONG VALUE\n", numberofLine);
    } else {
        if (Line->letters[strlen("AT")] != ' ') {
            fprintf(stderr, "ERROR %ld WRONG COMMAND\n", numberofLine);
        } else {
            char *end;
            llint x = strtoll(&(Line->letters[strlen("AT ")]), &end, 10);
            if (correctVariable(Line, x) && end[0] == 0) {
                if (Empty(Stack)) {
                    fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", numberofLine);
                } else {
                    Poly p = Pop(Stack);
                    Poly q = PolyAt(&p, x);
                    Push(Stack, q);
                    PolyDestroy(&p);
                }
            } else {
                fprintf(stderr, "ERROR %ld AT WRONG VALUE\n", numberofLine);
            }
        }
    }
}

void PRINT(const stack *Stack, size_t numberofLine) {
    if (Empty(Stack)) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", numberofLine);
    } else {
        Poly p = Top(Stack);
        PrintPoly(&p);
        printf("\n");
    }
}

void POP(stack *Stack, size_t numberofLine) {
    if (Empty(Stack)) {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", numberofLine);
    } else {
        Poly p = Pop(Stack);
        PolyDestroy(&p);
    }
}

/**
 * Funkcja ta to właściwa część funkcji COMPOSE, kiedy wiemy już, że po
 * poleceniu "COMPOSE" następuje spacja i nie jest ona ostatnim znakiem w wierszu.
 * @param[in,out] Stack : stos
 * @param[in] numberofLine : numer wiersza
 * @param[in] Line : wiersz
 */
static void ComposeHelp(stack *Stack, size_t numberofLine, const line *Line) {
    char *end;
    ullint k = strtoull(&(Line->letters[8]), &end, 10);

    if (correctIdx(Line, k, 8) && end[0] == 0) {
        if (Stack->top - 1 < k) {
            fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", numberofLine);
        } else {
            Poly p = Pop(Stack);

            Poly *q = (Poly *) mallocSafe (k * sizeof(Poly));
            for (ullint i = 1; i <= k; ++i) {
                q[k - i] = Pop(Stack);
            }
        
            Poly r = PolyCompose(&p, k, q);
            Push(Stack, r);

            PolyDestroy(&p);
            for (ullint i = 0; i < k; ++i) {
                PolyDestroy(&(q[i]));
            }
            free(q);
        }
    } else {
        fprintf(stderr, "ERROR %ld COMPOSE WRONG PARAMETER\n", numberofLine);
    }   
}

void COMPOSE(stack *Stack, size_t numberofLine, const line *Line) {
    if (Line->numberofLetters == 7) {
        fprintf(stderr, "ERROR %ld COMPOSE WRONG PARAMETER\n", numberofLine);
    } else {
        if (Line->letters[7] != ' ') {
            fprintf(stderr, "ERROR %ld WRONG COMMAND\n", numberofLine);
        } else {
            if (Line->numberofLetters > 8) {
                ComposeHelp(Stack, numberofLine, Line);
            } else {
                fprintf(stderr, "ERROR %ld COMPOSE WRONG PARAMETER\n", numberofLine);
            }
        }
    }
}
