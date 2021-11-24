/** @file
  Interfejs komend stosu

  @author Maja Wiśniewska <mw429666.students.mimuw.edu.pl>
  @date 2021
*/

#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "stack.h"
#include "line.h"

/**
 * Funkcja wstawia na szczyt stosu wielomian równy zeru.
 * @param[out] Stack : stos
 */
void ZERO(stack *Stack);

/**
 * Funkcja sprawdza, czy wielomian z wierzchołka stosu jest wielomianem stałym.
 * W przypadku pustego stostu wypisuje komunikat błędu.
 * @param[in] Stack : stos
 * @param[in] numberofLine : numer wiersza
 */
void IS_COEFF(const stack *Stack, size_t numberofLine);

/**
 * Funkcja sprawdza, czy wielomian z wierzchołka stosu jest równy 0.
 * W przypadku pustego stostu wypisuje komunikat błędu.
 * @param[in] Stack : stos
 * @param[in] numberofLine : numer wiersza
 */
void IS_ZERO(const stack *Stack, size_t numberofLine);

/**
 * Funkcja kopiuje wielomian z wierzchołka.
 * W przypadku pustego stostu wypisuje komunikat błędu.
 * @param[in,out] Stack : stos
 * @param[in] numberofLine : numer wiersza
 */
void CLONE(stack *Stack, size_t numberofLine);

/**
 * Funkcja dodaje dwa wielomiany z wierzchu stosu, 
 * usuwa je i wstawia na wierzchołek stosu ich sumę.
 * W przypadku za małej ilości wielomianów na stosie wypisuje komunikat błędu.
 * @param[in,out] Stack : stos
 * @param[in] numberofLine : numer wiersza
 */
void ADD(stack *Stack, size_t numberofLine);

/**
 * Funkcja mnoży dwa wielomiany z wierzchu stosu, 
 * usuwa je i wstawia na wierzchołek stosu ich iloczyn.
 * W przypadku za małej ilości wielomianów na stosie wypisuje komunikat błędu.
 * @param[in,out] Stack : stos
 * @param[in] numberofLine : numer wiersza
 */
void MUL(stack *Stack, size_t numberofLine);

/**
 * Funkcja neguje wielomian na wierzchołku stosu.
 * W przypadku pustego stosu wypisuje komunikat błędu.
 * @param[in,out] Stack : stos
 * @param[in] numberofLine : numer wiersza
 */
void NEG(stack *Stack, size_t numberofLine);

/**
 * Funkcja odejmuje od wielomianu z wierzchołka wielomian pod wierzchołkiem, 
 * usuwa je i wstawia na wierzchołek stosu różnicę.
 * W przypadku za małej ilości wielomianów na stosie wypisuje komunikat błędu.
 * @param[in,out] Stack : stos
 * @param[in] numberofLine : numer wiersza
 */
void SUB(stack *Stack, size_t numberofLine);

/**
 * Funkcja sprawdza, czy dwa wielomiany na wierzchu stosu są równe 
 * – wypisuje na standardowe wyjście 0 lub 1.
 * W przypadku za małej ilości wielomianów na stosie wypisuje komunikat błędu.
 * @param[in] Stack : stos
 * @param[in] numberofLine : numer wiersza
 */
void IS_EQ(stack *Stack, size_t numberofLine);

/**
 * Funkcja wypisuje na standardowe wyjście stopień wielomianu.
 * W przypadku pustego stosu wypisuje komunikat błędu.
 * @param[in] Stack : stos
 * @param[in] numberofLine : numer wiersza
 */
void DEG(const stack *Stack, size_t numberofLine);

/**
 * Funkcja wypisuje na standardowe wyjście stopień wielomianu 
 * ze względu na zmienną o numerze idx.
 * W przypadku pustego stosu lub złego indeksu wypisuje komunikat błędu.
 * @param[in] Stack : stos
 * @param[in] numberofLine : numer wiersza
 * @param[in] Line : wiersz
 */
void DEG_BY(const stack *Stack, size_t numberofLine, const line *Line);

/**
 * Funkcja wylicza wartość wielomianu w punkcie x, 
 * usuwa wielomian z wierzchołka i wstawia na stos wynik operacji.
 * W przypadku pustego stosu lub złego indeksu wypisuje komunikat błędu.
 * @param[in,out] Stack : stos
 * @param[in] numberofLine : numer wiersza
 * @param[in] Line : wiersz
 */
void AT(stack *Stack, size_t numberofLine, const line *Line);

/**
 * Funkcja wypisuje wielomian z wierzchołka stosu.
 * W przypadku pustego stosu wypisuje komunikat błędu.
 * @param[in] Stack : stos
 * @param[in] numberofLine : numer wiersza
 */
void PRINT(const stack *Stack, size_t numberofLine);

/**
 * Funkcja zdejmuje wielomian z wierzchołka stosu.
 * W przypadku pustego stosu wypisuje komunikat błędu.
 * @param[in] Stack : stos
 * @param[in] numberofLine : numer wiersza
 */
void POP(stack *Stack, size_t numberofLine);

/**
 * Funkcja wczytuje rozmiar tablicy wielomianów i jeśli nie następuje żaden błąd,
 * zdejmuje odpowiednią liczbę wielomianów ze stosu i wkłada na wierzch wynik składania.
 * W przypadku błędu wypisuje odpowiedni komunikat.
 * @param[in,out] Stack : stos
 * @param[in] numberofLine : numer wiersza
 * @param[in] Line : wiersz
 */
void COMPOSE(stack *Stack, size_t numberofLine, const line *Line);

#endif /* __COMMAND_H__ */