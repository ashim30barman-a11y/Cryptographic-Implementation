#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 256

void swap(unsigned char *a, unsigned char *b) {
    unsigned char temp = *a;
    *a = *b;
    *b = temp;
}

void ksa(unsigned char *S, const unsigned char *key, int key_len, clock_t *ksa_clocks) {
    clock_t start = clock();
    for (int i = 0; i < N; i++) {
        S[i] = i;
    }
    int j = 0;
    for (int i = 0; i < N; i++) {
        j = (j + S[i] + key[i % key_len]) % N;
        swap(&S[i], &S[j]);
    }
    *ksa_clocks = clock() - start;
}

void prga(unsigned char *S, unsigned char *plaintext, unsigned char *ciphertext, int len, clock_t *prga_clocks) {
    clock_t start = clock();
    int i = 0, j = 0;
    for (int k = 0; k < len; k++) {
        i = (i + 1) % N;
        j = (j + S[i]) % N;
        swap(&S[i], &S[j]);
        int t = (S[i] + S[j]) % N;
        ciphertext[k] = plaintext[k] ^ S[t];
    }
    *prga_clocks = clock() - start;
}

int main() {
    unsigned char S[N];
    const unsigned char *key = (unsigned char *)"SecretKey";
    int key_len = strlen((char *)key);
    unsigned char plaintext[] = "Hello, RC4!";
    int plaintext_len = strlen((char *)plaintext);
    unsigned char ciphertext[plaintext_len];
    
    clock_t ksa_clocks, prga_clocks;

    ksa(S, key, key_len, &ksa_clocks);
    prga(S, plaintext, ciphertext, plaintext_len, &prga_clocks);

    printf("Plaintext: %s\n", plaintext);
    printf("Ciphertext (hex): ");
    for (int i = 0; i < plaintext_len; i++) {
        printf("%02x ", ciphertext[i]);
    }
    printf("\nKSA Clock Ticks: %ld\n", ksa_clocks);
    printf("PRGA Clock Ticks: %ld\n", prga_clocks);
    printf("Total Clock Ticks: %ld\n", ksa_clocks + prga_clocks);

    return 0;
}
