/** @file
  Multivariable polynomial calss interface.

  @authors Jakub Pawlewicz <pan@mimuw.edu.pl>, Marcin Peczarski <marpe@mimuw.edu.pl>, 
  Maja Wiśniewska <mw429666.students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#ifndef __POLY_H__
#define __POLY_H__

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/** This is a type representing coefficients. */
typedef long poly_coeff_t;

/** This is a type representing exponent. */
typedef int poly_exp_t;

struct Mono;

/**
 * This is the structure representig polynomial.
 * Polynomial is either an integer (then 'arr == NULL'), 
 * or non-empty list of monomials (then `arr != NULL`).
 */
typedef struct Poly {
  /**
  * This is the union that holds the coefficient of the polynomial or
  * the number of monomials in the polynomial.
  * If `arr == NULL` then it is an integer coefficient.
  * Otherwise, it is a non-empty list of monomials. 
  */
  union {
    poly_coeff_t coeff; ///< coefficient
    size_t       size; ///< number of monomials
  };
  /** This is an array that holds a list of monomials */
  struct Mono *arr;
} Poly;

/**
 * This is the monomial holding structure.
 * The monomial takes the form @ f$px_i^n@f$.
 * The @f$p@f$ factor can also be
 * polynomial over the next variable @f$x_{i + 1}@f$.
 */
typedef struct Mono {
  Poly p; ///< factor
  poly_exp_t exp; ///< exponent
} Mono;

/**
 * Gives the value of the monomial exponent.
 * @param[in] m : monomial
 * @return the value of the exponent of the monomial
 */
static inline poly_exp_t MonoGetExp(const Mono *m) {
  return m->exp;
}

/**
 * Creates a polynomial that is a factor (constant polynomial).
 * @param[in] c : value of the coefficient
 * @return polynomial
 */
static inline Poly PolyFromCoeff(poly_coeff_t c) {
  return (Poly) {.coeff = c, .arr = NULL};
}

/**
 * Creates a polynomial identically equal to zero.
 * @return polynomial
 */
static inline Poly PolyZero(void) {
  return PolyFromCoeff(0);
}

static inline bool PolyIsCoeff(const Poly *p);

/**
 * Forms a monomial @f$px_i^n@f$.
 * Takes ownership of the content of the structure pointed to by @p p.
 * @param[in] p : polynomial - coefficient of the monomial
 * @param[in] n : exponent
 * @return monomial @f$px_i^n@f$
 */
static inline Mono MonoFromPoly(const Poly *p, poly_exp_t n) {
    assert(n == 0 || !(PolyIsCoeff(p) && p->coeff == 0));
    return (Mono) {.p = *p, .exp = n};
}

/**
 * Checks if the polynomial is a factor (or if it is a constant polynomial).
 * @param[in] p : polynomial
 * @return Is the polynomial a factor?
 */
static inline bool PolyIsCoeff(const Poly *p) {
  return p->arr == NULL;
}

/**
 * Checks if the polynomial is identically equal to zero.
 * @param[in] p : polynomial
 * @return Is the polynomial zero?
 */
bool PolyIsZero(const Poly *p);

/**
 * Removes a polynomial from memory.
 * @param[in] p : polynomial
 */
void PolyDestroy(Poly *p);

/**
 * Deletes the monomial from memory.
 * @param[in] m : monomial
 */
static inline void MonoDestroy(Mono *m) {
  PolyDestroy(&m->p);
}

/**
 * Make a full deep copy of a polynomial.
 * @param[in] p : polynomial
 * @return copied polynomial
 */
Poly PolyClone(const Poly *p);

/**
 * Makes a full, deep copy of the monomial.
 * @param[in] m : monomial
 * @return copied monomial
 */
static inline Mono MonoClone(const Mono *m) {
  return (Mono) {.p = PolyClone(&m->p), .exp = m->exp};
}

/**
 * Adds two polynomials.
 * @param[in] p : polynomial f$p@f$
 * @param[in] q : polynomial @f$q@f$
 * @return @f$p + q@f$
 */
Poly PolyAdd(const Poly *p, const Poly *q);

/**
 * Sums a list of monomials and forms a polynomial from them.
 * Takes ownership of the contents of the array @p monos.
 * @param[in] count : number of monomials
 * @param[in] monos : table of monomials
 * @return polynomial being the sum of monomials
 */
