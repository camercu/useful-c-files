/*********************************************************
 * @brief Awesome Debug Macros
 *
 * Adapted from Zed Shaw's "Learn C the Hard Way"
 * @see url: http://c.learncodethehardway.org/book/ex20.html
 * date accessed: August 2015
 *
 * These macros make debugging easier. Just include
 * this header file in a program and use them!
 *
 * To enable printing of debug() messages:
 * compile with -DDEBUG flag (preferred)
 * -- or --
 * insert '#define DEBUG' into source/header file
 *************************************************/

#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>  /* fprintf() */
#include <errno.h>  /* errno */
#include <string.h> /* strerror() */
#include <time.h>   /* clock() */
#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE, random(), RAND_MAX */
#include <unistd.h> /* usleep() */

/* common function return codes for success and failure */
#define FUNC_SUCCESS (0)
#define FUNC_FAILURE (-1)

/* trace format for debugging/logging
   (this just saves typing in later macro definitions) */
#define _TRACE_   __FILE__, __func__, __LINE__

#define STRINGIFY(X) #X

/* perform macro expansion before stringifying */
#define XSTRINGIFY(X) STRINGIFY(X)

/* safe, readable version of strerror(errno) */
#define clean_strerror() (errno == 0 ? "None" : strerror(errno))

/* logging macros: print formatted message to stderr, with trace to logging stmt */
#define log_err(MSG, ...) fprintf(stderr, "[ERROR] (%s:%s:%d:%s) " MSG "\n", _TRACE_, clean_strerror(), ##__VA_ARGS__)
#define log_warn(MSG, ...) fprintf(stderr, "[WARN] (%s:%s:%d:%s) " MSG "\n", _TRACE_, clean_strerror(), ##__VA_ARGS__)
#define log_info(MSG, ...) fprintf(stderr, "[INFO] (%s:%s:%d) " MSG "\n", _TRACE_, ##__VA_ARGS__)

/* print formatted debug messages to stderr only when NDEBUG is not defined */
#ifdef DEBUG
# define debug(MSG, ...) fprintf(stderr, "[DEBUG] (%s:%s:%d) " MSG "\n", _TRACE_, ##__VA_ARGS__)
#else
# define debug(MSG, ...)
#endif /* _DEBUG_MODE */

/* enhanced assert(); check that ASSERT_COND is true, otherwise log formatted
   error msg then "goto error" for cleanup/exit */
#define check(ASSERT_COND, MSG, ...) if(!(ASSERT_COND)) { log_err(MSG, ##__VA_ARGS__); goto error; }

/* To check for common errors where you might want to turn off the logging */
#define check_debug(ASSERT_COND, MSG, ...) if(!(ASSERT_COND)) { debug(MSG, ##__VA_ARGS__); goto error; }

/* checks that memory was allocated to pointer as expected, i.e. not NULL */
#define check_mem(MEM_PTR) if(!(MEM_PTR)) { log_err("Out of memory."); errno=ENOMEM; goto error; }

/* like check_mem, but for pointers you plan to dereference */
#define check_ptr(PTR) if(!(PTR)) { log_err("%s cannot be NULL; will cause dereference error.", STRINGIFY(PTR)); errno=EINVAL; goto error; }

/* placed in part of fn that shouldn't run, i.e. if/switch branches;
   prints error followed by "goto error" for cleanup/exit */
# define sentinel(MSG, ...) do { log_err(MSG, ##__VA_ARGS__); goto error; } while(0)

/* like check, but exits program immediately with EXIT_FAILURE when assert fails */
#define enforce(ASSERT_COND, MSG, ...) if(!(ASSERT_COND)) { log_err(MSG, ##__VA_ARGS__); exit(EXIT_FAILURE); }
#define enforce_mem(MEM_PTR) enforce((MEM_PTR), "Out of memory.")

/* Cause program to act as if check() failed, and goto error "unexpectedly".
   Insert wherever you want to test an unexpected failure in your program code. */
#define FAIL() do { log_err("FAIL point for debugging"); errno=0; goto error; } while (0)

/* Like fail(), causes "unexpected" program exit, like when failing enforce() */
#define DIE() do { log_err("EXIT_FAILURE for debugging"); exit(EXIT_FAILURE); } while (0)

/* computes how long a function takes to run (miliseconds) */
#define log_time(FUNC, ...) do { clock_t start=clock(); \
                            (*(FUNC))(__VA_ARGS__); \
                            clock_t end=clock(); \
                            double elapsed = (double)(end-start)*1000.0/CLOCKS_PER_SEC; \
                            log_info("%s took %.3f ms to run", STRINGIFY(FUNC), elapsed); } while(0)
/* ^^ useful resource I referenced to help build this:
   https://mikeash.com/pyblog/friday-qa-2010-12-31-c-macro-tips-and-tricks.html */

/* performs free() [or equivalent] only if ptr is not NULL, and sets ptr to
   NULL after free. Helps avoid double-free bugs with "goto error"
   Example: safefree(ptr);
   Example: safefree(mutex_ptr, pthread_mutex_destroy); */
#define __safefree(PTR, FREE_FUNC, ...) if((PTR)) { (*(FREE_FUNC))((void*)(PTR)); (PTR) = NULL; }
#define safefree(PTR, ...) __safefree((PTR), ##__VA_ARGS__, free )

/* Add random delay to program at this point (0-10ms). Useful for debugging race conditions */
#define jitter() usleep((int)((double)random()/(double)RAND_MAX * 10000))

/* printf pattern for displaying binary of single byte
   source: https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
   Example: printf("Byte value: " BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte)); */
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(BYTE)  \
  ((BYTE) & 0x80 ? '1' : '0'), \
  ((BYTE) & 0x40 ? '1' : '0'), \
  ((BYTE) & 0x20 ? '1' : '0'), \
  ((BYTE) & 0x10 ? '1' : '0'), \
  ((BYTE) & 0x08 ? '1' : '0'), \
  ((BYTE) & 0x04 ? '1' : '0'), \
  ((BYTE) & 0x02 ? '1' : '0'), \
  ((BYTE) & 0x01 ? '1' : '0')

#endif /* __dbg_h__ */
