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

#ifndef FILEOPS
#define FILEOPS
#endif

#include <stdio.h>

//  Assign reference to file and assert file's existence
FILE* initialize_file(FILE *file, char* fileName, char* fileMode);

//  Retrieve dataset from dataset file
//  then insert the data to inputs and outputs array
void load_datasets(FILE* datasetFile, double** inputs, double* outputs, int dataset_count, int single_input_size);
