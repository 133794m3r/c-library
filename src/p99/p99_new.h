/* This may look like nonsense, but it really is -*- mode: C -*-             */
/*                                                                           */
/* Except of parts copied from previous work and as explicitly stated below, */
/* the author and copyright holder for this work is                          */
/* (C) copyright  2010-2012 Jens Gustedt, INRIA, France                      */
/*                                                                           */
/* This file is free software; it is part of the P99 project.                */
/* You can redistribute it and/or modify it under the terms of the QPL as    */
/* given in the file LICENSE. It is distributed without any warranty;        */
/* without even the implied warranty of merchantability or fitness for a     */
/* particular purpose.                                                       */
/*                                                                           */
#ifndef     P99_NEW_H_
# define    P99_NEW_H_

#include "p99_c99.h"

/**
 ** @file
 **
 ** @brief Macros for initialization and allocation
 ** @see preprocessor_initialization
 **/

/**
 ** @addtogroup preprocessor_initialization Initialization facilities throught the preprocessor
 **
 ** @{
 **/

P99_MACRO_END(dummy);

/**
 ** @brief Zero out all bits in the object that @a X points to.
 **
 ** @a X can be a pointer or an array
 ** @code
 ** T *A;
 ** T B[3];
 ** PZERO(A, 29);
 ** PZERO(B, 2);
 ** @endcode
 **
 ** Here the first call zeroes out 29 elements of type T starting at
 ** the memory where @c A points to. The second zeroes out the first
 ** two elements of the array @c B.
 **
 ** This is a macro not a function, but anyhow @a X is evaluated only
 ** once.
 ** @see P99_MEMZERO for a macro that initializes to a default value
 ** that is guaranteed to correspond to the type.
 **/
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_PZERO, 0)
#define P99_PZERO(X, N) (memset((X), 0, sizeof(X[0]) * N))

/**
 ** @brief Zero out all bits in the object @a X.
 **
 ** @a X must be an object from which the address can be taken, such
 ** as a normal variable or an array.
 **
 ** This is a macro not a function, but anyhow @a X is evaluated only
 ** once.
 ** @see P99_MEMZERO for a macro that initializes to a default value
 ** that is guaranteed to correspond to the type.
 **/
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_TZERO, 0)
#define P99_TZERO(X) (memset(&(X), 0, sizeof(X)))


p99_inline
void* p00_memset(void* p00_tar, void const* p00_src, size_t p00_size, size_t p00_nb) {
  register char *p00_p = p00_tar;
  for (size_t p00_i = 0; p00_i < p00_nb; ++p00_i, p00_p += p00_size)
    memcpy(p00_p, p00_src, p00_size);
  return p00_tar;
}

/**
 ** @brief A type oriented replacement for @c memset
 **
 ** Initialize the object to which @a TA points with @a N copies of
 ** object @a SO. Generally the idea is that @c TA is a pointer to an
 ** object of the type of @a SO.
 **
 ** This similar to @c memset with the extra feature that @a SO may be
 ** of any type.
 **
 ** @warning @c *TA must be large enough to hold the @a N copies,
 ** otherwise the behavior is undefined.
 **/
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_MEMSET, 1)
#define P99_MEMSET(TA, SO, N) p00_memset((TA), (void const*)&(SO), sizeof(SO), N)

/**
 ** @brief A type oriented replacement for @c memset with argument @c 0.
 **
 ** Initialize the object to which @a TA points with @a N copies of a
 ** default object of type @a T. Generally the idea is that @c TA is a
 ** pointer to an object of type @a T.
 **
 ** @warning @c *TA must be large enough to hold the @a N copies,
 ** otherwise the behavior is undefined.
 **/
P00_DOCUMENT_TYPE_ARGUMENT(P99_MEMZERO, 0)
#define P99_MEMZERO(T, TA, N) p00_memset((TA), (void const*)&P99_LVAL(const T), sizeof(T), N)

/**
 ** @}
 **/

/**
 ** @addtogroup preprocessor_allocation Allocation facilities throught the preprocessor
 **
 ** @{
 **/

/**
 ** @brief A type oriented @c malloc wrapper
 **
 ** This macro receives an
 ** expression that is evaluated for its size.
 ** @code
 ** double * a = P99_MALLOC(double[10]); // allocate an array of 10 double
 ** node * n = P99_MALLOC(node);         // allocate a new node
 ** @endcode
 ** @remark As for the C library routine the allocated space is
 ** uninitialized. Better use ::P99_CALLOC wherever possible. In
 ** particular as in the second example, when you might have a dynamic
 ** data structure with pointers.
 **/
