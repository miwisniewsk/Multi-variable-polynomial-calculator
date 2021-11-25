/** @file
  Stack class interface

  @author Maja Wiśniewska <mw429666.students.mimuw.edu.pl>
  @date 2021
*/

#ifndef __STACK_H__
#define __STACK_H__

#include "poly.h"

/**
 * Shorter type notation of long long int
 */
typedef long long int llint;

/**
 * Shorter type notation of unsigned long long int
 */
typedef unsigned long long int ullint;

/**
 * This is the structure holding the stack of polynomials.
 */
typedef struct {
    Poly *Array;         ///< array of polynomials
    size_t sizeofArray;  ///< array size
    ullint top;          ///< number of items on the stack
} stack;

/**
 * The function checks if the stack is empty.
 * @param[in] Stack : stack
 * @return Is the stack empty?
 */
bool Empty(const stack *Stack);

/**
 * The function creates an empty stack.
 * @return empty stack.
 */
stack Init();

/**
 * The function puts a polynomial on the stack.
 * @param[in,out] Stack : stack
 * @param[in] p : polynomial
 */
void Push(stack *Stack, Poly p);

/**
 * The function pops the polynomial from the top of the stack.
 * @param[in,out] Stack : stack
 * @return polynomial
 */
Poly Pop(stack *Stack);

/**
 * The function returns the polynomial at the top of the stack.
 * @param[in] Stack : stack
 * @return polynomial
 */
Poly Top(const stack *Stack);

/**
 * The function clears the entire stack.
 * @param[in,out] Stack : stack
 */
void Clear(stack *Stack);

#endif /* __STACK_H__ */
