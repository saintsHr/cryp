#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void PRNG(const unsigned char *smallKey, unsigned int smallLen, unsigned char *largeKey, unsigned long long largeLen);