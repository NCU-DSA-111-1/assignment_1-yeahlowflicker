#ifndef NN_TRAINING
#define NN_TRAINING
#endif

#include <stdio.h>
#include <math.h>
#include "genann.h"


//  Calculate the Mean Absolute Error (MAE)
//  used to calculate training error
double mean_abs_error(double expected_output, double predicted_output);


//  Train a single neuron and get the training error
//  after calling genann_train(), a predicted_output is calculated using genann_run()
//  For the error, choose either MAE or MSE
double train_single_neuron(genann* neural_network, double* input_pointer, double* output_pointer, double expected_output, double learning_rate);


//  Log the error during a specific training epoch
//  it will display the value in the console, as well as writing to the output file
void log_train_process(int epoch, double totalError, FILE* outputFile);


//  Represents one training iteration
//  using all the datasets
void train_once(genann* neural_network, double** inputs, double* outputs, FILE* outputFile, int epoch, int training_iterations, double learning_rate, int dataset_count, int log_error_interval);


//  Entry point for training the network
//  This will training the network many times
void train_neural_network(genann* neural_network, double** inputs, double* outputs, FILE* outputFile, int training_iterations, double learning_rate, int dataset_count, int log_error_interval);
