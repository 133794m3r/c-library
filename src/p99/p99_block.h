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
#ifndef     P99_BLOCK_H_
# define    P99_BLOCK_H_

#include "p99_int.h"

/**
 ** @file
 ** @brief Macros that implement controlling blocks.
 ** @see preprocessor_blocks
 **/


/**
 ** @addtogroup preprocessor_blocks Preprocessor BLocks
 ** @brief This defines some macros that can be used to ensure that
 ** certain operations are always executed before entering and after
 ** leaving a particular block.
 ** @{
 **/

#ifndef P00_DOXYGEN
#define P00_ROBUST(...) __VA_ARGS__

#define P00 P99_FILEID(blk)

#define P00_BLK_GEN(BEFORE, COND, ...) for (P00_ROBUST(BEFORE); (COND) && P00; (__VA_ARGS__), P00 = 0)
#define P00_BLK_BEFORE(...) for (__VA_ARGS__; P00; P00 = 0)
#define P00_BLK_AFTER(...) P00_BLK_BEFAFT(, (__VA_ARGS__))
#define P00_BLK_CONDITIONAL(...) for (; (__VA_ARGS__) && P00; P00 = 0)
#define P00_BLK_START P00_BLK_BEFORE(_Bool P00 = 1)
#define P00_BLK_END P00_BLK_BEFORE()

/* The idea of this macro is that it evaluates INIT before the
   declaration of NAME. Therefore we may then push a variable on the
   stack with the same name as a variable of an enclosing scope, and
   use its value of inside the expression. */
#define P00_BLK_DECL_REC(TYPE, NAME, ...)                                  \
P00_BLK_BEFORE(TYPE P99_PASTE2(p00_decl_, NAME) = P00_ROBUST(__VA_ARGS__)) \
P00_BLK_BEFAFT(TYPE NAME = P99_PASTE2(p00_decl_, NAME), (void)NAME)

/* If no initializer is given, the variable is initialized by
   default. */
#define P00_BLK_DECL(...)                                      \
P99_IF_EQ_2(P99_NARG(__VA_ARGS__))                             \
(P00_BLK_DECL_2(__VA_ARGS__))                                  \
(P00_BLK_DECL_3(__VA_ARGS__))

#define P00_BLK_DECL_3(TYPE, NAME, ...) P00_BLK_BEFAFT(TYPE NAME = __VA_ARGS__, (void)NAME)

#define P00_BLK_DECL_2(TYPE, NAME) P00_BLK_DECL_3(TYPE, NAME, P99_INIT)

/* Declare @a NAME to be a pointer to a static variable of type @a
 * TYPE for the depending block. */
#define P00_BLK_DECL_STATIC(TYPE, NAME, ...)                           \
P00_BLK_BEFORE(TYPE* NAME = 0)                                         \
P99_PREFER(                                                            \
  static TYPE P99_PASTE2(p00_static_, NAME) = P00_ROBUST(__VA_ARGS__); \
  NAME = &P99_PASTE2(p00_static_, NAME);                               \
  goto P99_FILEID(p00_label_, NAME); ) P99_FILEID(p00_label_, NAME):

#endif

/**
 ** @addtogroup validity Checking code validity
 ** @{
 **/

#define P00_INHIBIT(NAME, EXT) P99_PASTE3(p00_inhibit_, NAME, EXT)


/**
 ** @brief Declare a feature @a NAME that can be compile time
 ** inhibited or allowed in certain parts of code.
 **
 ** @see P99_INHIBIT
 ** @see P99_ALLOW
 ** @see P99_INHIBIT_CHECK
 ** @see P99_CHECK_RETURN for an example
 **/
#define P99_DECLARE_INHIBIT(NAME) enum { P00_INHIBIT(NAME,) = 0 }

/**
 ** @brief Error out at compile time if @a NAME is inhibited.
 **
 ** @see P99_DECLARE_INHIBIT
 **/
#define P99_INHIBIT_CHECK(NAME)                                         \
switch (P99_STRINGIFY(P00_INHIBIT(NAME,))[P00_INHIBIT(NAME,)]) default:


