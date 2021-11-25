/** @file
  The file contains the row structure and the row creation and magnification interface

  @author Maja Wiśniewska <mw429666.students.mimuw.edu.pl>
  @date 2021
*/

#ifndef __LINE_H__
#define __LINE_H__

#include <stdio.h>

/**
 * This is the structure that holds the line of entry.
 */
typedef struct{
    char *letters;           ///< array of characters
    size_t numberofLetters;  ///< number of characters
    size_t sizeofArray;      ///< array length
} line;

/**
 * The function creates an empty line.
 * @return line
 */
line *createaLine(void);

/**
 * The function enlarges the row.
 * @param[in,out] Line : line
 */
void enlargeLine(line *Line);

#endif /* __LINE_H__ */
