#ifndef MUTATIONS_H
#define MUTATIONS_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MUTATIONS_LEN sizeof(mutations)/sizeof(mutations[0])
#define MAGIC_LEN sizeof(magic_array)/sizeof(magic_array[0])

char* flip_bits(char *data, size_t *size);
char* magic(char* data, size_t *size);

static char* (*mutations[])(char *data, size_t *size)= {
    flip_bits,
    magic
};

static int flip_array[] = {1, 2, 4, 8, 16, 32, 64, 128};

static const char *magic_array[] = {
    "\x00",
    "\xFF",
    "\x00\x00",
    "\xFF\xFF",
    "\x00\x00\x00\x00",
    "\xFF\xFF\xFF\xFF",
    "\x00\x00\x00\x00\x00\x00\x00\x00",
    "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF",
};

static size_t magic_length[] = {
    1,
    1,
    2,
    2,
    4,
    4,
    8,
    8
};

static void printf_hex(char *data, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        printf("%lu:%hhx ", i, data[i]);
    }
}

#endif