/**
 ** @brief Inhibit the use of feature @a NAME inside the dependent code.
 **
 ** @see P99_DECLARE_INHIBIT
 ** @see P99_ALLOW
 **/
#define P99_INHIBIT(NAME)                                                              \
P00_BLK_START                                                                          \
for (unsigned const*const P00_INHIBIT(NAME,) = 0; !P00_INHIBIT(NAME,) && P00; P00 = 0)


/**
 ** @brief Allow the use of feature @a NAME inside the dependent code.
 **
 ** @see P99_DECLARE_INHIBIT
 ** @see P99_INHIBIT
 **/
#define P99_ALLOW(NAME)                                        \
P00_BLK_START                                                  \
P00_BLK_DECL(unsigned const, P00_INHIBIT(NAME,), 0)


P99_DECLARE_INHIBIT(RETURN);

#if defined(P99_CHECK_RETURN) && !defined(P00_DOXYGEN)
#define return P99_INHIBIT_CHECK(RETURN) return
#endif

#ifdef P00_DOXYGEN
/**
 ** @brief Insert code checks for bare @c return statements inside
 ** ::P99_UNWIND_PROTECT
 **
 ** This check is not on by default, you should only enable it during
 ** development via, e.g, a <code>-DP99_CHECK_RETURN</code> compiler
 ** option.
 **/
#define P99_CHECK_RETURN
#endif

/** @}
 **/


#define P00_BLK_BEFAFT(BEFORE, ...)                            \
P99_INHIBIT(RETURN)                                            \
P00_BLK_GEN(P00_ROBUST(BEFORE), true, __VA_ARGS__)

#ifdef P00_DOXYGEN
/**
 ** @brief A meta-macro to protect a dependent block or statement by
 ** statement @a BEFORE that is executed before the block and @a AFTER
 ** that is executed afterward.
 **
 ** Preliminary exit of the block is possible with @c break or @c continue.
 **
 ** @warning @c return, @c exit() or other functions that don't return
 ** to the caller inside the dependent block will result in not
 ** executing @a AFTER, so be careful.
 **
 ** An example of a potential use is
 ** @code
 ** #define P99_INVARIANT(EXPR) P99_PROTECTED_BLOCK(assert((EXPR) && "failed on entry"), assert((EXPR) && "failed on leave"))
 ** @endcode
 **
 ** Such a construct may then be used like this
 ** @code
 ** P99_INVARIANT(x > 5) {
 **   // do something with x that changes it but ensures that
 **   // it will be strictly less than 5 at the end
 ** }
 ** @endcode
 **
 ** and this would trigger an assertion whenever the condition is not
 ** fulfilled when entering or leaving the block.
 **/
#define P99_PROTECTED_BLOCK(BEFORE, AFTER)
#else
#define P99_PROTECTED_BLOCK(BEFORE, ...)                       \
P00_BLK_START                                                  \
P00_BLK_BEFAFT(P00_ROBUST(BEFORE), __VA_ARGS__)                \
/* Ensure that a `break' will still execute AFTER */           \
P00_BLK_END
#endif

#ifdef P00_DOXYGEN
/**
 ** @brief A meta-macro to protect a dependent block or statement by
 ** a guard variable @a NAME of type @a T.
 **
 ** The guard variable is initialized with expression @a INITIAL and
 ** statement @a BEFORE and @a AFTER are executed before and after the
 ** block, respectively.
 **
 ** Preliminary exit of the block is possible with @c break or @c continue.
 **
 ** @warning @c return, @c exit() or other functions that don't return
 ** to the caller inside the dependent block will result in not
 ** executing @a AFTER, so be careful. Use ::P99_UNWIND_RETURN instead.
 **
 ** An example of a potential use would be a pointer to a mutex
 ** variable. It can be initialized with the address of a mutex as
 ** argument to @a INITIAL. @a BEFORE and @a AFTER may then lock resp
 ** unlock that mutex.
 **
 ** @see P99_UNWIND_PROTECT is used internally to protect the
 ** execution of @a AFTER
 ** @see P99_UNWIND to break through one or several nested guarded blocks
 ** @see P99_UNWIND_RETURN to return from the enclosing function
 **/
