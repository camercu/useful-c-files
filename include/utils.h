/********************************************************************
 * @brief Utility functions and macros for generic use.
 * @file utils.h
 ********************************************************************/

#ifndef __util_macros_h__
#define __util_macros_h__

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Macro to replace a boolean expression with it's string equivalent
 *
 * This is useful for printf statements where you want to see the boolean displayed as a string.
 *
 * Example usage:
 * @code
 * bool is_false = 0;
 * printf("Value of is_false: %s\n", BOOLSTR(is_false));
 * @endcode
 * This will print: <tt>Value of is_false: False</tt>
 */
#define BOOLSTR(BOOL_EXPR) ((BOOL_EXPR) ? "True" : "False")

/**
 * @brief Return the absolute value of a numeric type
 */
#define ABS(VALUE) ((VALUE) < 0 ? -(VALUE) : (VALUE))

/**
 * @brief Return the minimum of two numbers.
 * @warning
 * The numbers being compared must have the same signedness, or else you may
 * get unpredictable results!
 */
#define MIN(A, B) ((A) < (B) ? (A) : (B))

/**
 * @brief Return the maximum of two numbers.
 * @warning
 * The numbers being compared must have the same signedness, or else you may
 * get unpredictable results!
 */
#define MAX(A, B) ((A) > (B) ? (A) : (B))

/**
 * @brief Round @c X up to the nearest multiple of @c Y.
 * @warning
 * The numbers @c X and @c Y must be of the same type
 */
#define ROUNDUP(X, Y) (((X) + (Y) - 1) / (Y) * (Y))

/**
 * @brief Round @c X down to the nearest multiple of @c Y.
 * @warning
 * The numbers @c X and @c Y must be of the same type
 */
#define ROUNDDOWN(X, Y) ((X) / (Y) * (Y))

/**
 * @brief Get the ceiling of <tt>X / Y</tt>
 * @warning
 * The numbers @c X and @c Y must be of the same type
 */
#define CEILING(X, Y) (((X) + (Y) - 1) / (Y))

/**
 * @brief Swap the values contained by two numeric variables in place.
 *
 * @warning
 * The variables must be of the same type (or at least be represented with the
 * same number of bytes), or you may get unexpected results! */
#define SWAP(A, B)  \
    do {            \
        (A) ^= (B); \
        (B) ^= (A); \
        (A) ^= (B); \
    } while (0)


/**
 * @brief Return the number of elements in an array
 */
#define ARRAYLEN(ARR) (sizeof((ARR)) / sizeof(*(ARR)))

/**
 * @brief Make an N-bit mask.
 *
 * @warning
 * @c N must be in the range 1-31 (inclusive)!
 */
#define BITMASK(N) ((1 << (N)) - 1)

/**
 * @brief Make a bitmask where the bits between indeces [LOW..HI] are set.
 * @note
 * Index 0 is the least-significant bit
 * @warning
 * Caller must ensure <tt>LOW < HIGH</tt>. @c LOW and @c HI must both
 * be in the range of [0..31].
 */
#define MASKRANGE(LOW, HI) (((HI) - (LOW) + 1) << (LOW))

/**
 * @brief Token concatenation for macros
 *
 * The X* macros allow the C preprocessor to prescan/expand the tokens to their definitions.
 */
#define STR(S) #S
#define XSTR(S) STR(S)
#define CONC(A, B) A##B
#define XCONC(A, B) CONC(A, B)
#define XXCONC(A, B) XCONC(A, B)


/**
 * @brief Do a system call, restarting if interrupted by a signal.
 * @see: Kerrisk, Michael. The Linux Programming Interface: A Linux and UNIX
 * System Programming Handbook. No Starch Press. San Francisco, 2010. pg. 443
 *
 * Example usage:
 * @code
 * NO_EINTR(cnt = read(fd, buf, BUF_SIZE));
 * check(cnt != -1, "read() failed with other than EINTR");
 * @endcode
 */
#define NO_EINTR(STMT)                     \
    while ((STMT) == -1 && errno == EINTR) \
        ;

/**
 * @brief Typedef to make signal handlers more obvious.
 * It allows re-defining the signal() function as a wrapper for sigaction()
 */
typedef void (*sig_handler_t)(int signo);

/**
 * @brief Return a compile-time constant indicating whether a system uses
 * Big Endian byte order.
 * @returns @c 1 if big endian, @c 0 if little endian
 * @see https://stackoverflow.com/questions/8978935/detecting-endianness
 * @see https://godbolt.org/g/DAafKo
 */
#define is_little_endian() (__endian_u.val)

static const union {
    uint16_t u16;
    uint8_t val;
} __endian_u = {.u16 = 1};


/**
 * @brief Inform compiler of commonly expected value for branch prediction.
 */
#if (__GNUC__ >= 3)
#define LIKELY(EXPR) __builtin_expect(!!(EXPR), 1)
#define UNLIKELY(EXPR) __builtin_expect((EXPR), 0)
#else
#define LIKELY(EXPR) (EXPR)
#define UNLIKELY(EXPR) (EXPR)
#endif /* (__GNUC__ >= 3) */


/**
 * @brief Specify the minimum alignment in bytes (e.g. for structs)
 */
#ifdef __GNUC__
#define ALIGNED(N) __attribute__((__aligned__(N)))
#else
#define ALIGNED(N)
#endif /* __GNUC__


/**
 * @brief Pack structs to reduce internal alignment padding
 */
#ifdef __GNUC__
#define PACKED() __attribute__((__packed__()))
#else
#define PACKED()
#endif /* __GNUC__


/**
 * @brief Perform CODE only once, even after successive calls to containing function
 *
 * @warning
 * This is not intended to be thread-safe!
 */
#define DO_ONCE(CODE) \
    do { \
        static bool _was_done_already = false; \
        if (UNLIKELY(!_was_done_already)) { \
            _was_done_already = true; \
            CODE; \
        } \
    } while (0)



#endif /* __util_macros_h__ */
