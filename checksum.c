#include "checksum.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "genann.h"


//  Takes an input array and obtain the checksum
double evaluate_checksum(genann* neural_network, double* input, int check_input_size) {
    //  An array of double with length 2
    //  first element: current checksum
    //  second element: current input digit
    double* checksum = malloc(sizeof(double) * 2);
    *(checksum + 0) = *(checksum + 1) = 0;

    //  Print the list headers
    printf("\n===================\n");
    printf("Checksum | Input\n-------------------\n");

    for (int i = 0; i < check_input_size; ++i) {
        //  Assign the current input digit to the second element
        *(checksum + 1) = *(input + i);

        //  Log the XOR process
        printf("     %.0f   ^   %.0f\n", round(*(checksum + 0)), round(*(checksum + 1)));

        //  Obtain XOR result using the neural network, then store the result to the first element
        *(checksum + 0) = *genann_run(neural_network, checksum);
    }
    printf("===================\n");

    free(input);

    //  Return the first element, which is the final checksum value
    return round(*(checksum + 0));
}


//  The entry point to the checksum algorithm
double obtain_checksum(genann* neural_network, int max_test_input_length) {
    //  Define char array and request user input
    char* test_input = malloc(max_test_input_length * sizeof(char));
    printf("\n\nPlease enter a binary value for the system to calculate the checksum: ");
    scanf("%s", test_input);

    //  Compute the length of the user input using strlen()
    int test_input_length = strlen(test_input);

    //  Convert user's string input argument to integer array
    double* check_input = (double*)malloc(sizeof(double) * test_input_length);
    for (int i = 0; i < test_input_length; ++i)
        *(check_input + i) = test_input[i] == '0' ? 0 : 1;

    //  Release the memory allocated by the input string
    free(test_input);

    //  Calculate the checksum and return the result
    return evaluate_checksum(neural_network, check_input, test_input_length);
}
