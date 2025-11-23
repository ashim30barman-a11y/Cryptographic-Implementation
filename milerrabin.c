// mr_semiprime_experiment.c
// Miller–Rabin experiment on a 512-bit semiprime n = p*q (p,q are 256-bit primes).
// Uses GMP for big integers but implements the MR round logic explicitly.

#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

// --- Utilities ---------------------------------------------------------------

// Seed GMP RNG from /dev/urandom; fallback to time+pid if needed.
static void seed_rng(gmp_randstate_t st) {
    FILE *urnd = fopen("/dev/urandom", "rb");
    if (urnd) {
        unsigned char buf[32]; // 256 bits of seed
        if (fread(buf, 1, sizeof(buf), urnd) == sizeof(buf)) {
            mpz_t seed;
            mpz_init(seed);
            mpz_import(seed, sizeof(buf), 1, 1, 0, 0, buf);
            gmp_randseed(st, seed);
            mpz_clear(seed);
            fclose(urnd);
            return;
        }
        fclose(urnd);
    }
    // Fallback
    unsigned long fallback = (unsigned long)time(NULL) ^ (unsigned long)getpid();
    gmp_randseed_ui(st, fallback);
}

// Return a random a in [2, n-2].
static void random_base_in_range(mpz_t a, const mpz_t n, gmp_randstate_t st) {
    mpz_t range;
    mpz_init(range);
    mpz_sub_ui(range, n, 3);     // range = n - 3  (for [0, n-3])
    mpz_urandomm(a, st, range);  // a in [0, n-3]
    mpz_add_ui(a, a, 2);         // a in [2, n-2]
    mpz_clear(range);
}

// Generate a random prime with exactly 'bits' bits using mpz_nextprime.
// Ensures the top bit is set (so it is truly 'bits' wide) and odd.
static void random_prime_bits(mpz_t p, unsigned bits, gmp_randstate_t st) {
    mpz_urandomb(p, st, bits);
    // Ensure top bit set => exactly 'bits'-bit number
    mpz_setbit(p, bits - 1);
    // Ensure odd
    mpz_setbit(p, 0);
    // Move to next prime >= p (probabilistic but extremely reliable)
    mpz_nextprime(p, p);
    // If it somehow rolled to (bits+1)-bit (extremely unlikely), mask and retry
    while (mpz_sizeinbase(p, 2) != bits) {
        mpz_urandomb(p, st, bits);
        mpz_setbit(p, bits - 1);
        mpz_setbit(p, 0);
        mpz_nextprime(p, p);
    }
}

// Decompose n-1 as 2^s * d with d odd. Returns s and sets d.
static unsigned decompose_n_minus_one(const mpz_t n, mpz_t d) {
    mpz_sub_ui(d, n, 1); // d = n-1
    unsigned s = 0;
    while (mpz_even_p(d)) {
        mpz_fdiv_q_2exp(d, d, 1); // d >>= 1
        s++;
    }
    return s;
}

// One Miller-Rabin round for base 'a' against odd n>2.
// Returns 1 if this round says "probably prime", 0 if it proves composite.
static int miller_rabin_round(const mpz_t n, const mpz_t a, const mpz_t d, unsigned s) {
    mpz_t x, n_minus_1;
    mpz_init(x);
    mpz_init(n_minus_1);
    mpz_sub_ui(n_minus_1, n, 1);

    // x = a^d mod n
    mpz_powm(x, a, d, n);

    if (mpz_cmp_ui(x, 1) == 0 || mpz_cmp(x, n_minus_1) == 0) {
        mpz_clear(x);
        mpz_clear(n_minus_1);
        return 1; // "probably prime" for this round
    }

    for (unsigned r = 1; r < s; r++) {
        // x = x^2 mod n
        mpz_powm_ui(x, x, 2, n);
        if (mpz_cmp(x, n_minus_1) == 0) {
            mpz_clear(x);
            mpz_clear(n_minus_1);
            return 1; // "probably prime" for this round
        }
    }

    mpz_clear(x);
    mpz_clear(n_minus_1);
    return 0; // definitely composite
}

// Convenience: run one-round MR with a random base in [2, n-2].
static int mr_one_random_round(const mpz_t n, gmp_randstate_t st, const mpz_t d, unsigned s) {
    mpz_t a;
    mpz_init(a);
    random_base_in_range(a, n, st);
    int res = miller_rabin_round(n, a, d, s);
    mpz_clear(a);
    return res;
}

int main(void) {
    // Initialize RNG
    gmp_randstate_t st;
    gmp_randinit_mt(st);
    seed_rng(st);

    // Generate two random 256-bit primes p and q
    mpz_t p, q, n;
    mpz_init(p); mpz_init(q); mpz_init(n);

    random_prime_bits(p, 256, st);
    random_prime_bits(q, 256, st);

    // Compute n = p*q  (a ~512-bit composite)
    mpz_mul(n, p, q);

    // Ensure n is odd and > 3 (it will be)
    if (mpz_even_p(n) || mpz_cmp_ui(n, 3) <= 0) {
        fprintf(stderr, "Unexpected n (should be odd and >3)\n");
        return 1;
    }

    // Prepare n-1 = 2^s * d
    mpz_t d;
    mpz_init(d);
    unsigned s = decompose_n_minus_one(n, d);

    // Report sizes
    printf("p bits: %zu\n", mpz_sizeinbase(p, 2));
    printf("q bits: %zu\n", mpz_sizeinbase(q, 2));
    printf("n bits: %zu\n", mpz_sizeinbase(n, 2));
    printf("n (hex): ");
    mpz_out_str(stdout, 16, n);
    printf("\n");

    // Experiment parameters
    const unsigned long TRIALS = 100000UL;
    unsigned long false_probably_prime = 0;

    // Run MR one-round test TRIALS times with independent random bases
    for (unsigned long t = 0; t < TRIALS; t++) {
        // Each call uses a fresh random base in [2, n-2]
        int round_result = mr_one_random_round(n, st, d, s);
        // For a composite n, "round_result==1" is a *false* "probably prime"
        if (round_result == 1) false_probably_prime++;
    }

    // Summarize
    double rate = (double)false_probably_prime / (double)TRIALS;
    printf("\n--- Miller–Rabin one-round experiment on composite n ---\n");
    printf("Trials: %lu\n", TRIALS);
    printf("False 'probably prime' count: %lu\n", false_probably_prime);
    printf("Empirical false-positive rate per round: %.6f\n", rate);
    printf("Theoretical bound per round: <= 0.25\n");
    printf("Bound check: %s\n", (rate <= 0.25 + 1e-12) ? "PASS (rate ≤ 0.25)" : "FAIL (rate > 0.25)");

    // Cleanup
    mpz_clear(p); mpz_clear(q); mpz_clear(n);
    mpz_clear(d);
    gmp_randclear(st);
    return 0;
}

