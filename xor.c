#include <stdio.h>
#include <stdlib.h>
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

//  Calculate the Mean Square Error (MSE)
//  used to calculate training error
double mean_square_error(double expected_output, double predicted_output) {
    return pow(expected_output - predicted_output, 2);
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
    printf("#%d:\t%.12lf\n", epoch, totalError);
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

    for (int epoch = 0; epoch < TRAINING_ITERATIONS; ++epoch)
        train_once(neural_network, inputs, outputs, outputFile, epoch);
}


//  Takes an input array and obtain the checksum
double calculate_checksum(genann* neural_network, double* input) {
    //  An array of double with length 2
    //  first element: current checksum
    //  second element: current input digit
    double* checksum = malloc(sizeof(double) * 2);

    //  "4" is hard-coded
    for (int i = 0; i < 4; ++i) {
        //  Assign the current input digit to the second element
        *(checksum + 1) = *(input + i);

        //  Obtain XOR result using the neural network, then store the result to the first element
        *(checksum + 0) = *genann_run(neural_network, checksum);

        //  Log the XOR process
        printf("%f ^ %f\n", round(*(checksum + 0)), *(checksum + 1));
    }

    //  Return the first element, which is the final checksum value
    return round(*(checksum + 0));
}


//  Release the memory allocated by the neural network and the arrays
//  Call this at the end of the program
void release_memory(genann* neural_network, double** inputs, double* outputs) {
    genann_free(neural_network);
    free(inputs);
    free(outputs);
}


//  Main program
int main(int argc, char *argv[])
{
    printf("Train a small neural_network to the XOR function using backpropagation.\n");


    //  Input 2D array
    double** inputs = (double**)malloc(sizeof(double*) * DATASET_COUNT);
    for (int i = 0; i < DATASET_COUNT; ++i)
        *(inputs + i) = (double*)malloc(sizeof(double) * SINGLE_INPUT_SIZE);

    //  Output array
    double* outputs = (double*)malloc(sizeof(double) * SINGLE_OUTPUT_SIZE);


    datasetFile = initialize_file(datasetFile, "dataset.txt", "r");
    outputFile = initialize_file(outputFile, "output.txt", "w+");


    load_datasets(datasetFile, inputs, outputs);


    // Define the neural network
    genann *neural_network = genann_init(SINGLE_INPUT_SIZE, HIDDEN_LAYER_COUNT, SINGLE_HIDDEN_LAYER_NEURON_COUNT, SINGLE_OUTPUT_SIZE);


    train_neural_network(neural_network, inputs, outputs, outputFile);


    //  Hard-code an input array for calculating the checksum
    printf("\n");
    double* check_input = malloc(sizeof(double) * 4);
    *(check_input + 0) = 1;
    *(check_input + 1) = 1;
    *(check_input + 2) = 0;
    *(check_input + 3) = 0;

    //  Print the hard-coded input
    for (int i = 0; i < 4; ++i) {
        printf("%f", *(check_input + i));
    }
    printf("\n");

    //  Calculate and output the checksum
    printf("\nChecksum:\n%f\n", calculate_checksum(neural_network, check_input));

    release_memory(neural_network, inputs, outputs);

    return 0;
}
