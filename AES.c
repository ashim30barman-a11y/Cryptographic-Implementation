#include <stdio.h>
#include <string.h>

// S-box
const unsigned char sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

// Inverse S-box
const unsigned char inv_sbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

// Rcon
const unsigned char rcon[11] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

// Function prototypes to avoid implicit declaration warnings
void expand_key(const unsigned char *key, unsigned char *expanded);
unsigned char gf_mul(unsigned char a, unsigned char b);
void sub_bytes(unsigned char state[4][4]);
void inv_sub_bytes(unsigned char state[4][4]);
void shift_rows(unsigned char state[4][4]);
void inv_shift_rows(unsigned char state[4][4]);
void mix_columns(unsigned char state[4][4]);
void inv_mix_columns(unsigned char state[4][4]);
void add_round_key(unsigned char state[4][4], const unsigned char *expanded, int round);
void aes_encrypt_block(const unsigned char *in, unsigned char *out, const unsigned char *key);
void aes_decrypt_block(const unsigned char *in, unsigned char *out, const unsigned char *key);
size_t aes_ecb_encrypt(const unsigned char *in, size_t in_len, unsigned char *out, const unsigned char *key);
size_t aes_ecb_decrypt(const unsigned char *in, size_t in_len, unsigned char *out, const unsigned char *key);
void print_hex(const char *label, const unsigned char *data, size_t len);

// GF(2^8) multiplication
unsigned char gf_mul(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    unsigned char hi_bit_set;
    for (int counter = 0; counter < 8; counter++) {
        if (b & 1) p ^= a;
        hi_bit_set = a & 0x80;
        a <<= 1;
        if (hi_bit_set) a ^= 0x1b;
        b >>= 1;
    }
    return p;
}

// Key expansion
void expand_key(const unsigned char *key, unsigned char *expanded) {
    memcpy(expanded, key, 16);
    for (int i = 4; i < 44; i++) {
        unsigned char temp[4];
        memcpy(temp, expanded + (i - 1) * 4, 4);
        if (i % 4 == 0) {
            // Rotate
            unsigned char t = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = t;
            // SubBytes
            temp[0] = sbox[temp[0]];
            temp[1] = sbox[temp[1]];
            temp[2] = sbox[temp[2]];
            temp[3] = sbox[temp[3]];
            // Rcon
            temp[0] ^= rcon[i / 4];
        }
        expanded[i * 4 + 0] = expanded[(i - 4) * 4 + 0] ^ temp[0];
        expanded[i * 4 + 1] = expanded[(i - 4) * 4 + 1] ^ temp[1];
        expanded[i * 4 + 2] = expanded[(i - 4) * 4 + 2] ^ temp[2];
        expanded[i * 4 + 3] = expanded[(i - 4) * 4 + 3] ^ temp[3];
    }
}

// SubBytes
void sub_bytes(unsigned char state[4][4]) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            state[r][c] = sbox[state[r][c]];
        }
    }
}

// Inverse SubBytes
void inv_sub_bytes(unsigned char state[4][4]) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            state[r][c] = inv_sbox[state[r][c]];
        }
    }
}

// ShiftRows
void shift_rows(unsigned char state[4][4]) {
    for (int r = 1; r < 4; r++) {
        for (int s = 0; s < r; s++) {
            unsigned char temp = state[r][0];
            state[r][0] = state[r][1];
            state[r][1] = state[r][2];
            state[r][2] = state[r][3];
            state[r][3] = temp;
        }
    }
}

// Inverse ShiftRows
void inv_shift_rows(unsigned char state[4][4]) {
    for (int r = 1; r < 4; r++) {
        for (int s = 0; s < r; s++) {
            unsigned char temp = state[r][3];
            state[r][3] = state[r][2];
            state[r][2] = state[r][1];
            state[r][1] = state[r][0];
            state[r][0] = temp;
        }
    }
}

// MixColumns
void mix_columns(unsigned char state[4][4]) {
    for (int c = 0; c < 4; c++) {
        unsigned char col[4] = {state[0][c], state[1][c], state[2][c], state[3][c]};
        state[0][c] = gf_mul(0x02, col[0]) ^ gf_mul(0x03, col[1]) ^ col[2] ^ col[3];
        state[1][c] = col[0] ^ gf_mul(0x02, col[1]) ^ gf_mul(0x03, col[2]) ^ col[3];
        state[2][c] = col[0] ^ col[1] ^ gf_mul(0x02, col[2]) ^ gf_mul(0x03, col[3]);
        state[3][c] = gf_mul(0x03, col[0]) ^ col[1] ^ col[2] ^ gf_mul(0x02, col[3]);
    }
}

// Inverse MixColumns
void inv_mix_columns(unsigned char state[4][4]) {
    for (int c = 0; c < 4; c++) {
        unsigned char col[4] = {state[0][c], state[1][c], state[2][c], state[3][c]};
        state[0][c] = gf_mul(0x0e, col[0]) ^ gf_mul(0x0b, col[1]) ^ gf_mul(0x0d, col[2]) ^ gf_mul(0x09, col[3]);
        state[1][c] = gf_mul(0x09, col[0]) ^ gf_mul(0x0e, col[1]) ^ gf_mul(0x0b, col[2]) ^ gf_mul(0x0d, col[3]);
        state[2][c] = gf_mul(0x0d, col[0]) ^ gf_mul(0x09, col[1]) ^ gf_mul(0x0e, col[2]) ^ gf_mul(0x0b, col[3]);
        state[3][c] = gf_mul(0x0b, col[0]) ^ gf_mul(0x0d, col[1]) ^ gf_mul(0x09, col[2]) ^ gf_mul(0x0e, col[3]);
    }
}

