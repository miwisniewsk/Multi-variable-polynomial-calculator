/** @file
  Stack command interface

  @author Maja Wiśniewska <mw429666.students.mimuw.edu.pl>
  @date 2021
*/

#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "stack.h"
#include "line.h"

/**
 * The function inserts a polynomial equal to zero at the top of the stack.
 * @param[out] Stack : stack
 */
void ZERO(stack *Stack);

/**
 * The function checks if the polynomial at the top of the stack is a constant polynomial.
 * Prints an error message in case of an empty stack.
 * @param[in] Stack : stack
 * @param[in] numberofLine : number of line
 */
void IS_COEFF(const stack *Stack, size_t numberofLine);

/**
 * The function checks if the polynomial at the top of the stack is 0.
 * Prints an error message in case of an empty stack.
 * @param[in] Stack : stack
 * @param[in] numberofLine : number of line
 */
void IS_ZERO(const stack *Stack, size_t numberofLine);

/**
 * The function copies the polynomial from the vertex.
 * Prints an error message in case of an empty stack.
 * @param[in,out] Stack : stack
 * @param[in] numberofLine : number of line
 */
void CLONE(stack *Stack, size_t numberofLine);

/**
 * The function adds two polynomials from the top of the stack,
 * removes them and puts their sum at the top of the stack.
 * In case of too few polynomials on the stack, it prints an error message.
 * @param[in,out] Stack : stack
 * @param[in] numberofLine : number of line
 */
void ADD(stack *Stack, size_t numberofLine);

/**
 * The function multiplies the two polynomials from the top of the stack,
 * removes them and puts their product at the top of the stack.
 * In case of too few polynomials on the stack, it prints an error message.
 * @param[in,out] Stack : stack
 * @param[in] numberofLine : number of line
 */
void MUL(stack *Stack, size_t numberofLine);

/**
 * The function negates the polynomial at the top of the stack.
 * Prints an error message in case of an empty stack.
 * @param[in,out] Stack : stack
 * @param[in] numberofLine : number of line
 */
void NEG(stack *Stack, size_t numberofLine);

/**
 * The function subtracts from the polynomial at the vertex, the polynomial at the vertex,
 * removes them and puts the difference on top of the stack.
 * In case of too few polynomials on the stack, it prints an error message.
 * @param[in,out] Stack : stack
 * @param[in] numberofLine : number of line
 */
void SUB(stack *Stack, size_t numberofLine);

/**
 * The function checks if the two polynomials on the top of the stack are equal
 * - writes 0 or 1 to the standard output.
 * In case of too few polynomials on the stack, it prints an error message.
 * @param[in,out] Stack : stack
 * @param[in] numberofLine : number of line
 */
void IS_EQ(stack *Stack, size_t numberofLine);

/**
 * The function prints the degree of the polynomial to the standard output.
 * Prints an error message in case of an empty stack.
 * @param[in,out] Stack : stack
 * @param[in] numberofLine : number of line
 */
void DEG(const stack *Stack, size_t numberofLine);

/**
 * The function prints the degree of the polynomial to the standard output
 * due to the variable number idx.
 * W przypadku pustego stosu lub złego indeksu wypisuje komunikat błędu.
 * @param[in,out] Stack : stack
 * @param[in] numberofLine : number of line
 * @param[in] Line : line
 */
void DEG_BY(const stack *Stack, size_t numberofLine, const line *Line);

/**
 * The function calculates the value of the polynomial at the point x,
 * removes the polynomial from the vertex and puts the result of the operation on the stack.
 * Prints an error message in case of an empty stack or bad index.
 * @param[in,out] Stack : stack
 * @param[in] numberofLine : number of line
 * @param[in] Line : line

 */
void AT(stack *Stack, size_t numberofLine, const line *Line);

/**
 * The function prints the polynomial at the top of the stack.
 * Prints an error message in case of an empty stack.
 * @param[in,out] Stack : stack
 * @param[in] numberofLine : number of line
 */
void PRINT(const stack *Stack, size_t numberofLine);

/**
 * The function pops the polynomial from the top of the stack.
 * Prints an error message in case of an empty stack.
 * @param[in,out] Stack : stack
 * @param[in] numberofLine : number of line
 */
void POP(stack *Stack, size_t numberofLine);

/**
 * The function loads the size of an array of polynomials and if no error occurs,
 * removes the appropriate number of polynomials from the stack and puts the fold result on top.
 * In case of an error, it prints an appropriate message.
 * @param[in,out] Stack : stack
 * @param[in] numberofLine : number of line
 * @param[in] Line : line
 */
void COMPOSE(stack *Stack, size_t numberofLine, const line *Line);

#endif /* __COMMAND_H__ */
