
#include "mutations.h"



char* flip_bits(char *data, size_t *size)
{
    char *mutated_data = malloc(*size);
    memcpy(mutated_data, data, *size);
    size_t index = rand() % *size;
    mutated_data[index] ^= flip_array[index % 8];
    return mutated_data;
}


char* magic(char *data, size_t *size)
{
    char *mutated_data = malloc(*size);
    memcpy(mutated_data, data, *size);
    size_t magic_idx  = rand() % MAGIC_LEN;
    size_t magic_size = magic_length[magic_idx];
    size_t change_idx = rand() % ((*size) - magic_size); //TODO() check for neg mod

    //printf("[!] change_idx:%lu\n[!] magic_size:%lu\n", change_idx, magic_size);
    for(size_t i = 0; i < magic_size; i++)
    {
        mutated_data[change_idx + i] = magic_array[magic_idx][i];
    }

    return mutated_data;
}

