/** @file
  Plik zawiera interfejs funkcji zapisywania wielomianu

  @author Maja Wiśniewska <mw429666.students.mimuw.edu.pl>
  @date 2021
*/

#ifndef __SAVEPOLY_H__
#define __SAVEPOLY_H__

/**
 * Funkcja wczytuje wielomian i jeśli nie wykryto błędu to wkłada go
 * na wierzch stosu, w przeciwnym wypadku wypisuje odpowiedni komunikat.
 * @param[in] Line : wiersz
 * @param[in,out] Stack : stos
 * @param[in] numberofLine : numer wiersza
 */
void savePoly(const line *Line, stack *Stack, size_t numberofLine);

#endif /* __SAVEPOLY_H__ */