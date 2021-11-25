/** @file
  A file that stores a function for safely allocating memory.

  @author Maja Wiśniewska <mw429666.students.mimuw.edu.pl>
  @date 2021
*/

#ifndef __MALLOCSAFE_H__
#define __MALLOCSAFE_H__

#include <stdlib.h>

/**
 * The function will allocate @p size bytes, if possible.
 * Otherwise it terminates the program with code 1 emergency.
 * @param[in] size : number of bytes
 * @return type void pointer
 */
static inline void *mallocSafe(size_t size) {
    void *pointer = malloc(size);
    if (pointer == NULL) {
        exit(1);
    }
    return pointer;
}

#endif /* __MALLOCSAFE_H__ */
