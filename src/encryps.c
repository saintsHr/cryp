#include "encryps.h"

void encrypXOR(unsigned char *buffer, unsigned long long size, unsigned char *key){
    for (unsigned long long i = 0; i < size; i++){
        buffer[i] = buffer[i] ^ key[i];
    }
}