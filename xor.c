//  Run "gcc xor.c -lm genann.c -lm" to compile

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include "genann.h"


#define SINGLE_INPUT_SIZE 2
#define SINGLE_OUTPUT_SIZE 1
#define DATASET_COUNT 4
#define HIDDEN_LAYER_COUNT 1
#define SINGLE_HIDDEN_LAYER_NEURON_COUNT 2
#define TRAINING_ITERATIONS 10000
#define LEARNING_RATE 5.8
#define LOG_ERROR_INTERVAL 100
#define MAX_TEST_INPUT_LENGTH 100


//  Global file pointers
FILE* datasetFile = NULL;
FILE* outputFile = NULL;



//  Assign reference to file and assert file's existence
FILE* initialize_file(FILE *file, char* fileName, char* fileMode) {
    file = fopen(fileName, fileMode);
    assert(file != NULL);
    return file;
}

//  Retrieve dataset from dataset file
//  then insert the data to inputs and outputs array
void load_datasets(FILE* datasetFile, double** inputs, double* outputs) {
    assert(datasetFile != NULL);

    for (int i = 0; i < DATASET_COUNT; ++i) {
        for (int j = 0; j < SINGLE_INPUT_SIZE; ++j)
            fscanf(datasetFile, "%lf", (*(inputs + i) + j));
        fscanf(datasetFile, "%lf", (outputs + i));
    }
}

//  Calculate the Mean Absolute Error (MAE)
//  used to calculate training error
double mean_abs_error(double expected_output, double predicted_output) {
    return fabs(expected_output - predicted_output);
}


//  Train a single neuron and get the training error
//  after calling genann_train(), a predicted_output is calculated using genann_run()
//  For the error, choose either MAE or MSE
double train_single_neuron(genann* neural_network, double* input_pointer, double* output_pointer, double expected_output) {
    genann_train(neural_network, input_pointer, output_pointer, LEARNING_RATE);
    double predicted_output = *genann_run(neural_network, input_pointer);
    double error = mean_square_error(expected_output, predicted_output);
    return error;
}

//  Log the error during a specific training epoch
//  it will display the value in the console, as well as writing to the output file
void log_train_process(int epoch, double totalError, FILE* outputFile) {
    printf("Epoch #%d:\t%.12lf\n", epoch, totalError);
    assert(outputFile != NULL);
    fprintf(outputFile, "%.12lf\n", totalError);
}

//  Represents one training iteration
//  using all the datasets
void train_once(genann* neural_network, double** inputs, double* outputs, FILE* outputFile, int epoch) {
    double totalError = 0.0;

    for (int j = 0; j < DATASET_COUNT; ++j)
        totalError += train_single_neuron(neural_network, inputs[j], outputs + j, outputs[j]);

    //  Divide the sum of errors by the total dataset count
    //  to obtain the mean error value
    totalError /= DATASET_COUNT;

    if (epoch % LOG_ERROR_INTERVAL == 0)
        log_train_process(epoch, totalError, outputFile);
}

//  Entry point for training the network
//  This will training the network many times
void train_neural_network(genann* neural_network, double** inputs, double* outputs, FILE* outputFile) {
    //  Initialize the neural network randomly, using the current time
    srand(time(0));

    //  Print training header
    printf("\n\n=== NEURAL NETWORK TRAINING ===\n\n");

    //  Train the network repeatedly
    for (int epoch = 0; epoch < TRAINING_ITERATIONS; ++epoch)
        train_once(neural_network, inputs, outputs, outputFile, epoch);
}


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
double obtain_checksum(genann* neural_network) {
    //  Define char array and request user input
    char* test_input = malloc(MAX_TEST_INPUT_LENGTH * sizeof(char));
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


//  Release the memory allocated by the neural network and the arrays
//  Call this at the end of the program
void release_memory(genann* neural_network, double** inputs, double* outputs) {
    genann_free(neural_network);
    free(inputs);
    free(outputs);
}


//  Main program
int main(int argc, char *argv[]) {
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
    load_datasets(datasetFile, inputs, outputs);

    // Define the neural network
    genann *neural_network = genann_init(SINGLE_INPUT_SIZE, HIDDEN_LAYER_COUNT, SINGLE_HIDDEN_LAYER_NEURON_COUNT, SINGLE_OUTPUT_SIZE);

    //  Train the neural network
    train_neural_network(neural_network, inputs, outputs, outputFile);

    //  Calculate and output the checksum
    double obtained_checksum = obtain_checksum(neural_network);
    printf("\nChecksum result:\n%.0f\n", round(obtained_checksum));

    //  After operation, release the allocated memory
    release_memory(neural_network, inputs, outputs);

    return 0;
}
