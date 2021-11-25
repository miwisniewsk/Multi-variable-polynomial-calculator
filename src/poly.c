/** @file
  Implementation of functions declared in the poly.h file
  on rare polynomials of many variables

  @author Maja Wiśniewska <mw429666.students.mimuw.edu.pl>
  @date 2021
*/

#include "poly.h"
#include "mallocSafe.h"
#include <stdlib.h>

bool PolyIsZero(const Poly *p) {
    assert(p != NULL);

    if (PolyIsCoeff(p)) {
        if (p->coeff == 0) {
            return true;
        } else {
            return false;
        }
    } else {
        for (size_t i = 0; i < p->size; ++i) {
            if (!PolyIsZero(&(p->arr[i].p))) {
                return false;
            }
        }
        return true;
    }
}

void PolyDestroy(Poly *p) {
    assert(p != NULL);

    if (!PolyIsCoeff(p)) {
        for (size_t i = 0; i < p->size; ++i) {
            PolyDestroy(&(p->arr[i].p));
        }
        free(p->arr);
    }
}

/**
 * In the PolyCloneHelp function, I add a neq parameter so that I can use it for authoring
 * opposite polynomials. Copying polynomials I give neq = 1, and creating opposite neq = -1.
 * @param[in] p : polynomial
 * @param[out] r : polynomial
 * @param[in] neq : sign
 */
static void PolyCloneHelp(const Poly *p, Poly *r, int neq) {
    assert(p != NULL && r != NULL);

    if (PolyIsCoeff(p)) {
        *r = PolyFromCoeff(neq * p->coeff);
    } else {
        r->size = p->size;
        r->arr = mallocSafe(r->size * sizeof (Mono));

        for (size_t i = 0; i < r->size; ++i) {
            r->arr[i].exp = MonoGetExp(&(p->arr[i]));

            PolyCloneHelp(&(p->arr[i].p), &(r->arr[i].p), neq);
        }
    }
}

Poly PolyClone(const Poly *p) {
    assert (p != NULL);

    Poly r;

    PolyCloneHelp(p, &r, 1);

    return r;
}

/**
 * Function for comparing monomials.
 * @param[in] a : pointer
 * @param[in] b : pointer
 * @return sign
 */
static int compareMonos(const void * a, const void * b) {
    const Mono *pa = a;
    const Mono *pb = b;

    if (pa->exp > pb->exp) {
        return 1;
    } else if (pa->exp == pb->exp) {
        return 0;
    } else {
        return -1;
    }
}

/**
 * The PolyMonosClean function concatenates monomials with the same coefficients.
 * @param[in,out] r : polynomial
 */
static void PolyMonosClean(Poly *r) {
    assert(!PolyIsCoeff(r));

    qsort(r->arr, r->size, sizeof(Mono), compareMonos);

    for (size_t i = 0; i < r->size - 1; ++i) {
        if (MonoGetExp(&(r->arr[i])) == MonoGetExp(&(r->arr[i + 1]))) {
            poly_exp_t n = MonoGetExp(&(r->arr[i]));
            --r->size;

            Poly t = PolyAdd(&(r->arr[i].p), &(r->arr[i + 1].p));

            PolyDestroy(&(r->arr[i].p));
            PolyDestroy(&(r->arr[i + 1].p));

            r->arr[i].exp = n;
            r->arr[i].p = t;

            for (size_t j = i + 1; j < r->size; ++j) {
                r->arr[j] = r->arr[j + 1];
            }
            --i;
        }
    }
}

/**
 * The reductionToCoeff function simplifies the polynomials possible to constant polynomials,
 * e.g. 2 * x_0^0 * x_1^0 simplifies to 2.
 * @param[in,out] r : polynomial
 * @param[out] i : coefficient
 */
static void reductionToCoeff(Poly *r, poly_coeff_t *i) {
    if (!PolyIsCoeff(r)) {
        if (r->size == 1 && r->arr[0].exp == 0) {
            if (PolyIsCoeff(&(r->arr[0].p))) {
                *i = r->arr[0].p.coeff;
            } else {
                reductionToCoeff(&(r->arr[0].p), i);
            }
        }
    }
}

/**
 * The reduction function looks for polynomials that can be simplified and simplifies them.
 * @param[in,out] r : polynomial
 */