#define P99_GUARDED_BLOCK(T, NAME, INITIAL, BEFORE, AFTER)
#else
P00_DOCUMENT_WARN_VLA_ARGUMENT(P99_GUARDED_BLOCK, 0)
P00_DOCUMENT_DECLARATION_ARGUMENT(P99_GUARDED_BLOCK, 1)
P00_DOCUMENT_STATEMENT_ARGUMENT(P99_GUARDED_BLOCK, 4)
#define P99_GUARDED_BLOCK(T, NAME, INITIAL, BEFORE, AFTER)     \
P00_BLK_START                                                  \
P00_BLK_DECL_REC(T, NAME, P00_ROBUST(INITIAL))                 \
P99_UNWIND_PROTECT if (0) { P99_PROTECT: AFTER; } else         \
P00_BLK_BEFAFT(P00_ROBUST(BEFORE), AFTER)                      \
/* Ensure that a `break' will still execute AFTER */           \
P00_BLK_END
#endif

/**
 ** @brief Do nothing
 **
 ** This may be placed everywhere where a C statement can be placed.
 **/
#define P99_NOP ((void)0)

/**
 ** @brief Prefer the statements in the argument list over the
 ** statement or block that follows.
 **
 ** The argument list should consist of a possibly empty list of
 ** statements, exactly as you would put them inside a <code>{
 ** ... }</code>. There is no restriction on the statements that you
 ** may put in the list: @c break, @c continue, @c return or @c goto
 ** should work as expected, and the list may even contain commas.
 **
 ** The dependent statement or block will in general not be executed
 ** unless they contain a valid jump target that is jumped to. Such a
 ** jump target may be a normal label or a case label.
 **
 ** @code
 ** char *str = malloc(25);
 ** if (!str) goto CLEANUP;
 ** .
 ** .
 ** P99_PREFER(fprintf(stderr, "Happy: all allocation went well!\n");)
 **   CLEANUP: {
 **      // Do some repair work and exit gracefully
 **      fprintf(stderr, "Unhappy: something went wrong!\n");
 **   }
 ** @endcode
 ** @see P99_AVOID
 **/
#if p99_has_feature(statement_expression)
# define P99_PREFER(...) /* avoid the dangling else problem */    \
for (_Bool p00 = 1; p00 && p99_extension ((void)({ __VA_ARGS__ }), 1); p00 = 0)
#else
# define P99_PREFER(...) if (1) { __VA_ARGS__ } else
#endif

/**
 ** @brief Only execute the depending statement or block if it is
 ** jumped into explicitly from the outer block.
 **
 ** This can be used to comment out code temporarily on source
 ** level. This macro is preferable over the common <code>if
 ** (0)</code> dialect that is used for the same purpose, since it has
 ** no problem with a dangling @c else.
 **
 ** This can also be used to handle some exceptional cases to which
 ** you want to jump to explicitly, either by a @c goto or as a @c
 ** switch @c case.
 **
 ** With this the example from ::P99_PREFER reads simply
 ** @code
 ** char *str = malloc(25);
 ** if (!str) goto CLEANUP;
 ** .
 ** .
 ** P99_AVOID
 **   CLEANUP: {
 **     // Do some repair work and exit gracefully
 **     fprintf(stderr, "Unhappy: something went wrong!\n");
 **   }
 ** @endcode
 ** @see P99_PREFER
 **/
#define P99_AVOID for (;0;)

/**
 ** @brief Execute the statements in the argument list.
 **
 ** This is to have several statements executed in a place where
 ** syntactically only one statement (and not a <code>{ ... }</code>
 ** block) is allowed.
 **
 ** The argument list should consist of a possibly empty list of
 ** statements, exactly as you would put them inside a <code>{
 ** ... }</code>. There is no restriction on the statements that you
 ** may put in the list: @c break, @c continue, @c return or @c goto
 ** should work as expected, and the list may even contain commas.
 **
 ** Traditionally this would be done with a construction like
 ** @code
 ** do { __VA_ARGS__ } while(0)
 ** @endcode
 **
 ** That traditional construction changes the control flow in that @c
 ** break and @c continue statements would change their meaning inside
 ** the list.
 **
 ** @see P99_PREFER
 ** @see P99_AVOID
 **/