#define P99_MALLOC(T) malloc(sizeof(T))

/**
 ** @brief A type oriented @c realloc wrapper
 **
 ** This macro receives an
 ** expression in @a T that is evaluated for its size as the second argument.
 ** @code
 ** double * a = P99_REALLOC(0, double[10]); // allocate an array of 10 double
 ** a = P99_REALLOC(a, double[20]);          // re-allocate an array of 20 double
 ** assert(a);                               // check if allocation was correct
 ** @endcode
 ** @remark As for the C library routine the allocated space is
 ** uninitialized.
 ** @remark In contrast to the library call @c realloc this macro can
 ** not be used as a replacement for @c free, since the second
 ** argument can not be tricked to result in a size of 0.
 **/
#define P99_REALLOC(X, T) realloc((X), sizeof(T))

p99_inline
void* p00_calloc(void const* p00_src, size_t p00_size, size_t p00_nb) {
  return p00_memset(malloc(p00_size*p00_nb), p00_src, p00_size, p00_nb);
}

#define P00_CALLOC0(T, N) p00_calloc((void const*)&P99_LVAL(const T), sizeof(T), N)
#define P00_CALLOC(...) P00_CALLOC0(__VA_ARGS__)

#ifdef P00_DOXYGEN
/**
 ** @brief A type oriented replacement for @c calloc
 **
 ** This similar to ::P99_MALLOC with the extra feature that the
 ** allocated space is initialized with the default as if the variable
 ** or array would have been declared static.
 **
 ** This macro receives one or two arguments. The first is a @em type
 ** expression that is evaluated for the size of the type and used to
 ** construct a default initializer for the type. The second is
 ** optional and controls how may objects of the type should be
 ** created.
 ** @code
 ** double * a = P99_CALLOC(double, 10); // allocate and initialize an array of 10 double
 ** node * n = P99_CALLOC(node);         // allocate and initialize one new node
 ** @endcode
 **
 ** Observe that here the @c double array @c a is initialized with the
 ** default initializer @c 0.0 for doubles. As on a given platform @c
 ** 0.0 might be represented differently than an all-zero-bit object
 ** this might or might not be different from using plain @c calloc.
 **
 ** For @c n the advantage might be that the type @c node has pointer
 ** fields. These then are correctly initialized to null pointers. As
 ** on a given platform null pointers might be represented differently
 ** than an all-zero-bit object this might or might not be different
 ** from using plain @c calloc.
 **/
P00_DOCUMENT_TYPE_ARGUMENT(P99_CALLOC, 0)
#define P99_CALLOC(T, N)
#else
P00_DOCUMENT_TYPE_ARGUMENT(P99_CALLOC, 0)
#define P99_CALLOC(...) P00_CALLOC(P99_CALL_DEFARG_LIST(P00_CALLOC, 2, __VA_ARGS__))
#endif

#define P00_CALLOC_defarg_1() 1

#define P00_NEW(T) P99_PASTE2(T, _init)(P99_MALLOC(T))

#define P00_NEW_ARGS(T, ...) P99_PASTE2(T, _init)(P99_MALLOC(T), __VA_ARGS__)


/**
 ** @brief Allocate an element of type @c T as given by the first
 ** argument and initialize it with the remaining arguments, if any.
 **
 ** This supposes that the argument list is of the form
 ** @code
 ** T [, OTHER_PARAMETERS]
 ** @endcode
 ** and that a function or macro named @c T_init exists where
 ** @c T_init is just the concatenation of the type name @c T and
 ** `_init'. Thus @c T must be a typename that consists of just one
 ** token: `signed' would work but `signed long' would not.
 **
 ** The @c T_init function must have a signature that is
 ** compatible to one of
 ** @code
 ** T* T_init(T* p);
 ** T* T_init(T*p, OTHER_PARAMETERS);
 ** @endcode
 **
 ** - it must accept a pointer to @c T as a first argument
 ** - it must be robust to the fact of receiving a null pointer through that
 **     first argument (indicating an error condition)
 ** - it should return the @em same pointer @c p
 ** - it may accept additional arguments which may then be passed
 **     through from the call to ::P99_NEW
 **
 ** @return value is a pointer to a newly allocated and initialized object of type
 ** @c T, or @c (T*)0 if the allocation failed.
 **
 ** @see P99_CALL_DEFARG if you want to provide default arguments to
 ** the @c T_init function.
 **/
