/** @file
  The file contains the interface of the polynomial saving function

  @author Maja Wiśniewska <mw429666.students.mimuw.edu.pl>
  @date 2021
*/

#ifndef __SAVEPOLY_H__
#define __SAVEPOLY_H__

/**
 * The function reads the polynomial and inserts it if no error is found
 * on top of the stack, otherwise it prints the appropriate message.
 * @param[in] Line : line
 * @param[in,out] Stack : stack
 * @param[in] numberofLine : number of line
 */
void savePoly(const line *Line, stack *Stack, size_t numberofLine);

#endif /* __SAVEPOLY_H__ */