#define P99_BLOCK(...) P99_PREFER(__VA_ARGS__) P99_NOP

/**
 ** @brief An exclusive @c case for a @c switch statement
 **
 ** This @c case will @em only be executed when the @c switch value is
 ** triggered and not if we fall through from a previous case.
 **
 ** @code
 ** switch(errno) {
 **  case 0: break; // everything works fine
 **  P99_XDEFAULT : {
 **    fprintf(stderr, "AUTSCH: call to schnoeck failed with unhandled case!\n");
 **    perror("AUTSCH");
 **  }
 **  P99_XCASE ENOMEM :
 **    fprintf(stderr, "Autsch: call to schnoeck didn't have enough memory!\n");
 **  severe_error_occured = true;
 **  P99_XCASE EINTR : {
 **    fprintf(stderr, "Autsch: call to schnoeck was interrupted!\n");
 **    // do something else in that case
 **  }
 **  // common clean up code comes here
 **  errno = 0;
 ** }
 ** @endcode
 **
 ** Please observe that these macros prefix exactly one statement or
 ** block and not a series of statements as a normal @c case would
 ** do and none of them has the semantic of a @c break. In particular:
 ** - @c EINTR and the default cases have <code>{ .. }</code> around their statements
 **   such that they may execute several statements.
 ** - The @c severe_error_occured assignment is executed for the
 **   default and @c ENOMEM case.
 ** - The final cleanup code is executed by everybody but <code>case
 **   0:</code>.
 **/
#define P99_XCASE P99_AVOID case

/**
 ** @brief The default case analogous to ::P99_XCASE
 **/
#define P99_XDEFAULT P99_AVOID default

typedef enum p00_uncase_enum {
  p00_uncase = 0,
} p00_uncase_enum;


#define P00_UNCASE switch((p00_uncase_enum)0) P99_XCASE 0

/**
 ** @brief A command prefixed with this cannot be a case target from
 ** surrounding scopes.
 **
 ** Actually this might not produce errors but just spoofy warnings,
 ** but well then you have been warned...
 **/
#define P99_UNCASE P00_UNCASE :

/**
 ** @brief Handle and reset @c errno.
 **
 ** This is will inspect @c errno (which is expensive) exactly once.
 ** If @c errno is @c 0, it will do nothing as efficient as
 ** possible. Otherwise it will execute the dependent block much as a
 ** @c switch statement:
 **
 ** @code
 ** P99_HANDLE_ERRNO {
 **   fprintf(stderr, "The compiler should tell us that this fprintf here is unreachable.\n");
 **   P99_XCASE EINTR : {
 **     fprintf(stderr, "Autsch: call to schnoeck was interrupted!\n");
 **     // do something else in that case
 **   }
 **   P99_XCASE ENOMEM :
 **       fprintf(stderr, "Autsch: call to schnoeck didn't have enough memory!\n");
 **   P99_XDEFAULT : {
 **       fprintf(stderr, "AUTSCH: call to schnoeck failed with unhandled case!\n");
 **       perror("AUTSCH");
 **     }
 **   fprintf(stderr, "We are doing some common cleanup for the errno handling code.\n");
 **   }
 ** @endcode
 **/
#define P99_HANDLE_ERRNO                                       \
P00_BLK_START                                                  \
P00_BLK_DECL(int const, p99_errno, errno)                      \
  switch (P99_UNLIKELY(!!p99_errno)) case true:                \
    P99_UNWIND_PROTECT                                         \
      if (false) { P99_PROTECT: errno = 0; } else              \
        P00_BLK_AFTER(errno = 0)                               \
          switch (p99_errno) case 0:


