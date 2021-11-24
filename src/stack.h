/** @file
  Interfejs klasy stosu

  @author Maja Wiśniewska <mw429666.students.mimuw.edu.pl>
  @date 2021
*/

#ifndef __STACK_H__
#define __STACK_H__

#include "poly.h"

/**
 * Krótszy zapis typu long long int
 */
typedef long long int llint;

/**
 * Krótszy zapis typu unsigned long long int
 */
typedef unsigned long long int ullint;

/**
 * To jest struktura przechowująca stos wielomianów.
 */
typedef struct {
    Poly *Array;         ///< tablica wielomianów
    size_t sizeofArray;  ///< rozmiar tablicy
    ullint top;          ///< ilość elementow na stosie
} stack;

/**
 * Funkcja sprawdza, czy stos jest pusty.
 * @param[in] Stack : stos
 * @return Czy stos jest pusty?
 */
bool Empty(const stack *Stack);

/**
 * Funkcja tworzy pusty stos.
 * @return pusty stos
 */
stack Init();

/**
 * Funkcja wkłada wielomian na stos.
 * @param[in,out] Stack : stos
 * @param[in] p : wielomian
 */
void Push(stack *Stack, Poly p);

/**
 * Funkcja zdejmuje wielomian z wierzchołka stosu.
 * @param[in,out] Stack : stos
 * @return wielomian
 */
Poly Pop(stack *Stack);

/**
 * Funkcja zwraca wielomian z wierzchołka stosu.
 * @param[in] Stack : stos
 * @return wielomian
 */
Poly Top(const stack *Stack);

/**
 * Funkcja czyści cały stos.
 * @param[in,out] Stack : stos
 */
void Clear(stack *Stack);

#endif /* __STACK_H__ */