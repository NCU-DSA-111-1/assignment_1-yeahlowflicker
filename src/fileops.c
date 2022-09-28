/*

fileops.c: Handle file operations for data read/write

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the
use of this software.

This project is created for the 1st assignemnt of the Data Structure
class (Fall 2022) of the NCU Communication Engineering Department.

The project is built and owned by Cheng Yu Hin (Stu. no. 110503534).
It may not actively maintained and should be kept private whenever possible.

*/

#include "fileops.h"

#include <stdio.h>
#include <assert.h>

//  Assign reference to file and assert file's existence
FILE* initialize_file(FILE *file, char* fileName, char* fileMode) {
    file = fopen(fileName, fileMode);
    assert(file != NULL);
    return file;
}


//  Retrieve dataset from dataset file
//  then insert the data to inputs and outputs array
void load_datasets(FILE* datasetFile, double** inputs, double* outputs, int dataset_count, int single_input_size) {
    assert(datasetFile != NULL);

    for (int i = 0; i < dataset_count; ++i) {
        for (int j = 0; j < single_input_size; ++j)
            fscanf(datasetFile, "%lf", (*(inputs + i) + j));
        fscanf(datasetFile, "%lf", (outputs + i));
    }
}