enum p99_unwind {
  /**
   ** @brief The level of nesting ::P99_UNWIND_PROTECT
   **
   ** This will always be accessible as read-only constant and taking
   ** an address of it will produce an error.
   **/
  p99_unwind_level = 0,
  p00_unwind_top = 0,
  p00_unwind_prev = 0,
  p00_unwind_bottom = 0,
  /**
   ** @brief The code an eventual call to ::P99_UNWIND
   **
   ** This will be @c 0 if no ::P99_UNWIND is in progress. Otherwise a
   ** positive number tells how many levels will at most be unwound. A
   ** negative value will cause the whole ::P99_UNWIND_PROTECT level
   ** to traversed. This same negative value is visible on all levels
   ** through the execution of the protected parts.
   **
   ** This will always be accessible as read-only constant and taking
   ** an address of it will produce an error.
   **/
  p99_unwind_code = 0,
  p99_unwind_return = INT_MAX,
};


P99_DECLARE_STRUCT(p00_jmp_buf0);

struct p00_jmp_buf0 {
  p00_jmp_buf0 * p99_lifo;
  bool volatile p00_returning;
  jmp_buf p00_buf;
};

#define P00_JMP_BUF0_INITIALIZER { .p00_returning = 0, /* initialize .p00_buf implicitly */ }

typedef p00_jmp_buf0 p00_jmp_buf[1];

#define P00_JMP_BUF_INITIALIZER { [0] = P00_JMP_BUF0_INITIALIZER }

P99_DECLARE_STRUCT(p00_inhibitor);

struct p00_inhibitor {
  int p00_a;
};

p99_inline
noreturn
void p00_longjmp(p00_jmp_buf0 * p00_buf, int p00_val) {
  longjmp(p00_buf->p00_buf, p00_val);
}

/**
 ** @brief Unwind execution from several levels of nesting inside a
 ** function.
 **
 ** This macro allows to safely unwind several levels of block
 ** statements.
 **
 ** @code
 ** P99_UNWIND_PROTECT {
 **   // do something
 **   while (cond0) {
 **     for (;cond1;) {
 **        if (cond2) P99_UNWIND(-1);
 **     }
 **   }
 **  P99_PROTECT :
 **   // do some cleanup here
 **   // if everything went well ::p99_unwind_code has value 0 otherwise it
 **   // receives a value from P99_UNWIND
 ** }
 ** @endcode
 **
 ** Here ::P99_UNWIND terminates the execution of the inner blocks and
 ** resumes execution at the special "label" ::P99_PROTECT, if
 ** present.
 **
 ** The argument to ::P99_UNWIND controls how many levels are
 ** unwound. If it is positive at most that number of level is
 ** unwound, but never more levels than there are on the call stack of
 ** the enclosing function. If it is negative, all levels on the stack
 ** of the enclosing function are unwound and during that process this
 ** negative value that is passed to ::P99_UNWIND is accessible
 ** through ::p99_unwind_code.
 **
 ** @warning Variables that are modified before the call to
 ** ::P99_UNWIND are only guaranteed to have their new value in the
 ** protected part if they are declared @c volatile.
 **
 ** @warning There should be no plain @c return statement in the
 ** depending block before the ::P99_PROTECT label.
 **
 ** @see "test-p99-block.c" for a more sophisticated example of nested
 ** ::P99_UNWIND_PROTECT.
 ** @see ::P99_UNWIND
 ** @see ::P99_UNWIND_RETURN for a replacement of @c return that
 **      respects the protected parts
 ** @see ::P99_CHECK_RETURN to check for suspicious bare @c return statements
 ** @see ::p99_unwind_code
 ** @see ::p99_unwind_level
 **/
#define P99_UNWIND_PROTECT                                                                       \
P00_BLK_START                                                                                    \
/* The jump buffer for the case that P99_UNWIND_RETURN is launched                               \
   inside this construct. Only the lowest level variable will be                                 \
   needed but since we don't know our level yet, we have to declare                              \
   this at every level. */                                                                       \
