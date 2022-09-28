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
