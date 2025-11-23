#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

// Function to measure clock cycles
unsigned long long get_clock_cycles() {
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
}

// Function to perform RSA operations for a given bit size
void rsa_operations(int bit_size, FILE *output_file) {
    gmp_randstate_t state;
    mpz_t p, q, N, phi_N, e, d, m, c, m_prime, middle;
    unsigned long long start_cycles, end_cycles;
    unsigned long long prime_gen_cycles[10000];
    unsigned long long min_cycles = ULLONG_MAX, max_cycles = 0, sum_cycles = 0;
    double avg_cycles;

    // Initialize GMP variables
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));
    mpz_inits(p, q, N, phi_N, e, d, m, c, m_prime, middle, NULL);

    // Step 1: Prime Number Generation (10,000 iterations)
    fprintf(output_file, "\n=== Prime Generation (%d-bit) ===\n", bit_size);
    for (int i = 0; i < 10000; i++) {
        start_cycles = get_clock_cycles();

        // Generate p with MSB=1, LSB=1, middle bits random
        mpz_set_ui(p, 0);
        mpz_setbit(p, bit_size - 1);
        mpz_setbit(p, 0);
        mpz_urandomb(middle, state, bit_size - 2);
        mpz_mul_2exp(middle, middle, 1);
        mpz_add(p, p, middle);
        mpz_nextprime(p, p);

        // Generate q with MSB=1, LSB=1, middle bits random
        mpz_set_ui(q, 0);
        mpz_setbit(q, bit_size - 1);
        mpz_setbit(q, 0);
        mpz_urandomb(middle, state, bit_size - 2);
        mpz_mul_2exp(middle, middle, 1);
        mpz_add(q, q, middle);
        mpz_nextprime(q, q);

        end_cycles = get_clock_cycles();
        
        prime_gen_cycles[i] = end_cycles - start_cycles;
        sum_cycles += prime_gen_cycles[i];
        if (prime_gen_cycles[i] < min_cycles) min_cycles = prime_gen_cycles[i];
        if (prime_gen_cycles[i] > max_cycles) max_cycles = prime_gen_cycles[i];
    }
    
    avg_cycles = (double)sum_cycles / 10000;
    fprintf(output_file, "Minimum clock cycles: %llu\n", min_cycles);
    fprintf(output_file, "Maximum clock cycles: %llu\n", max_cycles);
    fprintf(output_file, "Average clock cycles: %.2f\n", avg_cycles);

    // Step 2: Compute RSA Modulus and Euler's Totient
    start_cycles = get_clock_cycles();
    mpz_mul(N, p, q);                    // N = p * q
    end_cycles = get_clock_cycles();
    unsigned long long modulus_cycles = end_cycles - start_cycles;

    start_cycles = get_clock_cycles();
    mpz_sub_ui(p, p, 1);                // p-1
    mpz_sub_ui(q, q, 1);                // q-1
    mpz_mul(phi_N, p, q);               // phi(N) = (p-1)*(q-1)
    end_cycles = get_clock_cycles();
    unsigned long long totient_cycles = end_cycles - start_cycles;

    fprintf(output_file, "\n=== Modulus and Totient (%d-bit) ===\n", bit_size);
    fprintf(output_file, "Modulus (N = p*q) clock cycles: %llu\n", modulus_cycles);
    fprintf(output_file, "Totient (phi(N)) clock cycles: %llu\n", totient_cycles);

    // Step 3: Public and Private Key Generation
    mpz_set_ui(e, 65537);               // e = 2^16 + 1
    start_cycles = get_clock_cycles();
    mpz_invert(d, e, phi_N);           // Compute d such that e*d ≡ 1 mod phi(N)
    end_cycles = get_clock_cycles();
    fprintf(output_file, "\n=== Key Generation (%d-bit) ===\n", bit_size);
    fprintf(output_file, "Clock cycles: %llu\n", end_cycles - start_cycles);

    // Step 4: Message Encryption and Decryption
    mpz_urandomb(m, state, 1023);      // Generate 1023-bit message
    start_cycles = get_clock_cycles();
    mpz_powm(c, m, e, N);              // c = m^e mod N
    mpz_powm(m_prime, c, d, N);        // m' = c^d mod N
    end_cycles = get_clock_cycles();
    
    fprintf(output_file, "\n=== Encryption/Decryption (%d-bit) ===\n", bit_size);
    fprintf(output_file, "Clock cycles: %llu\n", end_cycles - start_cycles);
    
    // Verify decryption
    int verify = mpz_cmp(m, m_prime) == 0;
    fprintf(output_file, "Decryption verification: %s\n", verify ? "Success" : "Failed");

    // Additional: Sizes for "message complexity" (bit lengths)
    fprintf(output_file, "\n=== Sizes (Bit Lengths) for %d-bit Primes ===\n", bit_size);
    fprintf(output_file, "Modulus N: %zu bits\n", mpz_sizeinbase(N, 2));
    fprintf(output_file, "Public exponent e: %zu bits\n", mpz_sizeinbase(e, 2));
    fprintf(output_file, "Private exponent d: %zu bits\n", mpz_sizeinbase(d, 2));
    fprintf(output_file, "Message m: %zu bits\n", mpz_sizeinbase(m, 2));
    fprintf(output_file, "Ciphertext c: %zu bits\n", mpz_sizeinbase(c, 2));

    // Additional: Theoretical Time and Memory Complexity
    fprintf(output_file, "\n=== Theoretical Time and Memory Complexity (k = bit_size of primes, N ~ 2k bits) ===\n");
    fprintf(output_file, "Prime Generation (per prime):\n");
    fprintf(output_file, "  Time: O(k^4) expected (probabilistic primality testing)\n");
    fprintf(output_file, "  Memory: O(k) for p, q, and temporary GMP variables\n");
    fprintf(output_file, "Modulus (N = p*q):\n");
    fprintf(output_file, "  Time: O(k^2) for multiplication of two k-bit numbers\n");
    fprintf(output_file, "  Memory: O(k) for N and temporary GMP variables\n");
    fprintf(output_file, "Totient (phi(N) = (p-1)*(q-1)):\n");
    fprintf(output_file, "  Time: O(k^2) for subtraction and multiplication\n");
    fprintf(output_file, "  Memory: O(k) for phi_N and temporary GMP variables\n");
    fprintf(output_file, "Key Generation (invert):\n");
    fprintf(output_file, "  Time: O(k^3) for extended Euclidean algorithm\n");
    fprintf(output_file, "  Memory: O(k) for d and temporary GMP variables\n");
    fprintf(output_file, "Encryption (m^e mod N):\n");
    fprintf(output_file, "  Time: O(k^2) since e is small (17 bits)\n");
    fprintf(output_file, "  Memory: O(k) for c and temporary GMP variables\n");
    fprintf(output_file, "Decryption (c^d mod N):\n");
    fprintf(output_file, "  Time: O(k^3) since d is large (~2k bits)\n");
    fprintf(output_file, "  Memory: O(k) for m_prime and temporary GMP variables\n");

    // Clean up
    mpz_clears(p, q, N, phi_N, e, d, m, c, m_prime, middle, NULL);
    gmp_randclear(state);
}

int main() {
    FILE *output_file = fopen("rsa_results.txt", "w");
    if (!output_file) {
        printf("Error opening output file\n");
        return 1;
    }

    // Perform RSA operations for different key sizes
    rsa_operations(512, output_file);
    rsa_operations(768, output_file);
    rsa_operations(1024, output_file);

    fclose(output_file);
    printf("Results written to rsa_results.txt\n");
    return 0;
}
