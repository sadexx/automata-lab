#include "automata/random.h"

/*
 * SplitMix64 (Steele, Lea, Flood 2014; public-domain reference by
 * Sebastiano Vigna). One 64-bit word of state, every seed is valid,
 * and the output passes standard statistical test suites.
 */

#define DEFAULT_SEED UINT64_C(0x853C49E6748FEA9B)

static uint64_t state = DEFAULT_SEED;

void ca_random_seed(uint64_t seed) { state = seed; }

uint64_t ca_random_u64(void) {
  uint64_t z = (state += UINT64_C(0x9E3779B97F4A7C15));
  z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
  z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
  return z ^ (z >> 31);
}

int ca_random_int(int min, int max) {
  if (min > max) {
    return min;
  }

  /*
   * Width of the range. Computed in 64 bits because max - min can exceed
   * INT_MAX (e.g. INT_MIN..INT_MAX), which would overflow in int.
   */
  uint64_t span = (uint64_t)((int64_t)max - (int64_t)min) + 1;

  /*
   * Rejection sampling removes modulo bias. 2^64 is not generally a
   * multiple of span, so the lowest (2^64 mod span) raw values would map
   * to small results one extra time. Discard them. Unsigned negation is
   * well defined: (0 - span) % span == 2^64 mod span.
   */
  uint64_t threshold = (0 - span) % span;
  uint64_t r;
  do {
    r = ca_random_u64();
  } while (r < threshold);

  return (int)((int64_t)min + (int64_t)(r % span));
}
