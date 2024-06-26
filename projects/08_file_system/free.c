#include "free.h"
#include <stdio.h>

#define BLOCK_SIZE 4096

int find_low_clear_bit(unsigned char x)
{
    for (int i = 0; i < 8; i++)
        if (!(x & (1 << i)))
            return i;

    return -1;
}

/* Set a specific bit to the value in set (0 or 1) */
void set_free(unsigned char *block, int num, int set)
{
    int byte_index = num / 8; // 8 bits per byte
    int bit_index = num % 8;

    int byte = block[byte_index];

    block[byte_index] =
        (byte & ~(1 << bit_index)) | (set << bit_index);
}

/* Find a 0 bit and return its index */
int find_free(unsigned char *block)
{
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        int bit = find_low_clear_bit(block[i]);
        if (bit != -1)
            return (i * 8 + bit);
    }
    return -1;
}