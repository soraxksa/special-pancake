#ifndef CORPUS_H
#define CORPUS_H

#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;

class test_case{
public:

    string filename;
    char *data;
    size_t data_len;

    test_case(const string &filename, size_t data_len):
        filename{filename},
        data{nullptr},
        data_len{data_len}
    {
        //cout << "test_case " << filename << " created\n";
        ifstream input_stream(filename, ios::binary);
        //cout << "size:" << data_len << "\n";
        streamsize size{static_cast<streamsize>(data_len)};
        data = (char*) malloc(data_len);
        input_stream.read(data, size);
    }

    ~test_case()
    {
        //cout << "deleted:" << this->filename << "\n";
        if(this->data){
            free(this->data);
            data = NULL;
        }
    }

};
 


#endif