// AddRoundKey
void add_round_key(unsigned char state[4][4], const unsigned char *expanded, int round) {
    int offset = round * 16;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            state[r][c] ^= expanded[offset + 4 * c + r];
        }
    }
}

// Encrypt single block
void aes_encrypt_block(const unsigned char *in, unsigned char *out, const unsigned char *key) {
    unsigned char expanded[176];
    expand_key(key, expanded);

    unsigned char state[4][4];
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            state[r][c] = in[4 * c + r];
        }
    }

    add_round_key(state, expanded, 0);
    for (int round = 1; round < 10; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, expanded, round);
    }
    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, expanded, 10);

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            out[4 * c + r] = state[r][c];
        }
    }
}

// Decrypt single block
void aes_decrypt_block(const unsigned char *in, unsigned char *out, const unsigned char *key) {
    unsigned char expanded[176];
    expand_key(key, expanded);

    unsigned char state[4][4];
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            state[r][c] = in[4 * c + r];
        }
    }

    add_round_key(state, expanded, 10);
    for (int round = 9; round > 0; round--) {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(state, expanded, round);
        inv_mix_columns(state);
    }
    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_round_key(state, expanded, 0);

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            out[4 * c + r] = state[r][c];
        }
    }
}

// ECB mode encryption for multiple blocks with PKCS7 padding
size_t aes_ecb_encrypt(const unsigned char *in, size_t in_len, unsigned char *out, const unsigned char *key) {
    size_t block_size = 16;
    size_t padded_len = ((in_len / block_size) + 1) * block_size;
    if (in_len % block_size == 0) {
        padded_len = in_len + block_size;  // Add extra block if no padding needed
    }
    unsigned char block[16];
    size_t pos = 0;
    while (pos < in_len) {
        size_t to_copy = (in_len - pos < block_size) ? (in_len - pos) : block_size;
        memcpy(block, in + pos, to_copy);
        if (to_copy < block_size) {
            unsigned char pad_val = block_size - to_copy;
            memset(block + to_copy, pad_val, pad_val);
            aes_encrypt_block(block, out + pos, key);
            return pos + block_size;  // Actual output length
        } else {
            aes_encrypt_block(block, out + pos, key);
        }
        pos += block_size;
    }
    // If exact multiple, add padding block
    if (in_len % block_size == 0) {
        memset(block, 0x10, 16);
        aes_encrypt_block(block, out + pos, key);
        pos += block_size;
    }
    return pos;
}

// ECB mode decryption for multiple blocks, removes PKCS7 padding
size_t aes_ecb_decrypt(const unsigned char *in, size_t in_len, unsigned char *out, const unsigned char *key) {
    if (in_len % 16 != 0) return 0;  // Invalid length
    unsigned char block[16];
    for (size_t pos = 0; pos < in_len; pos += 16) {
        aes_decrypt_block(in + pos, block, key);
        memcpy(out + pos, block, 16);
    }
    // Remove padding
    unsigned char pad_val = out[in_len - 1];
    if (pad_val == 0 || pad_val > 16) return 0;  // Invalid padding
    for (int i = 1; i < pad_val; i++) {
        if (out[in_len - 1 - i] != pad_val) return 0;  // Invalid
    }
    return in_len - pad_val;
}

void print_hex(const char *label, const unsigned char *data, size_t len) {
    printf("%s: ", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

int main() {
    // Test vectors
    unsigned char plaintext[16] = {
        0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
        0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34
    };
    unsigned char key[16] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    unsigned char expected_ciphertext[16] = {
        0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb,
        0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32
    };
    unsigned char ciphertext[16];
    unsigned char decrypted[16];

    // Test single block encryption
    printf("Testing single block encryption:\n");
    aes_encrypt_block(plaintext, ciphertext, key);
    print_hex("Plaintext", plaintext, 16);
    print_hex("Key", key, 16);
    print_hex("Ciphertext", ciphertext, 16);
    print_hex("Expected Ciphertext", expected_ciphertext, 16);

    // Verify ciphertext
    if (memcmp(ciphertext, expected_ciphertext, 16) == 0) {
        printf("Encryption test passed!\n");
    } else {
        printf("Encryption test failed!\n");
    }

    // Test single block decryption
    aes_decrypt_block(ciphertext, decrypted, key);
    print_hex("Decrypted", decrypted, 16);
    if (memcmp(decrypted, plaintext, 16) == 0) {
        printf("Decryption test passed!\n");
    } else {
        printf("Decryption test failed!\n");
    }

    // Test ECB mode with multiple blocks (example with padding)
    unsigned char multi_block_input[32] = {
        0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
        0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34,
        0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
        0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34
    };
    unsigned char multi_block_output[48]; // Padded to next block size (32 + 16)
    unsigned char multi_block_decrypted[32];
    size_t encrypted_len, decrypted_len;

    printf("\nTesting ECB mode with multiple blocks:\n");
    encrypted_len = aes_ecb_encrypt(multi_block_input, 32, multi_block_output, key);
    print_hex("Multi-block input", multi_block_input, 32);
    print_hex("Multi-block encrypted", multi_block_output, encrypted_len);

    decrypted_len = aes_ecb_decrypt(multi_block_output, encrypted_len, multi_block_decrypted, key);
    print_hex("Multi-block decrypted", multi_block_decrypted, decrypted_len);

    if (decrypted_len == 32 && memcmp(multi_block_input, multi_block_decrypted, 32) == 0) {
        printf("ECB mode test passed!\n");
    } else {
        printf("ECB mode test failed!\n");
    }

    return 0;
}
