/*

training.c: The neural network training module

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the
use of this software.

This project is created for the 1st assignemnt of the Data Structure
class (Fall 2022) of the NCU Communication Engineering Department.

The project is built and owned by Cheng Yu Hin (Stu. no. 110503534).
It may not actively maintained and should be kept private whenever possible.

*/

#include "training.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "genann.h"


//  Calculate the Mean Absolute Error (MAE)
//  used to calculate training error
double mean_abs_error(double expected_output, double predicted_output) {
    return fabs(expected_output - predicted_output);
}


//  Train a single neuron and get the training error
//  after calling genann_train(), a predicted_output is calculated using genann_run()
//  For the error, choose either MAE or MSE
double train_single_neuron(genann* neural_network, double* input_pointer, double* output_pointer, double expected_output, double learning_rate) {
    genann_train(neural_network, input_pointer, output_pointer, learning_rate);
    double predicted_output = *genann_run(neural_network, input_pointer);
    double error = mean_abs_error(expected_output, predicted_output);
    return error;
}


//  Log the error during a specific training epoch
//  it will display the value in the console, as well as writing to the output file
void log_train_process(int epoch, double totalError, FILE* outputFile) {
    printf("Epoch #%d:\t%.20lf\n", epoch, totalError);
    assert(outputFile != NULL);
    fprintf(outputFile, "%.20lf\n", totalError);
}

//  Represents one training iteration
//  using all the datasets
void train_once(genann* neural_network, double** inputs, double* outputs, FILE* outputFile, int epoch, int training_iterations, double learning_rate, int dataset_count, int log_error_interval) {
    double totalError = 0.0;

    for (int j = 0; j < dataset_count; ++j)
        totalError += train_single_neuron(neural_network, inputs[j], outputs + j, outputs[j], learning_rate);

    //  Divide the sum of errors by the total dataset count
    //  to obtain the mean error value
    totalError /= dataset_count;

    if (epoch % log_error_interval == 0)
        log_train_process(epoch, totalError, outputFile);
}


//  Entry point for training the network
//  This will training the network many times
void train_neural_network(genann* neural_network, double** inputs, double* outputs, FILE* outputFile, int training_iterations, double learning_rate, int dataset_count, int log_error_interval) {
    //  Print training header
    printf("\n\n=== NEURAL NETWORK TRAINING ===\n\n");

    //  Train the network repeatedly
    for (int epoch = 0; epoch < training_iterations; ++epoch)
        train_once(neural_network, inputs, outputs, outputFile, epoch, training_iterations, learning_rate, dataset_count, log_error_interval);
}
