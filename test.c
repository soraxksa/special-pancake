#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>




int main(int argc, char **argv)
{
    printf("argv[0]:%s\nargv[1]:%s\n",argv[0], argv[1]);
    int fd = open(argv[1], O_RDONLY);
    char buf[100];
    read(fd, buf, 100);
    printf("test: buf:%s\n", buf);
    if(buf[0] == 0xFFFFFFFF)
    {
        printf("deref null\n");
        int *x = 0;
        *x = 1;
    }
    return 0;
}
