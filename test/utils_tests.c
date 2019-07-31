#include "minunit.h"
#include "utils.h"

const char *test_endian() {
    mu_assert(is_little_endian() == __LITTLE_ENDIAN__,
              "Expected: %d, got: %d",
              __LITTLE_ENDIAN__,
              is_little_endian());
    return NULL;
}

const char *all_tests() {
    mu_suite_start();
    mu_run_test(test_endian);
    return NULL;
}

RUN_TESTS(all_tests);
