#include "automata/random.h"
#include "test.h"

#include <limits.h>

#define SEQUENCE_LENGTH 100

static void test_values_stay_in_inclusive_range(void) {
  int seen[10] = {0};

  ca_random_seed(42);
  for (int i = 0; i < 10000; i++) {
    int v = ca_random_int(0, 9);
    CHECK(v >= 0 && v <= 9);
    if (v >= 0 && v <= 9) {
      seen[v] = 1;
    }
  }

  /* Inclusive means both endpoints must be reachable. */
  for (int v = 0; v < 10; v++) {
    CHECK(seen[v]);
  }
}

static void test_negative_range(void) {
  ca_random_seed(7);
  for (int i = 0; i < 1000; i++) {
    int v = ca_random_int(-3, 3);
    CHECK(v >= -3 && v <= 3);
  }
}

static void test_single_value_range(void) {
  for (int i = 0; i < 100; i++) {
    CHECK_EQ_INT(ca_random_int(5, 5), 5);
  }
}

static void test_inverted_range_returns_min(void) {
  CHECK_EQ_INT(ca_random_int(10, 3), 10);
  CHECK_EQ_INT(ca_random_int(0, -1), 0);
}

static void test_full_int_range(void) {
  /*
   * max - min overflows int here. Under -fsanitize=undefined a span
   * computed in int would be reported; here we also check that both
   * halves of the range are actually produced.
   */
  int negatives = 0;
  int positives = 0;

  ca_random_seed(1);
  for (int i = 0; i < 1000; i++) {
    int v = ca_random_int(INT_MIN, INT_MAX);
    if (v < 0) {
      negatives++;
    } else {
      positives++;
    }
  }

  CHECK(negatives > 0);
  CHECK(positives > 0);
}

static void test_same_seed_same_sequence(void) {
  int first[SEQUENCE_LENGTH];

  ca_random_seed(123);
  for (int i = 0; i < SEQUENCE_LENGTH; i++) {
    first[i] = ca_random_int(0, 1000000);
  }

  ca_random_seed(123);
  for (int i = 0; i < SEQUENCE_LENGTH; i++) {
    CHECK_EQ_INT(ca_random_int(0, 1000000), first[i]);
  }
}

static void test_different_seeds_differ(void) {
  uint64_t a[SEQUENCE_LENGTH];
  int differences = 0;

  ca_random_seed(1);
  for (int i = 0; i < SEQUENCE_LENGTH; i++) {
    a[i] = ca_random_u64();
  }

  ca_random_seed(2);
  for (int i = 0; i < SEQUENCE_LENGTH; i++) {
    if (ca_random_u64() != a[i]) {
      differences++;
    }
  }

  CHECK(differences > SEQUENCE_LENGTH / 2);
}

static void test_distribution_is_roughly_uniform(void) {
  /*
   * Deterministic for a fixed seed, so this cannot be flaky. It catches
   * gross bias (e.g. a broken range calculation), not subtle statistics.
   */
  enum { BUCKETS = 10, DRAWS = 100000 };
  int counts[BUCKETS] = {0};

  ca_random_seed(2026);
  for (int i = 0; i < DRAWS; i++) {
    counts[ca_random_int(0, BUCKETS - 1)]++;
  }

  int expected = DRAWS / BUCKETS;
  for (int b = 0; b < BUCKETS; b++) {
    CHECK(counts[b] > expected * 9 / 10 && counts[b] < expected * 11 / 10);
  }
}

int main(void) {
  printf("test_random\n");
  RUN_TEST(test_values_stay_in_inclusive_range);
  RUN_TEST(test_negative_range);
  RUN_TEST(test_single_value_range);
  RUN_TEST(test_inverted_range_returns_min);
  RUN_TEST(test_full_int_range);
  RUN_TEST(test_same_seed_same_sequence);
  RUN_TEST(test_different_seeds_differ);
  RUN_TEST(test_distribution_is_roughly_uniform);
  return test_summary();
}