#define P99_NEW(...) P99_IF_LT(P99_NARG(__VA_ARGS__), 2)(P00_NEW(__VA_ARGS__))(P00_NEW_ARGS(__VA_ARGS__))

#ifdef P00_DOXYGEN
/**
 ** @brief Declare a `delete' operator for type @a T.
 **
 ** This supposes that type @a T has a `destructor', i.e a destroy
 ** function, that just takes a pointer to the element that is to be
 ** initialized.
 **
 ** Other arguments after the type argument are interpreted as storage
 ** class specifier for the functions. Default is ::p99_inline.
 **/
P00_DOCUMENT_TYPE_ARGUMENT(P99_DECLARE_DELETE, 0)
#define P99_DECLARE_DELETE(T)                                           \
/*! @brief Operator @c delete for type T   **/                          \
/*! @attention @ref T ## _destroy  is supposed to exist and to be callable with just one T * argument **/ \
/*! @attention @a el show have been allocated through P99_NEW */        \
/*! @see P99_NEW */                                                     \
/*! @memberof T */                                                      \
  void P99_PASTE2(T, _delete)(T const*p00_el) { }

#define P99_DEFINE_DELETE(T) P99_INSTANTIATE(void, P99_PASTE2(T, _delete), T const*)
#else
P00_DOCUMENT_TYPE_ARGUMENT(P99_DECLARE_DELETE, 0)
#define P99_DECLARE_DELETE(...)                                \
P99_IF_LT(P99_NARG(__VA_ARGS__), 2)                            \
(P00_DECLARE_DELETE(__VA_ARGS__, p99_inline))                  \
(P00_DECLARE_DELETE(__VA_ARGS__))                              \
P99_MACRO_END(P99_DECLARE_DELETE)
#define P99_DEFINE_DELETE(...) P00_DEFINE_DELETE(__VA_ARGS__,)
#endif

#define P00_DECLARE_DELETE(T, ...)                                                                          \
__VA_ARGS__                                                                                                 \
void P99_PASTE2(T, _delete)(T const*p00_el) {                                                               \
  if (p00_el) {                                                                                             \
    T* p00_e = (T*)p00_el;                                                                                  \
    P99_PASTE2(T, _destroy)(p00_e);                                                                         \
    free((void*)p00_e);                                                                                     \
  }                                                                                                         \
}

#define P00_DEFINE_DELETE(T, ...) P99_INSTANTIATE(void, P99_PASTE2(T, _delete), T const*)

p99_inline
size_t p99_maxof(size_t p00_m, size_t p00_n) {
  return p00_m < p00_n ? p00_n : p00_m;
}

P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_MINOF, 0)
P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_MINOF, 1)
#define P99_MAXOF(A, B) ((A) < (B) ? (B) : (A))

P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_MAXOF, 0)
P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_MAXOF, 1)
#define P99_MINOF(A, B) ((A) < (B) ? (A) : (B))

#define P00_SIZEOF2(T, ...) sizeof(P99_TOKJOIN(., ((const T){0}), __VA_ARGS__))

#ifdef P00_DOXYGEN
/**
 ** @brief Determine the size of field @a F in structure @a T.
 **
 ** Additional parameters in the argument list are joined to @a F with dots.
 **/
#define P99_SIZEOF(T, F, ...) P99_IF_EQ(P99_NARG(__VA_ARGS__), 1)(sizeof(__VA_ARGS__))(P00_SIZEOF2(__VA_ARGS__))
#else
#define P99_SIZEOF(...) P99_IF_EQ(P99_NARG(__VA_ARGS__), 1)(sizeof(__VA_ARGS__))(P00_SIZEOF2(__VA_ARGS__))
#endif

