#include <stdio.h>

typedef struct {
	float output;
	float error;
	float *weight;
	float *delta;
} neuron_t;

typedef struct {
	int numNeurons;
	neuron_t *neuron;
} layer_t;

typedef struct {
	int numLayers;
	int numPatterns;
	float momentum;
	float learning_rate;
	float global_error;
	layer_t *layer;
	layer_t *inputLayer;
	layer_t *outputLayer;
} network_t;

