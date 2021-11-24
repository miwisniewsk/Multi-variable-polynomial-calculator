/** @file
  Plik zawiera implementację funkcji zawartych w pilku stack.h

  @author Maja Wiśniewska <mw429666.students.mimuw.edu.pl>
  @date 2021
*/

#include <stdlib.h>
#include "stack.h"

/**
 * Prosta funkcja zwracająca w przybliżeniu podwojoną wartość.
 * @param[in] n : liczba całkowita
 * @return result
 */
static size_t more(size_t n) {
    size_t result = 2 * n + 1;

    return result;
}

/**
 * Funkcja powiększająca stos.
 * @param[in,out] Stack : stos
 */
static void enlargeStack(stack *Stack) {
    Stack->sizeofArray = more(Stack->sizeofArray);
    Stack->Array = (Poly *) realloc(Stack->Array,Stack->sizeofArray * sizeof(Poly));
    if (Stack->Array == NULL) {
        exit(1);
    }
}

bool Empty(const stack *Stack) {
    return (Stack->top == 0);
}

stack Init() {
    stack Stack;
    Stack.top = 0;
    Stack.sizeofArray = 0;
    Stack.Array = NULL;
    return Stack;
}

void Push(stack *Stack, Poly p) {
    if (Stack->sizeofArray == Stack->top) {
        enlargeStack(Stack);
    }
    Stack->Array[Stack->top] = p;
    ++Stack->top;
}

Poly Pop(stack *Stack) {
    --Stack->top;
    return Stack->Array[Stack->top];
}

Poly Top(const stack *Stack) {
    return Stack->Array[Stack->top - 1];
}

void Clear(stack *Stack) {
    for (ullint i = 0; i < Stack->top; ++i) {
        PolyDestroy(&Stack->Array[i]);
    }
    free(Stack->Array);
    Stack->sizeofArray = 0;
    Stack->top = 0;
}