/**
 ** @addtogroup flexible Flexible array members
 **
 ** C99 allows to define a flexible array member as the last member of
 ** a @c struct, namely an array of undetermined length.
 ** @code
 ** P99_DECLARE_STRUCT(package_head);
 ** struct package_head {
 **   char name[20];
 **   size_t len;
 **   uint64_t data[];
 ** };
 ** @endcode
 ** Such a @c struct can then allocated on the heap with a suitable
 ** size such that the field @c data has as much elements as fit in
 ** the allocated space from the start of @c data onward. Usually one
 ** would allocate such @c struct with
 ** @code
 ** package_head *a = malloc(sizeof(package_head) + 10 * sizeof(uint64_t));
 ** package_head *b = malloc(sizeof(*b) + 12 * sizeof(b->data[0]));
 ** @endcode
 ** This has several disadvantages. First the syntax is clumsy, we
 ** have to use a relatively complicated expression that uses two
 ** elements of the specification of @c a or @c b.
 **
 ** Then this is wasting space. Due to packing of the @c struct the
 ** offset of @c data "inside" the @c struct may be less than
 ** <code>sizeof(package_head)</code>. In most cases the real size of
 ** the object that we want to construct is
 ** @code
 ** offsetof(package_head, data) + N * sizeof(uint64_t)
 ** @endcode
 ** so we are wasting
 ** @code
 ** sizeof(package_head) - offsetof(package_head, data)
 ** @endcode
 ** bytes.
 **
 ** The above formula for the exact size is only valid for larger
 ** values of @c N. We must also ensure that we allocate at least @c
 ** sizeof(package_head) bytes. So the complete formula for looks
 ** something like
 ** @code
 ** #define P99_FSIZEOF(T, F, N) P99_MAXOF(sizeof(T), offsetof(T, F) + P99_SIZEOF(T, F[0]) * N)
 ** @endcode
 ** which is probably not something that you want to write on a daily base.
 **
 ** We provide several interfaces to allocate @c struct with flexible members
 ** @see P99_FCALLOC
 ** @see P99_FMALLOC
 ** @see P99_FREALLOC
 ** @{
 **/

/**
 ** @def P99_FHEAD
 ** @brief For a pointer @a P to a flexible @c struct member @a F in
 ** @c struct T find the start address of the container.
 **/
P00_DOCUMENT_TYPE_ARGUMENT(P99_FHEAD, 0)
#define P99_FHEAD(T, F, P) ((T*)(((char*)P) - offsetof(T, F)))

/**
 ** @def P99_FSIZEOF
 ** @brief Compute the size for an instance of @c struct T that is
 ** able to hold @a N items in flexible @c struct member @a F.
 **/
P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_FSIZEOF, 2)
P00_DOCUMENT_TYPE_ARGUMENT(P99_FSIZEOF, 0)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_FSIZEOF, 1)
#define P99_FSIZEOF(T, F, N) P99_MAXOF(sizeof(T), offsetof(T, F) + P99_SIZEOF(T, F[0]) * N)
#define P00_FSIZEOF(T, F, M) p99_maxof(sizeof(T), offsetof(T, F) + M)

#define P00_FREALLOC(P, T, F, M) realloc(P, P00_FSIZEOF(T, F, M))
/**
 ** @def P99_FREALLOC
 ** @brief Reallocate an instance @a P of @c struct T such that it is
 ** able to hold @a N items in flexible @c struct member @a F.
 **
 ** No initialization of the instance is performed.
 **
 ** This behaves analogous to @c realloc. In particular, the object
 ** can be moved to a different location, and therefore the return
 ** value must always be checked.
 **/
P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_FREALLOC, 3)
P00_DOCUMENT_TYPE_ARGUMENT(P99_FREALLOC, 1)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_FREALLOC, 2)
#define P99_FREALLOC(P, T, F, N) realloc(P, P99_FSIZEOF(T, F, N))

/**
 ** @def P99_FMALLOC
 ** @brief Allocate an instance of @c struct T that is able to hold @a
 ** N items in flexible @c struct member @a F.
 **
 ** No initialization of the instance is performed.
 **/
P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_FMALLOC, 2)
P00_DOCUMENT_TYPE_ARGUMENT(P99_FMALLOC, 0)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_FMALLOC, 1)
#define P99_FMALLOC(T, F, N) malloc(P99_FSIZEOF(T, F, N))
#define P00_FMALLOC(T, F, M) malloc(P00_FSIZEOF(T, F, M))
/**
 ** @def P99_FCALLOC
 ** @brief Allocate an instance of @c struct T that is able to hold @a
 ** N items in flexible @c struct member @a F
 **
 ** Initialization of the instance is performed as for @c calloc.
 **/
P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_FCALLOC, 2)
P00_DOCUMENT_TYPE_ARGUMENT(P99_FCALLOC, 0)
P00_DOCUMENT_PERMITTED_ARGUMENT(P99_FCALLOC, 1)
#define P99_FCALLOC(T, F, N) calloc(P99_FSIZEOF(T, F, N),1)
#define P00_FCALLOC(T, F, M) calloc(P00_FSIZEOF(T, F, M),1)

/**
 ** @}
 **/

/**
 ** @}
 **/

#endif      /* !P99_NEW_H_ */
