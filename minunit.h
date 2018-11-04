//////////////////////////////////////////////////////////
// MINIMAL UNIT TESTING MACROS
// from Learn C the Hard Way
// by Zed A. Shaw
// url: http://c.learncodethehardway.org/book/ex30.html
// Adapted from code by Jera Design
// url: http://www.jera.com/techinfo/jtns/jtn002.html
//////////////////////////////////////////////////////////

/****************************************************
 * These macros make automated unit-testing easier.
 *
 * General layout of a unit-test file:

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

 *
 * That's it!
 ****************************************************/

#ifndef _minunit_h
#define _minunit_h

#ifndef _DEBUG_MODE
# define _DEBUG_MODE
#endif /* _DEBUG_MODE */

#include <stdio.h>
#include <stdlib.h>
#include "dbg.h"

#ifdef __cplusplus
extern "C" {
#endif

#define mu_suite_start() const char *message = NULL

#define mu_assert(test, message) if (!(test)) { log_err(message); return message; }

#define mu_run_test(test) do { debug("\n----- %s", #test); \
	message = test(); tests_run++; if (message) return message; } while (0)

#define RUN_TESTS(name) int main(int argc, const char *argv[]) { \
		argc = 1; \
		debug("----- RUNNING: %s", argv[0]);\
		printf("----\nRUNNING: %s\n", argv[0]);\
		const char *result = name();\
		if (result != 0) {\
			printf("FAILED: %s\n", result);\
		}\
		else {\
			printf("ALL TESTS PASSED\n");\
		}\
		printf("Tests run: %d\n", tests_run);\
		exit(result != 0);\
	}

int tests_run = 0;

#ifdef __cplusplus
}
#endif

#endif /* _minunit_h */
