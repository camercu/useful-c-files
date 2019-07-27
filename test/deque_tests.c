#include "minunit.h"

#include "deque.h"

const char *test_create_destroy() {
    deque_t *new = dq_create();
    if (!new) return "dq_create() failed. Out of memory?";
    dq_destroy(new, NULL);
    return NULL;  // indicates success
}

// ...
// more test functions like the one above
// ...

const char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create_destroy);
    // ...
    // more test function calls
    // ...

    return NULL;  // indicates success
}

RUN_TESTS(all_tests);
