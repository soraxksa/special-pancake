#ifndef UTILS_H
#define UTILS_H

#include <cstdint>

bool is_dir(const char *path);
bool is_file(const char *path);
void create_file(const char *filename, const char *data, uint64_t size);
char* read_file(const char *filename, uint64_t *size);

#endif