static void reduction(Poly *r) {
    if (!PolyIsCoeff(r)) {
        for (size_t i = 0; i < r->size; ++i) {
            reduction(&(r->arr[i].p));
        }
        poly_coeff_t i = 0;
        reductionToCoeff(r, &i);
        if (i != 0) {
            PolyDestroy(r);
            *r = PolyFromCoeff(i);
        }
    }
}

/**
 * PolyCleanZero function removes zeros from non-constant polynomial.
 * @param[in,out] r : polynomial
 */
static void PolyCleanZero(Poly *r) {
    assert(r != NULL);

    if (!PolyIsCoeff(r)) {
        for (size_t i = 0; i < r->size; ++i) {
            if (PolyIsZero(&(r->arr[i].p))) {
                MonoDestroy(&(r->arr[i]));
                --r->size;

                for (size_t j = i; j < r->size; ++j) {
                    r->arr[j] = r->arr[j + 1];
                }
            } else {
                PolyCleanZero(&(r->arr[i].p));
            }
        }
    }
}

/**
 * The PolyClean function looks for polynomials equal to constant polynomials
 * and simplifies them to constant polynomials if they were not, and removes the zeros.
 * @param[in,out] r : polynomial
 */
static void PolyClean(Poly *r) {
    if (PolyIsZero(r)) {
        PolyDestroy(r);
        *r = PolyZero();
    } else {
        PolyCleanZero(r);
    }

    reduction(r);
}

/**
 * The PolyAddHelp function checks which polynomials are constants and
 * passes them to the appropriate adding functions.
 * @param[in] p : polynomial
 * @param[in] q : polynomial
 * @param[out] r : polynomial
 */
static void PolyAddHelp(const Poly *p, const Poly *q, Poly *r);

/**
 * The oneCoeffAdd function adds a constant to the polynomial.
 * @param[in] p : polynomial
 * @param[out] r : polynomial
 * @param[in] c : coefficient
 */
static void oneCoeffAdd(const Poly *p, Poly *r, poly_coeff_t c) {
    assert(p != NULL && p->arr != NULL);

    if (PolyIsCoeff(&(p->arr[0].p)) && MonoGetExp(&(p->arr[0])) == 0) {
        *r = PolyClone(p);
        r->arr[0].p.coeff =  r->arr[0].p.coeff + c;
    } else {
        if (MonoGetExp(&(p->arr[0])) == 0) {
            r->size = p->size;
            r->arr = (Mono*) mallocSafe(r->size * sizeof (Mono));
            for (size_t i = 1; i < r->size; ++i) {
                r->arr[i] = MonoClone(&(p->arr[i]));
            }
            r->arr[0].exp = 0;
            oneCoeffAdd(&(p->arr[0].p), &(r->arr[0].p), c);
        } else {
            r->size = p->size + 1;
            r->arr = (Mono *) mallocSafe(r->size * sizeof(Mono));
            r->arr[0].p = PolyFromCoeff(c);
            r->arr[0].exp = 0;
            for (size_t i = 0; i < p->size; ++i) {
                r->arr[i + 1] = MonoClone(&(p->arr[i]));
            }
        }
    }
}

/**
 * The noCoeffAdd function adds two non-constant polynomials together.
 * @param[in] p : polynomial
 * @param[in] q : polynomial
 * @param[out] r : polynomial
 */
static void noCoeffAdd(const Poly *p, const Poly *q, Poly *r) {
    assert(p != NULL && q != NULL);

    r->arr = (Mono*) mallocSafe((p->size + q->size) * sizeof (Mono));

    size_t i = 0;
    size_t j = 0;
    size_t k = 0;

    while (i < p->size || j < q->size) {
        if (i == p->size) {
            r->arr[k] = q->arr[j];
            ++j;
        } else if (j == q->size) {
            r->arr[k] = p->arr[i];
            ++i;
        } else {
            if (MonoGetExp(&(p->arr[i])) < MonoGetExp(&(q->arr[j]))) {
                r->arr[k] = MonoClone(&(p->arr[i]));
                ++i;
            } else if (MonoGetExp(&(p->arr[i])) == MonoGetExp(&(q->arr[j]))) {
                r->arr[k].exp = MonoGetExp(&(p->arr[i]));
                PolyAddHelp(&(p->arr[i].p), &(q->arr[j].p), &(r->arr[k].p));
                ++i;
                ++j;
            } else {
                r->arr[k] = MonoClone(&(q->arr[j]));
                ++j;
            }
        }
        ++k;
    }
    r->size = k;
}

