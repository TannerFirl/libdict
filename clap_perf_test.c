/*
 * clap_perf_test.c - standalone micro-benchmark harness for
 * dict_prime_geq() (src/hashtable_common.c).
 *
 * This harness is identical across the "before" and "after" trees; only
 * the implementation of dict_prime_geq() in src/hashtable_common.c
 * differs (linear scan vs. binary search). It calls dict_prime_geq()
 * many times with a fixed, deterministic sequence of inputs spanning the
 * full range of the kPrimes[] table (including values that force a full
 * scan to the last entry, which is the worst case for the linear-scan
 * implementation) and reports both a checksum (to guarantee both builds
 * compute identical results) and the elapsed wall-clock time.
 */

#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>

#include "hashtable_common.h"

#define ITERATIONS 20000000u

static double
now_sec(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

int
main(void)
{
    unsigned i;
    unsigned checksum = 0;
    /* Deterministic pseudo-random-like sweep over the full unsigned
     * range using a fixed linear congruential sequence (fixed seed),
     * so every run (before/after) sees the exact same input sequence. */
    unsigned n = 1u;
    double t0, t1;

    t0 = now_sec();
    for (i = 0; i < ITERATIONS; ++i) {
	n = n * 1103515245u + 12345u;
	checksum += dict_prime_geq(n);
    }
    t1 = now_sec();

    fprintf(stderr, "CHECKSUM=%u\n", checksum);
    printf("PERF_TIME_SEC=%.6f\n", t1 - t0);
    return 0;
}
