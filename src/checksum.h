/*

checksum.c: Request user input and calculate a checksum using the trained
neural network

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the
use of this software.

This project is created for the 1st assignemnt of the Data Structure
class (Fall 2022) of the NCU Communication Engineering Department.

The project is built and owned by Cheng Yu Hin (Stu. no. 110503534).
It may not actively maintained and should be kept private whenever possible.

*/

#ifndef CHECKSUM
#define CHECKSUM
#endif

#include <stdio.h>
#include "genann.h"

//  Takes an input array and obtain the checksum
double evaluate_checksum(genann* neural_network, double* input, int check_input_size);

//  The entry point to the checksum algorithm
double obtain_checksum(genann* neural_network, int max_test_input_length);
