#include "minunit.h"

#include <stdlib.h>
#include <string.h>
#include "deque.h"

const char *test_create_destroy() {
    /* pre-fill heap with garbage */
    static const size_t sz = 1024;
    void *garbage = malloc(sz);
    check_mem(garbage);
    memset(garbage, 0xFF, sz);
    free(garbage);

    deque_t *new = dq_create();
    mu_assert(new, "dq_create() failed. Out of memory?");
    mu_assert(new->n_items == 0, "Didn't zero-init n_items");
    mu_assert(!(new->head) && !(new->tail), "Didn't init head/tail to NULL");
    dq_destroy(new, NULL);
    return NULL; /* indicates success */
error:
    return "FAIL during setup/teardown";
}

const char *test_push_pop(size_t n_items) {
    size_t i, tmp;
    deque_t *dq = dq_create();
    check_mem(dq);

    /* test PUSH */
    for (i = 1; i <= n_items; i++) {
        mu_assert(dq_push(dq, (void *)i), "Failed to push. Out of memory?");
        mu_assert(dq_len(dq) == (ssize_t)i,
                  "Incorrect push len update.\n"
                  "\tExpected:%lu, Got:%lu",
                  i, dq_len(dq));
    }

    /* test POP */
    for (i = n_items; i > 0; i--) {
        tmp = (size_t)dq_pop(dq);
        mu_assert(i == tmp,
                  "Incorrect pop return value.\n"
                  "\tExpected:%lu, Got:%lu",
                  i, tmp);
        mu_assert(dq_len(dq) == (ssize_t)i - 1, "Incorrect push len update");
    }
    mu_assert(dq_len(dq) == 0, "Logic error");
    dq_destroy(dq, NULL);
    return NULL;
error:
    return "FAIL during setup/teardown";
}

const char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create_destroy);
    mu_run_test(test_push_pop, 3); /**< example of parameterized testing */
    /* ... */
    /* more test function calls */
    /* ... */

    return NULL; /* indicates success */
}

RUN_TESTS(all_tests);
