#ifndef FILEOPS
#define FILEOPS
#endif

#include <stdio.h>

//  Assign reference to file and assert file's existence
FILE* initialize_file(FILE *file, char* fileName, char* fileMode);

//  Retrieve dataset from dataset file
//  then insert the data to inputs and outputs array
void load_datasets(FILE* datasetFile, double** inputs, double* outputs, int dataset_count, int single_input_size);
