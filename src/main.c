#include <stdio.h>
#include <stdlib.h>

#include "encryps.h"
#include "util.h"

void decryp(unsigned char *key, unsigned char *buffer, unsigned long long size){
    unsigned char rKey[size];
    PRNG(key, 10, rKey, size);

    encrypXOR(buffer, size, rKey);
}

int main(int argc, char *argv[]){
    // checks for correct usage
    if (argc < 2 || argc > 3){
        printf("Usage: cryp <file>\n");
        return 1;
    }

    char *path = argv[1];

    // try to open the file
    FILE *file = fopen(path, "rb");

    // checks if given file exists
    if (!file){
        printf("Error: file does not exists or is a directory\n");
        return 0;
    }

    // gets the file size
    fseek(file, 0, SEEK_END);
    unsigned long long size = ftell(file);
    rewind(file);

    if (size <= 0){
        printf("Error: file is empty\n");
        fclose(file);
        return -1;
    }

    // creates the read buffer
    unsigned char *bufferRead = malloc(size);
    if (!bufferRead){
        printf("Error: memory allocation failed\n");
        fclose(file);
        return -1;
    }

    size_t read = fread(bufferRead, 1, size, file);
    fclose(file);

    if (read != size){
        printf("Error: cannot read file");
        free(bufferRead);
        return -1;
    }

    // Creates the small key
    unsigned int sKeySize = 10;
    unsigned char *sKey = malloc(size);
    if (!sKey) return 1;

    // fills the small key
    for (unsigned long long i = 0; i < sKeySize; i++) {
        sKey[i] = rand() & 0xFF;
    }

    // creates and fill long key
    unsigned char lKey[size];
    PRNG(sKey, sKeySize, lKey, size);

    // creates output file
    char outputName[256];
    FILE *output = fopen(path, "wb");

    // decrypts if user gives an valid key
    if (argc == 3 && memcmp(sKey, argv[2], 10) == 0){
        FILE *output = fopen(path, "wb");

        unsigned char *bufferWrite = malloc(size);
        decryp(sKey, bufferWrite, size);

        fwrite(bufferWrite, 1, size, output);

        fclose(output);
        return 0;
    }

    // encrypts
    unsigned char *bufferWrite = malloc(size);
    memcpy(bufferWrite, bufferRead, size);
    encrypXOR(bufferWrite, size, lKey);

    fwrite(bufferWrite, 1, size, output);
    fclose(output);

    // shows the key to the user
    printf("Key: ");
    for (unsigned int i = 0; i < 10; i++){
        printf("%02X", sKey[i]);
    }
    printf("\n");

    free(bufferWrite);
    free(bufferRead);
    return 0;
}