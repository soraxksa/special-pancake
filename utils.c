#include "utils.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

bool is_dir(const char *path)
{
    struct stat stat_buf = {0};
    if(stat(path, &stat_buf) != 0)
        return 0;
    return S_ISDIR(stat_buf.st_mode);
}

bool is_file(const char *path)
{
    struct stat stat_buf = {0};
    if(stat(path, &stat_buf) != 0)
        return 0;
    return S_ISREG(stat_buf.st_mode);
}
 
void create_file(const char *filename, const char *data, size_t size)
{
    int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    write(fd, data, size);
    close(fd);
}
 
char* read_file(const char *filename, size_t *size)
{
    int fd = open(filename, O_RDONLY);
    if(fd == -1)
        return NULL;

    struct stat info;
    fstat(fd, &info);
    *size = info.st_size;
    char *data = (char*) malloc(*size);
    read(fd, data, *size);

    close(fd);
    return data;
}
 
