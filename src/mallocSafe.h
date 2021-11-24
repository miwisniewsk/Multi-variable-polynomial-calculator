/** @file
  Plik przechowujący funkcję służacą do bezpiecznego alokowania pamięci.

  @author Maja Wiśniewska <mw429666.students.mimuw.edu.pl>
  @date 2021
*/

#ifndef __MALLOCSAFE_H__
#define __MALLOCSAFE_H__

#include <stdlib.h>

/**
 * Funkcja alokuje @p size bajtów, o ile to możliwe. 
 * W przeciwnym przypadku zakańcza awaryjnie program z kodem 1.
 * @param[in] size : liczba bajtów
 * @return wskaźnik typu void
 */
static inline void *mallocSafe(size_t size) {
    void *pointer = malloc(size);
    if (pointer == NULL) {
        exit(1);
    }
    return pointer;
}

#endif /* __MALLOCSAFE_H__ */