static void PolyAddHelp(const Poly *p, const Poly *q, Poly *r) {
    assert(p != NULL && q != NULL);

    if (PolyIsCoeff(p)) {
        if (PolyIsCoeff(q)) {
            *r = PolyFromCoeff(p->coeff + q->coeff);
        } else {
            oneCoeffAdd(q, r, p->coeff);
        }
    } else if (PolyIsCoeff(q)) {
        oneCoeffAdd(p, r, q->coeff);
    } else {
        noCoeffAdd(p, q, r);
    }
}

Poly PolyAdd(const Poly *p, const Poly *q) {
    Poly r;

    PolyAddHelp(p, q, &r);
    PolyClean(&r);

    return r;
}

Poly PolyAddMonos(size_t count, const Mono monos[]) {
    assert(count > 0 || monos != NULL);

    Poly r;
    r.arr = (Mono*) mallocSafe(count * sizeof (Mono));
    r.size = count;

    for (size_t i = 0; i < count; ++i) {
        r.arr[i] = monos[i];
    }

    PolyMonosClean(&r);
    PolyClean(&r);

    return r;
}

static void PolyMulHelp(const Poly *p, const Poly *q, Poly *r);

/**
 * The oneCoeffMul function multiplies the polynomial by a scalar, 
 * writes the result of the multiplication to the polynomial r
 * @param[in] p : polynomial
 * @param[in] q : coefficient
 * @param[out] r : polynomial
 */
static void oneCoeffMul(const Poly *p, poly_coeff_t q, Poly *r) {
    assert(p != NULL);

    if (!PolyIsCoeff(p)) {
        r->arr = (Mono*) mallocSafe(p->size * sizeof (Mono));
        r->size = p->size;

        for (size_t i = 0; i < p->size; ++i) {
            r->arr[i].exp = MonoGetExp(&(p->arr[i]));
            oneCoeffMul(&(p->arr[i].p), q, &(r->arr[i].p));
        }
    } else {
        *r = PolyFromCoeff(p->coeff * q);
    }
}

/**
 * The noCoeffMul function multiplies two non-property polynomials.
 * @param[in] p : polynomial
 * @param[in] q : polynomial
 * @param[out] r : polynomial
 */
static void noCoeffMul(const Poly *p, const Poly *q, Poly *r) {
    assert(p != NULL && q != NULL);

    r->size = p->size * q->size;
    r->arr = (Mono *) mallocSafe(r->size * sizeof(Mono));

    for (size_t i = 0; i < p->size; ++i) {
        for (size_t j = 0; j < q->size; ++j) {
            r->arr[i * q->size + j].exp = MonoGetExp(&(p->arr[i])) + MonoGetExp(&(q->arr[j]));
            PolyMulHelp(&(p->arr[i].p), &(q->arr[j].p), &(r->arr[i * q->size + j].p));
        }
    }

    PolyMonosClean(r);
}

/**
 * The PolyMulHelp function checks which polynomials are constants and
 * passes them to the appropriate multiplication functions.
 * @param[in] p : polynomial
 * @param[in] q : polynomial
 * @param[out] r : polynomial
 */
static void PolyMulHelp(const Poly *p, const Poly *q, Poly *r) {
    assert(p != NULL && q != NULL);

    if (PolyIsCoeff(p)) {
        if (PolyIsCoeff(q)) {
            *r = PolyFromCoeff(p->coeff * q->coeff);
        } else {
            oneCoeffMul(q, p->coeff, r);
        }
    } else {
        if (PolyIsCoeff(q)) {
            oneCoeffMul(p, q->coeff, r);
        } else {
            noCoeffMul(p, q, r);
        }
    }
}

Poly PolyMul(const Poly *p, const Poly *q) {
    assert(p != NULL && q != NULL);

    Poly r;

    PolyMulHelp(p, q, &r);

    PolyClean(&r);

    return r;
}

Poly PolyNeg(const Poly *p) {
    assert (p != NULL);

    Poly p2;

    PolyCloneHelp(p, &p2, -1);

    return p2;
}

Poly PolySub(const Poly *p, const Poly *q) {
    Poly t = PolyNeg(q);
    Poly w = PolyAdd(p, &t);

    PolyDestroy(&t);

    return w;
}

/**
 * The PolyDegByHelp function calculates the degree of the polynomial over a given variable by saving
 * the degree of a given monomial and checking if it is the greatest.
 * @param[in] p : polynomial
 * @param[in,out] exp_max : coefficient
 * @param[in,out] index : index
 * @param[in] var_idx : index
 */
