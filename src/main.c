#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "encryps.h"
#include "util.h"

static bool hex_to_bytes(const char *hex, unsigned char *out, size_t out_len) {
    if (strlen(hex) < out_len * 2) return false;
    for (size_t i = 0; i < out_len; i++) {
        if (!isxdigit(hex[2*i]) || !isxdigit(hex[2*i+1])) return false;
        unsigned int byte;
        if (sscanf(hex + 2*i, "%2x", &byte) != 1) return false;
        out[i] = (unsigned char)byte;
    }
    return true;
}

static bool key_equal(const unsigned char *a, const unsigned char *b, size_t len) {
    unsigned char diff = 0;
    for (size_t i = 0; i < len; i++) diff |= a[i] ^ b[i];
    return diff == 0;
}

bool verify_key(const unsigned char *sKey, size_t key_len, const char *user_hex) {
    unsigned char userKey[key_len];
    if (!hex_to_bytes(user_hex, userKey, key_len)) return false;
    return key_equal(sKey, userKey, key_len);
}

void decryp(unsigned char *key, unsigned char *buffer, unsigned long long size){
    unsigned char *rKey = malloc(size);
    if (!rKey) return;
    PRNG(key, 10, rKey, size);
    encrypXOR(buffer, size, rKey);
    free(rKey);
}

int main(int argc, char *argv[]){
    srand((unsigned int)time(NULL));

    // checks for correct usage
    if (argc < 2 || argc > 3){
        printf("Usage: cryp <file> [key]\n");
        return 1;
    }

    char *path = argv[1];

    // try to open the file
    FILE *file = fopen(path, "rb");

    // checks if given file exists
    if (!file){
        printf("Error: file does not exists or is a directory\n");
        return 1;
    }

    // gets the file size
    if (fseek(file, 0, SEEK_END) != 0) {
        printf("Error: cannot seek file\n");
        fclose(file);
        return 1;
    }
    unsigned long long size = ftell(file);
    if (size <= 0) {
        printf("Error: file is empty\n");
        fclose(file);
        return 1;
    }
    rewind(file);

    // creates the read buffer
    unsigned char *bufferRead = malloc(size);
    if (!bufferRead){
        printf("Error: memory allocation failed\n");
        fclose(file);
        return 1;
    }

    size_t read = fread(bufferRead, 1, size, file);
    fclose(file);

    if (read != size){
        printf("Error: cannot read file\n");
        free(bufferRead);
        return 1;
    }

    // creates the small key
    unsigned int sKeySize = 10;
    unsigned char *sKey = malloc(sKeySize);
    if (!sKey) {
        free(bufferRead);
        return 1;
    }

    // if user gives key, convert and get it
    if (argc == 3) {
        if (!hex_to_bytes(argv[2], sKey, sKeySize)) {
            printf("Error: invalid key format\n");
            free(bufferRead);
            free(sKey);
            return 1;
        }
    } else {
        // fills the small key randomly
        for (unsigned int i = 0; i < sKeySize; i++) {
            sKey[i] = rand() & 0xFF;
        }
    }

    // creates and fills long key
    unsigned char *lKey = malloc(size);
    if (!lKey) {
        free(bufferRead);
        free(sKey);
        return 1;
    }
    PRNG(sKey, sKeySize, lKey, size);

    // creates output buffer
    unsigned char *bufferWrite = malloc(size);
    if (!bufferWrite) {
        free(bufferRead);
        free(sKey);
        free(lKey);
        return 1;
    }
    memcpy(bufferWrite, bufferRead, size);

    // decrypts if user gives a valid key
    if (argc == 3){
        decryp(sKey, bufferWrite, size);
    } else {
        // encrypts
        encrypXOR(bufferWrite, size, lKey);
    }

    // write output file
    FILE *output = fopen(path, "wb");
    if (!output) {
        printf("Error: cannot open output file\n");
        free(bufferRead);
        free(bufferWrite);
        free(sKey);
        free(lKey);
        return 1;
    }
    fwrite(bufferWrite, 1, size, output);
    fclose(output);

    // shows the key to the user
    if (argc == 2){
        printf("Key: ");
        for (unsigned int i = 0; i < sKeySize; i++){
            printf("%02X", sKey[i]);
        }
        printf("\n");
    }

    free(bufferWrite);
    free(bufferRead);
    free(sKey);
    free(lKey);

    return 0;
}