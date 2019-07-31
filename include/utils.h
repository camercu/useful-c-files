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
 * @brief Macro for absolute value of a numeric type
 */
#define ABS(VALUE) ((VALUE) < 0 ? -(VALUE) : (VALUE))

/**
 * @brief Macro to return the minimum of two numbers.
 * @warning
 * The numbers being compared must have the same signedness, or else you may
 * get unpredictable results!
 */
#define MIN(A, B) ((A) < (B) ? (A) : (B))

/**
 * @brief Macro to return the minimum of two numbers.
 * @warning
 * The numbers being compared must have the same signedness, or else you may
 * get unpredictable results!
 */
#define MAX(A, B) ((A) > (B) ? (A) : (B))

/**
 * @brief Macro to swap the values contained by two numeric variables.
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
 * @brief A macro to restart a system call that was interrupted by a signal.
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
 * @brief Returns a compile-time constant indicating whether a system uses
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

#endif /* __util_macros_h__ */