static void PolyDegByHelp(const Poly *p, poly_exp_t *exp_max, size_t *index, size_t var_idx) {
    assert(p != NULL);

    ++*index;

    if (!PolyIsCoeff(p) && *index <= var_idx) {
        for (size_t i = 0; i < p->size; ++i) {
            if (*index == var_idx && MonoGetExp(&(p->arr[i])) > *exp_max) {
                *exp_max = MonoGetExp(&(p->arr[i]));
            }

            PolyDegByHelp(&(p->arr[i].p), exp_max, index, var_idx);
        }
    } else {
        --*index;
    }
}

poly_exp_t PolyDegBy(const Poly *p, size_t var_idx) {
    assert(p != NULL);

    if (PolyIsZero(p)) {
        return -1;
    }

    size_t index = -1;
    poly_exp_t exp_max = 0;

    PolyDegByHelp(p, &exp_max, &index, var_idx);

    return exp_max;
}

poly_exp_t PolyDeg(const Poly *p) {
    assert(p != NULL);

    if (!PolyIsCoeff(p)) {
        poly_exp_t exp_max = -1;
        for (size_t i = 0; i < p->size; ++i) {
            if (PolyDeg(&(p->arr[i].p)) + p->arr[i].exp > exp_max) {
                exp_max = PolyDeg(&(p->arr[i].p)) + p->arr[i].exp;
            }
        }

        return exp_max;
    } else {
        if (PolyIsZero(p)) {
            return -1;
        } else {
            return 0;
        }
    }
}

/**
* The PolyIsEqHelp function is a helper to PolyIsEq, has
 * additional bool type parameter initially set to true, which changes to
 * false when seeing any difference in the polynomials. This feature would consider
 * polynon 0 * x and 2x ^ 2 and 2x ^ 2 for different. However, the rest of the creating functions
 * polynomials do not allow such polynomials to arise.
 * @param[in] p : polynomial
 * @param[in] q : polynomial
 * @param[out] equal: Are they equal?
 */
static void PolyisEqHelp(const Poly *p, const Poly *q, bool *equal) {
    assert(p != NULL && q != NULL);

    if (PolyIsCoeff(p)) {
        if (!PolyIsCoeff(q)) {
            *equal = false;
        } else {
            if (p->coeff != q->coeff) {
                *equal = false;
            }
        }
    } else {
        if (PolyIsCoeff(q)) {
            *equal = false;
        } else {
            if (q->size != p->size) {
                *equal = false;
            } else {
                for (size_t i = 0; i < p->size; ++i) {
                    if (MonoGetExp(&(p->arr[i])) != MonoGetExp(&(q->arr[i]))) {
                        *equal = false;
                    }
                    PolyisEqHelp(&(p->arr[i].p), &(q->arr[i].p), equal);
                }
            }
        }
    }
}

bool PolyIsEq(const Poly *p, const Poly *q) {
    bool equal = true;

    PolyisEqHelp(p, q, &equal);

    return equal;
}

/**
 * The countMonos function counts how many monomials will arise after substituting a given value with x_0.
 * @param[in] p : polynomial
 * @return the number of monomials that will arise
 */
static size_t countMonos(const Poly *p) {
    size_t count = 0;

    for (size_t i = 0; i < p->size; ++i) {
        if (!PolyIsCoeff(&(p->arr[i].p))) {
            count = count + p->arr[i].p.size;
        } else {
            ++count;
        }
    }

    return count;
}

/**
 * The exponentiation function enhances the exponents.
 * @param[in] x : coefficient
 * @param[in] n : exponent
 * @return exponentiation result
 */
static poly_coeff_t exponentiation(poly_coeff_t x, poly_exp_t n) {
    poly_coeff_t wynik = 1;
    for (poly_coeff_t i = 0; i < n; ++i) {
        wynik = wynik * x;
    }
    return wynik;
}

/**
 * The function creates a list of monomials by substituting the given value with x_0.
 * @param[in,out] monos : table of monomials
 * @param[in] p : polynomial
 * @param[in] x : exponent
 */