Poly PolyAddMonos(size_t count, const Mono monos[]);

/**
 * Multiplies two polynomials.
 * @param[in] p : polynomial @f$p@f$
 * @param[in] q : polynomial @f$q@f$
 * @return @f$p * q@f$
 */
Poly PolyMul(const Poly *p, const Poly *q);

/**
 * Returns the opposite polynomial.
 * @param[in] p : polynomial @f$p@f$
 * @return @f$-p@f$
 */
Poly PolyNeg(const Poly *p);

/**
 * Subtracts a polynomial from a polynomial.
 * @param[in] p : polynomial @f$p@f$
 * @param[in] q : polynomial @f$q@f$
 * @return @f$p - q@f$
 */
Poly PolySub(const Poly *p, const Poly *q);

/**
 * Returns the degree of a polynomial given the variable (-1 for a polynomial
 * identically equal to zero). Variables are indexed from 0.
 * Variable with index 0 means the main variable of this polynomial.
 * Larger indices denote the variables of the polynomials located
 * in coefficients.
 * @param[in] p : polynomial
 * @param[in] var_idx : variable index
 * @return degree of polynomial @p p due to the variable with index @p var_idx
 */
poly_exp_t PolyDegBy(const Poly *p, size_t var_idx);

/**
 * Returns the degree of a polynomial (-1 for a polynomial identically equal to zero).
 * @param[in] p : polynomial
 * @return degree of polynomial @p p
 */
poly_exp_t PolyDeg(const Poly *p);

/**
 * Checks the equality of two polynomials.
 * @param[in] p : polynomial @f$p@f$
 * @param[in] q : polynomial @f$q@f$
 * @return @f$p = q@f$
 */
bool PolyIsEq(const Poly *p, const Poly *q);

/**
 * Computes the value of a polynomial at a point @p x.
 * Inserts a value under the first variable of the polynomial @p x.
 * The result can be a polynomial if the coefficients are polynomials.
 * Then the variable indices in such a polynomial are decreased by one.
 * Formally for a polynomial @f$p(x_0, x_1, x_2, \ldots)@f$ the result is
 * polynomial @f$p(x, x_0, x_1, \ldots)@f$.
 * @param[in] p : polynomial @f$p@f$
 * @param[in] x : argument value @f$x@f$
 * @return @f$p(x, x_0, x_1, \ldots)@f$
 */
Poly PolyAt(const Poly *p, poly_coeff_t x);

/**
 * The function prints the polynomial.
 * @param[in] p : polynomial
 */
void PrintPoly(const Poly *p);

/**
 * Sums a list of monomials and forms a polynomial from them. Takes over ownership
 * the memory pointed to by @p monos and its contents. It can freely modify
 * the contents of this memory. We assume the memory pointed to by @p monos
 * has been allocated to the heap. If @p count or @p monos is zero
 * (NULL), creates a polynomial identically equal to zero.
 * @param[in] count : number of monomials
 * @param[in] monos : table of monomials
 * @return polynomial being the sum of monomials
 */
Poly PolyOwnMonos(size_t count, Mono *monos);

/**
 * Sums a list of monomials and forms a polynomial from them. Does not modify the content
 * array @p monos. If required, it makes complete copies of monomials
 * from the @p monos array. If @p count or @p monos is NULL,
 * creates a polynomial identically equal to zero.
 * @param[in] count : number of monomials
 * @param[in] monos : table of monomials
 * @return polynomial being the sum of monomials
 */
Poly PolyCloneMonos(size_t count, const Mono monos[]);

/**
 * The function exponentiates a polynomial, returns the result of the exponentiation.
 * @param[in] p : polynomial
 * @param[in] exp : power
 * @return polynomial
 */
Poly PolyExp(const Poly *p, poly_exp_t exp);

/**
 * The function returns a polynomial that is the result of polynomial composition.
 * Polynomial compoziton means substitution of the next varaibles by
 * polynomials in an array.
 * @param[in] p : polynomial
 * @param[in] k : array size
 * @param[in] q : array of polynomials
 * @return the resulting polynomial
 */
Poly PolyCompose(const Poly *p, size_t k, const Poly q[]);

#endif /* __POLY_H__ */
