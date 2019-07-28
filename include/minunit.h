/*************************************************************
 * @brief MINIMAL UNIT TESTING MACROS
 *
 * Adapted from code in Learn C the Hard Way, by Zed A. Shaw:
 * @see url: http://c.learncodethehardway.org/book/ex30.html
 * (which was in turn adapted from code by Jera Design:
 * @see url: http://www.jera.com/techinfo/jtns/jtn002.html
 * @file minunit.h
 *
 * These macros make automated unit-testing easier.
 *
 * General layout of a unit-test file:
@code

#include "minunit.h"

const char *test_something() {
    // testing assertions go here  //
    // ex: mu_assert(1 < 2, "Math is broken!");
    return NULL; // indicates success
}

// ...
// more test functions like the one above
// ...

const char *all_tests() {
    mu_suite_start();

    mu_run_test(test_something);
    // ...
    // more test function calls
    // ...

    return NULL; // indicates success
}

RUN_TESTS(all_tests);

@endcode
 *
 * That's it!
 *
 * @note
 * The return type of every test function should be @code const char*@endcode
 *
 * @note
 * Test functions return NULL to indicate success. You normally don't
 * need to explicity return an error message on failure because mu_assert()
 * handles that for you.
 ****************************************************/

#ifndef _minunit_h
#define _minunit_h

/* Must put this before the header includes to make sure debugging works
 * properly */
#ifdef NDEBUG
#    undef NDEBUG
#endif /* NDEBUG */
#ifndef DEBUG
#    define DEBUG
#endif /* DEBUG */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbg.h"

/**
 * @brief Initializes datastructures for the main testing routine.
 *
 * @warning
 * This MUST be the first line of the main testing function (i.e.
 * the one that calls all of the other test functions)
 */
#define mu_suite_start()        \
    const char *message = NULL; \
    memset(&__mu_msg_buf, 0, MAX_MSG_LEN + 1)

/**
 * @brief A beefed-up version of assert(3) that prints a formatted
 * debug statement on assertion failure.
 *
 * @param test_cond The test expression that should evaluate to true on passing
 * @param message_fmt A printf-like format string
 * @param ... [optional] Arguments that need to be passed to the format string
 */
#define mu_assert(test_cond, message_fmt, ...)                           \
    if (!(test_cond)) {                                                  \
        log_err(message_fmt, ##__VA_ARGS__);                             \
        snprintf(__mu_msg_buf, MAX_MSG_LEN, message_fmt, ##__VA_ARGS__); \
        return __mu_msg_buf;                                             \
    }

/**
 * @brief A macro to run a test function, along with any args that get passed to
 * the function.
 *
 * This should be used to call test functions within the main test
 * routine, after calling mu_assert().
 *
 * @param test The test function to run
 * @param ... [optional] Arguments to pass to the test function. This
 * allows for parameterized testing.
 */
#define mu_run_test(test, ...)                              \
    do {                                                    \
        fprintf(stderr, "--- [minunit] %s() ---\n", #test); \
        message = test(__VA_ARGS__);                        \
        __mu_tests_run++;                                   \
        if (message)                                        \
            return message;                                 \
    } while (0)

/**
 * @brief This kicks off the series of test routines by calling your test_all
 * function.
 *
 * This macro should only be called once! Also note that it takes the place of
 * main(), so you do not need to create a main() function.
 * @param test_all The main test routine that calls all other tests to run.
 * @returns @c 0 on success, @ 1 on failure. This is the program exit code.
 */
#define RUN_TESTS(test_all)                                          \
    int main(int argc, const char *argv[]) {                         \
        argc = 1;                                                    \
        fprintf(stderr, ">>> [minunit] RUNNING: %s >>>\n", argv[0]); \
        printf("[minunit] RUNNING: %s ---\n", argv[0]);              \
        const char *result = test_all();                             \
        if (result != 0) {                                           \
            printf("[minunit] FAILED: %s\n", result);                \
        } else {                                                     \
            printf("[minunit] ALL TESTS PASSED\n");                  \
        }                                                            \
        printf("[minunit] Tests run: %d\n", __mu_tests_run);         \
        exit(result != 0);                                           \
    }

/* These are for internal use of the test suite */
#define MAX_MSG_LEN 512
char __mu_msg_buf[MAX_MSG_LEN + 1];
int __mu_tests_run = 0;

#endif /* _minunit_h */
