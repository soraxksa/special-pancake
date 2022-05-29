#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "mutations.h"


void create_file(const char *filename, const char *data, size_t size)
{
    int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    write(fd, data, size);
    close(fd);
}

int fuzz(char* data_filename)
{
    const char *prog_filename = "./test";
    pid_t pid = fork();
    char *argv_[3] = {"./test", data_filename, NULL};
    if(pid == -1)
    {
        printf("ERROR: fork()\n");
        exit(-1);
    }
    if(pid == 0) //child
    {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execv(prog_filename, argv_); 
    }
    else
    {
        int wait_status, options = 0;
        waitpid(pid, &wait_status, options); // wait for the program to finish launching.
        ptrace(PTRACE_CONT, pid, NULL, NULL);

        while(1) 
        {
            int optinos = 0, wait_status;
            waitpid(pid, &wait_status, optinos);
            if (WIFEXITED(wait_status)) {
                //printf("[!] exited, status=%d\n", WEXITSTATUS(wait_status));
                break;
            }
            if(WIFSIGNALED(wait_status))
            {
                return 1;
            }
            if(WIFSTOPPED(wait_status))
            {
                // signal 127
                return 2;
            }

        }

    }

    return 0;
}

char* mutate(char *data, size_t *size)
{
    //TODO maybe we shouldnt copy, we could remember where we changed the data
    //then after the fuzzing de-change it back.
    
    int fun_idx = rand() % MUTATIONS_LEN;
    //printf("mutation methods:%d\n", fun_idx);
    char *mutated_data = mutations[fun_idx](data, size);
    
    return mutated_data;
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

void usage(char** argv) 
{
    printf("Usage: %s <image.jpg>\n", argv[0]);
}

int main(int argc, char **argv) 
{
    if(argc == 1) 
    {
        usage(argv);
        exit(-1);
    }

    srand(time(0));
    const char *filename = argv[1];
    size_t file_size;
    char *data = read_file(filename, &file_size);
    if(data == NULL)
    {
        printf("ERROR: reading the file:%s\n", filename);
        exit(-1);
    }

    for(int cnt = 0; cnt < 2020; cnt++)
    {
        char *mutated_data = mutate(data, &file_size);
        //printf("[!] data:%s\n[!] mutated_data:%s\n", data, mutated_data);
        char mutated_filename[20] = "crashs/c";
        snprintf(mutated_filename+8, 10, "%d", cnt);  
        create_file(mutated_filename, mutated_data, file_size);
        int result = fuzz(mutated_filename);
        char mutated_filename2[20];
        switch(result)
        {
            case 1:
                strcpy(mutated_filename2, "crashed/k");
                snprintf(mutated_filename2+9, 10, "%d", cnt); 
                create_file(mutated_filename2, mutated_data, file_size);
                break;                                       
            case 2:
                strcpy(mutated_filename2, "crashed/c");
                snprintf(mutated_filename2+9, 10, "%d", cnt); 
                create_file(mutated_filename2, mutated_data, file_size);
                break;
        }

        free(mutated_data);

        if((cnt+1) % 1000 == 0)
        {
            system("rm crashs/c*");
        }
    }

    return 0;
}
