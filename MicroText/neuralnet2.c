#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUM_TRAINING 4
#define NUM_INPUTS 2 //Not sure about the numbers
#define NUM_HIDDEN 2 //Not sure either
#define NUM_OUTPUTS 1 //Still not sure

int main (int argc, char **argv){
    float inputs[NUM_TRAINING][NUM_INPUTS] = {{rand() % 1}, {rand() % 1}}; //2 possible inputs
    float outputs[NUM_TRAINING][NUM_OUTPUTS] = {{0},{1},{1},{0}}; //4 possible outputs

    float learningrate = 0.3; //controls changes in bias/weight (typically at 0.3)
    int iterations = 10000;  //epochs

    float Wxh[NUM_INPUTS+1][NUM_HIDDEN]; //weights between inputs x and hidden nodes, including an extra one for bias
    float Why[NUM_HIDDEN+1][NUM_OUTPUTS]; //weights between hidden nodes and output y, including an extra one for bias

    for (int i = 0; i < NUM_INPUTS + 1; i++){
        for (int h = 0; h < NUM_HIDDEN; h++){
            Wxh[i][h] = ((float)rand() / (double)RAND_MAX) * 0.2 - 0.1;
        }
    }
    for (int j = 0; j < NUM_HIDDEN + 1; j++){
        for (int o = 0; o < NUM_OUTPUTS; o++){
            Why[j][o] = ((float)rand() / (double)RAND_MAX) * 0.2 - 0.1;
        }
    }

    float x[NUM_INPUTS+1]; //input vector including one extra for bias
    float y[NUM_OUTPUTS]; //output vector
    float zhWeightedSums[NUM_HIDDEN]; //weighted sums for the hidden nodes
    float hActivationValues[NUM_HIDDEN+1]; //activation values of the hidden nodes, including one extra for the bias
    float zyWeightedSums[NUM_OUTPUTS]; //weighted sums for the output nodes
    float probabilities[NUM_OUTPUTS]; //activation values of the output nodes
    float outputErrors[NUM_OUTPUTS]; //error in the output
    float deltaWxh[NUM_INPUTS+1][NUM_HIDDEN]; //adjustments to weights between inputs x and hidden nodes
    float deltaWhy[NUM_HIDDEN+1][NUM_OUTPUTS]; //adjustments to weights between hidden nodes and output y
    float loss, sum; //for storing the loss

    return 0;
}