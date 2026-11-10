#ifndef AUTOMATA_TEST_H
#define AUTOMATA_TEST_H

/*
 * Minimal test harness. Each tests/test_*.c file is its own executable,
 * so these file-scope counters are private to that executable.
 *
 * A failed CHECK records the failure and continues, so one run reports
 * every broken expectation instead of stopping at the first.
 */

#include <stdio.h>
#include <stdlib.h>

static int test_checks = 0;
static int test_failures = 0;

#define CHECK(cond)                                                            \
  do {                                                                         \
    test_checks++;                                                             \
    if (!(cond)) {                                                             \
      test_failures++;                                                         \
      fprintf(stderr, "%s:%d: CHECK failed: %s\n", __FILE__, __LINE__, #cond); \
    }                                                                          \
  } while (0)

/* Evaluates each argument exactly once and prints both values on failure. */
#define CHECK_EQ_INT(actual, expected)                                         \
  do {                                                                         \
    long long actual_ = (long long)(actual);                                   \
    long long expected_ = (long long)(expected);                               \
    test_checks++;                                                             \
    if (actual_ != expected_) {                                                \
      test_failures++;                                                         \
      fprintf(stderr,                                                          \
              "%s:%d: CHECK_EQ_INT failed: %s == %lld, expected %lld\n",       \
              __FILE__, __LINE__, #actual, actual_, expected_);                \
    }                                                                          \
  } while (0)

#define RUN_TEST(fn)                                                           \
  do {                                                                         \
    printf("  %s\n", #fn);                                                     \
    fn();                                                                      \
  } while (0)

/* Print a summary and return the process exit status for main(). */
static inline int test_summary(void) {
  printf("  %d checks, %d failed\n", test_checks, test_failures);
  return test_failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

#endif /* AUTOMATA_TEST_H */