P00_BLK_BEFAFT(auto p00_jmp_buf p00_unwind_return = P00_ROBUST(P00_JMP_BUF_INITIALIZER),         \
               /* returning can only be on because it was set by                                 \
                  P99_UNWIND_RETURN and we fell of the edge after all                            \
                  P99_PROTECT clauses */                                                         \
               p00_unwind_return[0].p00_returning ? p00_longjmp(p00_unwind_return, 1) : P99_NOP) \
/* Detect the bottom of enclosing other P99_UNWIND_PROTECT. If it                                \
   exists set it to that one, otherwise set it to our newly allocated                            \
   jump buffer. We have to do this in two steps such that the                                    \
   initializer of the variable is not the variable itself. */                                    \
P00_BLK_DECL_REC(register p00_jmp_buf *const, p00_unwind_bottom,                                 \
             (p00_unwind_bottom ? p00_unwind_bottom : &p00_unwind_return))                       \
  /* inhibit further access of the p00_unwind_return variable */                                 \
  P00_BLK_DECL(register p00_inhibitor const, p00_unwind_return, { INT_MAX })                     \
/* are we unwinding or not? */                                                                   \
  P00_BLK_DECL(register _Bool, p00_unw)                                                          \
/* The return code from the longjmp to which we apply the                                        \
   special convention concerning the value 0. */                                                 \
  P00_BLK_BEFAFT(register int p00_code = 0,                                                      \
                 /* An eventual continuation of the unwind process is                            \
                    decided here, since here the p00_unwind_top                                  \
                    variable that is visible is that of the enclosing                            \
                    scope, but the unwind code variable is ours.  If                             \
                    the enclosing scope is the outer scope,                                      \
                    p00_unwind_top is a integer with value zero. So                              \
                    even then the P99_UNWIND is syntactically correct,                           \
                    but fortunately the underlying call to longjmp                               \
                    will not be issued. */                                                       \
                 (p00_unw                                                                        \
                  ? P99_UNWIND(p00_code < 0 ? p00_code : p00_code - 1)                           \
                  : P99_NOP))                                                                    \
/* maintain the level of nesting of different calls to                                           \
   P99_UNWIND_PROTECT */                                                                         \
  P00_BLK_DECL_REC(register unsigned const, p99_unwind_level, p99_unwind_level + 1)              \
  P00_BLK_DECL(p00_jmp_buf0*, p00_unwind_prev, p00_unwind_top)                                   \
/* the buffer variable for setjmp/longjump */                                                    \
  P00_BLK_DECL(auto p00_jmp_buf, p00_unwind_top, P99_INIT)                                       \
  P00_BLK_BEFORE(p00_code = setjmp(p00_unwind_top[0].p00_buf))                                   \
  /* detect whether or not we are unwinding */                                                   \
  P00_BLK_BEFORE(p00_unw = !!p00_code)                                                           \
  P00_BLK_DECL(register int const, p99_unwind_code, p00_code)                                    \
  P00_BLK_END                                                                                    \
/* dispatch. cast the _Bool to int since this is what happens anyhow                             \
   and some compilers will issue strange warnings. The "true" case must be provided              \
   through a placement of P99_PROTECT inside the depending statement. */                         \
  switch ((int)p00_unw) case 0:

p99_inline
void p00_unwind(void* p00_top, unsigned p00_level, int p00_cond) {
  if (p00_level && p00_cond && p00_top) p00_longjmp(p00_top, p00_cond);
}

/**
 ** @brief Preliminary resume from one or several levels of nested
 ** ::P99_UNWIND_PROTECT
 **
 ** @param X If this is evaluates to @c 0 nothing is done. If the
 ** result is positive as most as many levels of ::P99_UNWIND_PROTECT
 ** will be unwound. Otherwise all levels on the stack will be unwound
 ** and the control variables ::p99_unwind_code of these will be set
 ** to the value of @a x.
 **/
/* This uses a very special trick, such that this a valid call to
   longjmp at the end. On the lowest level p00_unwind_top is an
   integer of value 0 that converts to a void* that can be passed to
   longjmp as a formal parameter just such that the syntax is
   correct. On higher recursion levels this will be the variable of
   type jmp_buf that sits on the stack. jmp_buf is guaranteed to be an
   array type, so the expression here evaluates to a pointer that then
   is passed to the function. */
