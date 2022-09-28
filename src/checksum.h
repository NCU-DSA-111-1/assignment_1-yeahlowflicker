#ifndef CHECKSUM
#define CHECKSUM
#endif

#include <stdio.h>
#include "genann.h"

//  Takes an input array and obtain the checksum
double evaluate_checksum(genann* neural_network, double* input, int check_input_size);

//  The entry point to the checksum algorithm
double obtain_checksum(genann* neural_network, int max_test_input_length);
