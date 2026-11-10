#ifndef AUTOMATA_RANDOM_H
#define AUTOMATA_RANDOM_H

/*
 * Seedable pseudo-random number generator for simulation rules.
 *
 * The generator is process-global so that rules can call it without
 * receiving a context argument. It is deterministic: the same seed always
 * produces the same sequence on every platform. It is NOT thread-safe and
 * NOT suitable for cryptography.
 *
 * If ca_random_seed() is never called, the generator starts from a fixed
 * internal seed, so an unseeded program is still reproducible.
 */

#include <stdint.h>

/* Reset the generator. Any 64-bit value, including 0, is a valid seed. */
void ca_random_seed(uint64_t seed);

/* Return the next raw 64-bit value of the sequence. */
uint64_t ca_random_u64(void);

/*
 * Return a uniformly distributed integer in the inclusive range [min, max].
 * If min > max, return min.
 */
int ca_random_int(int min, int max);

#endif /* AUTOMATA_RANDOM_H */