static void createMonos(Mono *monos, const Poly *p, poly_coeff_t x) {
    size_t i = 0;
    size_t j = 0;

    while (i < p->size) {
        if (PolyIsCoeff(&(p->arr[i].p))) {
            monos[j].exp = 0;
            monos[j].p = PolyFromCoeff(p->arr[i].p.coeff * exponentiation(x,(p->arr[i].exp)));

            ++j;
            ++i;
        } else {
            for (size_t k = 0; k < p->arr[i].p.size ; ++k) {
                monos[j].exp = MonoGetExp(&(p->arr[i].p.arr[k]));

                Poly r = PolyFromCoeff(exponentiation(x, (p->arr[i].exp)));

                monos[j].p = PolyMul(&r,&(p->arr[i].p.arr[k].p));
                ++j;
            }
            ++i;
        }
    }
}

Poly PolyAt(const Poly *p, poly_coeff_t x) {
    assert(p != NULL);

    if (PolyIsCoeff(p)) {
        return *p;
    }

    size_t count = countMonos(p);

    Mono *monos = (Mono *) mallocSafe(count * sizeof(Mono));

    createMonos(monos, p, x);

    Poly r = PolyAddMonos(count, monos);

    free(monos);

    return r;
}

void PrintPoly(const Poly *p);

/**
 * The PrintMono function prints the monomial.
 * @param[in] m : monomial
 */
static void PrintMono(const Mono *m) {
    printf("(");
    PrintPoly(&(m->p));
    printf(",%d)", m->exp);
}

void PrintPoly(const Poly *p) {
    if (PolyIsCoeff(p)) {
        printf("%ld", p->coeff);
    } else {
        PrintMono(&(p->arr[0]));
        for (size_t i = 1; i < p->size; ++i) {
            printf("+");
            PrintMono(&(p->arr[i]));
        }
    }
}

Poly PolyOwnMonos(size_t count, Mono *monos) {
    if (count == 0) {
        if (monos != NULL) {
            free (monos);
        }
        return PolyZero();
    } else if (monos == NULL) {
        return PolyZero();
    } else {
        Poly r;
        r.arr = monos;
        r.size = count;

        PolyMonosClean(&r);
        PolyClean(&r);

        return r;
    }
}

Poly PolyCloneMonos(size_t count, const Mono monos[]) {
    if (count == 0 || monos == NULL) {
        return PolyZero();
    } else {
        Poly r;
        r.arr = (Mono*) (count * sizeof (Mono));
        r.size = count;

        for (size_t i = 0; i < count; ++i) {
            r.arr[i] = MonoClone(&(monos[i]));
        }

        PolyMonosClean(&r);
        PolyClean(&r);

        return r;
    }
}

Poly PolyExp(const Poly *p, poly_exp_t exp) {
    if (exp == 0) {
        return PolyFromCoeff(1);
    } else {
        Poly k = PolyExp(p, exp - 1);
        Poly r = PolyMul(p, &k);
        PolyDestroy(&k);
        return r;
    }
}

/**
 * The PolyAddPolos function adds all the polynomials in the array and
 * returns the result of this addition.
 * @param[in] k : array size
 * @param[in] q : array of polynomials
 * @return polynomial
 */
static Poly PolyAddPolos(size_t k, const Poly q[]) {
    Poly r = PolyZero();
    Poly t;

    for (size_t i = 0; 2 * i + 1 < k; ++i) {
        t = PolyAdd(&r, &(q[2 * i]));
        PolyDestroy(&r);
        r = PolyAdd(&t, &(q[2 * i + 1]));
        PolyDestroy(&t);
    }

    if (k % 2 == 0) {
        return r;
    } else {
        t = PolyAdd(&r, &(q[k - 1]));
        PolyDestroy(&r);
        return t;
    }
}

Poly PolyCompose(const Poly *p, size_t k, const Poly q[]) {
    assert (p != NULL);

    if (PolyIsCoeff(p)) {
        return *p;
    } else {
      
        Poly *polos = (Poly *) mallocSafe(p->size * sizeof(Poly));

        for (size_t j = 0; j < p->size; ++j) {
            if (k > 0) {
                Poly t = PolyCompose(&(p->arr[j].p), k - 1, &(q[1]));
                Poly r = PolyExp(&(q[0]), p->arr[j].exp);
            
                polos[j] = PolyMul(&r, &t); 
                
                PolyDestroy(&t);
                PolyDestroy(&r);  
            } else if (p->arr[j].exp == 0) {
                polos[j] = PolyCompose(&(p->arr[j].p), 0, &(q[0]));
            } else {
                polos[j] = PolyZero();
            }
        } 

        Poly q = PolyAddPolos(p->size, polos);

        for (size_t i = 0; i < p->size; ++i) {
            PolyDestroy(&(polos[i]));
        }
        free(polos);

        return q;
    }
}
