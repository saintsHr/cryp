#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void generateOutputName(const char *input, char *output, size_t maxLen);
void generateOutputNameDec(const char *input, char *output, size_t maxLen);
void PRNG(const unsigned char *smallKey, unsigned int smallLen, unsigned char *largeKey, unsigned long long largeLen);