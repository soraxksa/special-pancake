#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
//#include <fstream>
#include <utility>

#include "mutations.h"
#include "utils.h"
#include "corpus.h"

namespace fs = std::filesystem;
using namespace std;

int fuzz(char* data_filename)
{
    char prog_filename[] = "./test";
    pid_t pid = fork();
    char *argv_[3] = {prog_filename, data_filename, NULL};
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

void usage(char** argv) 
{
    printf("Usage: %s <image.jpg>\n", argv[0]);
}

const char *input_path = NULL;
const char *output_dir = "./tmp";

int main(int argc, char **argv) //argv[0] -i input_dir  program_name  @@
{

    if(argc == 1) 
    {
        usage(argv);
        exit(-1);
    }

    input_path = argv[1];
    
    fs::path input_dir{input_path};

    if(!fs::is_directory(fs::status(input_dir)))
    {
        printf("ERROR: not a dir\n");
        exit(-1);
    }

    
    vector<unique_ptr<test_case> > corpus{};
    for(auto const& dir_entry : fs::directory_iterator{input_dir})
    {
        if(is_regular_file(dir_entry))
        {
            corpus.push_back(unique_ptr<test_case>(new test_case(dir_entry.path(), dir_entry.file_size())));
        }
    }

    srand(time(0));
    uint64_t cnt = 0;
    while(1)
    {

        size_t indx = rand() % corpus.size();
        size_t file_size = corpus.at(indx)->data_len;
        char *data = corpus.at(indx)->data;

        char *mutated_data = mutate(data, &file_size);
        //printf("[!] data:%s\n[!] mutated_data:%s\n", data, mutated_data);
        char mutated_filename[20] = "crashs/c";
        snprintf(mutated_filename+8, 10, "%lu", cnt);  
        create_file(mutated_filename, mutated_data, file_size);
        int result = fuzz(mutated_filename);
        char mutated_filename2[20];
        switch(result)
        {
            case 1:
                strcpy(mutated_filename2, "crashed/k");
                snprintf(mutated_filename2+9, 10, "%lu", cnt); 
                create_file(mutated_filename2, mutated_data, file_size);
                break;                                       
            case 2:
                strcpy(mutated_filename2, "crashed/c");
                snprintf(mutated_filename2+9, 10, "%lu", cnt); 
                create_file(mutated_filename2, mutated_data, file_size);
                break;
        }

        free(mutated_data);

        if((cnt+1) % 1000 == 0)
        {
            system("rm crashs/c*");
        }                                            
        cnt++;
    }


    return 0;
}
