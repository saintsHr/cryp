#include "util.h"

void PRNG(const unsigned char *smallKey, unsigned int smallLen, unsigned char *largeKey, unsigned long long largeLen) {
    unsigned int seed = 0;
    for (int i = 0; i < smallLen; i++){
        seed = seed * 31 + smallKey[i];
    }

    srand(seed);

    for (unsigned long long i = 0; i < largeLen; i++){
        largeKey[i] = rand() & 0xFF;
    }
}