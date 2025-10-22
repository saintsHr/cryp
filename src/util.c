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

void generateOutputName(const char *input, char *output, size_t maxLen) {
    const char *ext = strrchr(input, '.');
    size_t baseLen = ext ? (size_t)(ext - input) : strlen(input);
    const char *suffix = "_cryp.bin";
    size_t suffixLen = strlen(suffix);

    if (baseLen + suffixLen >= maxLen) {
        if (maxLen > suffixLen + 1) {
            baseLen = maxLen - suffixLen - 1;
        } else {
            baseLen = 0;
        }
    }

    memcpy(output, input, baseLen);
    output[baseLen] = '\0';

    strncat(output, suffix, maxLen - strlen(output) - 1);
}