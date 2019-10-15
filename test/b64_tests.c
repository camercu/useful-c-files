#include "base64.h"
#include "minunit.h"

const char *test_b64decode(void) {
    const char *monkey_biz = "TW9ua2V5IEJ1c2luZXNz";
    size_t outlen;
    char *output = b64decode(monkey_biz, strlen(monkey_biz), B64_STANDARD, &outlen);
    mu_assert(!strcmp(output, "Monkey Business"),
              "Decoded output didn't match expected: '%s'", output);
    mu_assert(outlen == 15, "Wrong length of decoded output!");
    free(output);

    const char *padded = "cGFkZGluZyBjaGVjaw==";
    output = b64decode(padded, strlen(padded), B64_STANDARD, &outlen);
    mu_assert(!strcmp(output, "padding check"),
              "Padded output didn't match expected: '%s'", output);
    mu_assert(outlen == 13, "Wrong length of padded output: %zu", outlen);
    free(output);

    output = b64decode(padded, strlen(padded)-2, B64_STANDARD, &outlen);
    mu_assert(!strcmp(output, "padding check"),
              "Padded output #2 didn't match expected: '%s'", output);
    mu_assert(outlen == 13, "Wrong length of padded output #2: %zu", outlen);
    free(output);

    const char *pad1 = "MSBwYWQgY2hlY2s=";
    output = b64decode(pad1, strlen(pad1), B64_STANDARD, &outlen);
    mu_assert(!strcmp(output, "1 pad check"),
              "1-pad output didn't match expected: '%s'", output);
    mu_assert(outlen == 11, "Wrong length for 1-pad output: %zu", outlen);
    free(output);

    output = b64decode(pad1, strlen(pad1)-1, B64_STANDARD, &outlen);
    mu_assert(!strcmp(output, "1 pad check"),
              "1-pad output #2 didn't match expected: '%s'", output);
    mu_assert(outlen == 11, "Wrong length for 1-pad output #2: %zu", outlen);
    free(output);

    return NULL;
}


const char *all_tests() {
    mu_suite_start();

    mu_run_test(test_b64decode);

    return NULL;
}

RUN_TESTS(all_tests);

