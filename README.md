# XOR Checksum Calculator with Neural Network

This is a simple checksum calculator using a simple neural network.

This project belongs to CHENG Yu Hin, and is used as the assignment for the Data Structure class of Fall 2022, NCU.

| Module | Description |
| ---- | ---- |
| checksum.c | Calculate the checksum using the trained neural network |
| fileops.c | File operations, for training the neural network |
| main.c | Main program |
| training.c | Training the neural network using feed-forward and backpropagation |

## Compile & Run

The program can be compiled using the following command:

`gcc main.c -o main.out -lm fileops.c -lm training.c -lm checksum.c -lm genann.c -lm`

After successfully compiling, a file named `main.out` will be generated in the same directory.

Use the following command to run the compiled application:

`./main.out`