#define P99_UNWIND(X) p00_unwind(p00_unwind_top, p99_unwind_level, (X))

/**
 ** @brief Return from the enclosing function after unwinding all
 ** levels of nested ::P99_UNWIND_PROTECT.
 **
 ** By this you can guarantee that all eventually existing
 ** ::P99_PROTECT parts of enclosing ::P99_UNWIND_PROTECT are properly
 ** executed. This is in some similar to the guarantee of C++ to call
 ** destructors before returning from a function.
 **
 ** There is one important difference, though: the @c return
 ** expression (if any) is evaluated @em after the protected parts are
 ** unwound. So if these modify parts of that @c return expression you
 ** might not get what you expect. So instead of
 **
 ** @code
 ** P99_UNWIND_RETURN expr;
 ** @endcode
 **
 ** You might want do something like
 ** @code
 ** volatile retType myret = expr;
 ** P99_UNWIND_RETURN myret;
 ** @endcode
 **/
#define P99_UNWIND_RETURN                                              \
/* This is just there to prevent spurious dangling else warnings */    \
P00_BLK_START                                                          \
for (;                                                                 \
     !(p00_unwind_bottom && !setjmp(p00_unwind_bottom[0]->p00_buf))    \
       /* assign before we unwind all the way down */                  \
       || (p00_unwind_bottom[0]->p00_returning = 1,                    \
           /* If an unwind is possible, i.e if we are not in the outer \
              frame this will stop the evaluation of the expression    \
              here, and unwind as side effect. Otherwise, this will    \
              continue normally and directly proceed with the          \
              return. */                                               \
           P99_UNWIND(-p99_unwind_return),                             \
           1);                                                         \
     ) P99_ALLOW(RETURN) return

/**
 ** @brief The pseudo label to which we jump when we unwind the stack
 ** with ::P99_UNWIND.
 **
 ** @warning ::P99_UNWIND_PROTECT may contain at most one such label.
 **
 ** @warning Other than a normal C label, ::P99_PROTECT must be at a
 ** location where several statements can be placed. In particular, it
 ** should not be the direct dependent of a conditional.
 **
 ** @see P99_UNWIND_PROTECT
 ** @see P99_UNWIND_RETURN
 ** @see P99_UNWIND
 ** @see p99_unwind_code
 ** @see p99_unwind_level
 **/
#define P99_PROTECT                                                   \
P99_DECLARE_INHIBIT(RETURN);                                          \
if (0) {                                                              \
  /* The switch expression of the surrounding switch from             \
     P99_UNWIND_PROTECT should only have values true and false. So    \
     this default label can never trigger. It is here to ensure that  \
     no other "default" label is placed on the same level of "switch" \
     by error. */                                                     \
 default: ;                                                           \
 } else case 1 : P00_UNCASE

/**
 ** @brief Add some default documentation and links to the following
 ** block macro.
 **/
#define P99_BLOCK_DOCUMENT                                                                      \
/*! \warning Restrictions on preliminary exits from the dependent block or statement apply. **/ \
/*! \see P99_PROTECTED_BLOCK **/                                                                \
/*! \see P99_GUARDED_BLOCK **/

/**
 ** @brief Assert runtime condition @a EXPR on entry and on leave of the
 ** dependent block.
 ** @headerfile p99_c99.h "p99_c99.h"
 **/
P00_DOCUMENT_MULTIPLE_ARGUMENT(P99_INVARIANT, 0)
P99_BLOCK_DOCUMENT
#define P99_INVARIANT(EXPR)                                                                   \
P99_PROTECTED_BLOCK(assert((EXPR) && "failed on entry"), assert((EXPR) && "failed on leave"))


/** @}
 **/


/* Disable bogus warnings that are provoked by the code in this file. */

P99_IF_COMPILER(INTEL, warning(disable: 589)) /* transfer of control bypasses initialization of... */


#endif      /* !P99_BLOCK_H_ */
