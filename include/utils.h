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
 * @brief Round @c x up to the nearest multiple of @c y.
 * @warning
 * The numbers @c x and @c y must be of the same type
 */
#define ROUNDUP(x, y) (((x) + (y) - 1) / (y) * (y))

/**
 * @brief Round @c x down to the nearest multiple of @c y.
 * @warning
 * The numbers @c x and @c y must be of the same type
 */
#define ROUNDDOWN(x, y) ((x) / (y) * (y))

/**
 * @brief Get the ceiling of <tt>x / y</tt>
 * @warning
 * The numbers @c x and @c y must be of the same type
 */
#define CEILING(x, y) (((x) + (y) - 1) / (y))

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
#define ARRAYLEN(arr) (sizeof((arr)) / sizeof(*(arr)))

/**
 * @brief Make an n-bit mask.
 *
 * @warning
 * @c n must be in the range 1-31 (inclusive)!
 */
#define BITMASK(n) ((1 << (n)) - 1)

/**
 * @brief Make a bitmask where the bits between indeces [low..hi] are set.
 * @note
 * Index 0 is the least-significant bit
 * @warning
 * Caller must ensure <tt>low < high</tt>. @c low and @c hi must both
 * be in the range of [0..31].
 */
#define MASKRANGE(low, hi) (((hi) - (low) + 1) << (low))

/**
 * @brief Token concatenation for macros
 *
 * The X* macros allow the C preprocessor to prescan/expand the tokens to their definitions.
 */
#define MAKESTR(s) #s
#define XSTR(s) MAKESTR(s)
#define CONC(x, y) x##y
#define XCONC(x, y) CONC(x, y)
#define XXCONC(x, y) XCONC(x, y)


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
#define LIKELY(expr) __builtin_expect(!!(expr), 1)
#define UNLIKELY(expr) __builtin_expect((expr), 0)
#else
#define LIKELY(expr) (expr)
#define UNLIKELY(expr) (expr)
#endif /* (__GNUC__ >= 3) */


/**
 * @brief Specify the minimum alignment in bytes (e.g. for structs)
 */
#ifdef __GNUC__
#define ALIGNED(n) __attribute__((__aligned__(n)))
#else
#define ALIGNED(n)
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
 * @brief Perform code only once, even after successive calls to containing function
 *
 * @warning
 * This is not intended to be thread-safe!
 */
#define DO_ONCE(code) \
    do { \
        static bool _was_done_already = false; \
        if (UNLIKELY(!_was_done_already)) { \
            _was_done_already = true; \
            code; \
        } \
    } while (0)



#endif /* __util_macros_h__ */
