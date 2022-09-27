//  Run "gcc main.c -o main.out -lm fileops.c -lm training.c -lm checksum.c -lm genann.c -lm" to compile

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "fileops.h"
#include "training.h"
#include "checksum.h"
#include "genann.h"


#define SINGLE_INPUT_SIZE 2
#define SINGLE_OUTPUT_SIZE 1
#define DATASET_COUNT 4
#define HIDDEN_LAYER_COUNT 1
#define SINGLE_HIDDEN_LAYER_NEURON_COUNT 2
#define TRAINING_ITERATIONS 100000
#define LEARNING_RATE 0.3
#define LOG_ERROR_INTERVAL 100
#define MAX_TEST_INPUT_LENGTH 100


//  Global file pointers
FILE* datasetFile = NULL;
FILE* outputFile = NULL;


//  Release the memory allocated by the neural network and the arrays
void release_memory(genann* neural_network, double** inputs, double* outputs) {
    genann_free(neural_network);
    free(inputs);
    free(outputs);
}


//  Main program
int main(int argc, char *argv[]) {
    //  Initialize the neural network randomly, using the current time
    srand(time(0));

    //  Define and initialize input 2D array
    double** inputs = (double**)malloc(sizeof(double*) * DATASET_COUNT);
    for (int i = 0; i < DATASET_COUNT; ++i)
        *(inputs + i) = (double*)malloc(sizeof(double) * SINGLE_INPUT_SIZE);

    //  Define and initialize output array
    double* outputs = (double*)malloc(sizeof(double) * SINGLE_OUTPUT_SIZE);

    //  Define and initialize files
    datasetFile = initialize_file(datasetFile, "dataset.txt", "r");
    outputFile = initialize_file(outputFile, "output.txt", "w+");

    //  Load given datasets from file
    load_datasets(datasetFile, inputs, outputs, DATASET_COUNT, SINGLE_INPUT_SIZE);

    // Define the neural network
    genann *neural_network = genann_init(SINGLE_INPUT_SIZE, HIDDEN_LAYER_COUNT, SINGLE_HIDDEN_LAYER_NEURON_COUNT, SINGLE_OUTPUT_SIZE);

    //  Train the neural network
    train_neural_network(neural_network, inputs, outputs, outputFile, TRAINING_ITERATIONS, LEARNING_RATE, DATASET_COUNT, LOG_ERROR_INTERVAL);

    //  Calculate and output the checksum
    double obtained_checksum = obtain_checksum(neural_network, MAX_TEST_INPUT_LENGTH);
    printf("\nChecksum result:\n%.0f\n", round(obtained_checksum));

    //  After operation, release the allocated memory
    release_memory(neural_network, inputs, outputs);

    return 0;
}
