/* Neural Net X 
 * AUTHORS:
 * Behan Remoshan
 * Nicolas

 Last Modified: Today

 */


#ifndef NEURAL_NET_H
#define NEURAL_NET_H




/* Neural network like class*/

typedef struct {
	
	/* Basic constants*/
	int NumInTrainSet;
	int NumInputs;
	int NumHidden;
	int NumOutputs;
	int NumBias;

	/* Arrays for storing neurons and weights  */
	float *TrainSetDesiredOutputVals;	/*   */
	float *GivenDesiredOutputVals;    /* values set by bkp_set_output() */
   	float *DesiredOutputVals;         /* pointer used by bkp_forward() and bkp_backward() */
   	float *TrainSetInputVals;         /* values set by bkp_set_training_set() */
   	float *GivenInputVals;            /* values set by bkp_set_input() */
   	float *InputVals;                 /* pointer used by bkp_forward() and bkp_backward() */
   	float *IHWeights;                 /* NumInputs x NumHidden */
   	float *PrevDeltaIH;
   	float *PrevDeltaHO;
   	float *PrevDeltaBH;
   	float *PrevDeltaBO;
   	float *HiddenVals;
   	float *HiddenBetas;
   	float *HOWeights;                 /* NumHidden x NumOutputs */
  	float *BiasVals;
   	float *BHWeights;                 /* NumBias x NumHidden */
   	float *BOWeights;                 /* NumBias x NumOutputs */
   	float *OutputVals;
   	float *OutputBetas;

   	float RMSSquareOfOutputBetas;
   	float PrevRMSError;
   	float LastRMSError;
   	float LearningError;
   	float StepSize;
   	float HStepSize;
   	float Momentum;
   	float Cost;
   	int Epoch;
   	int NumConsecConverged;

   	int InputReady;
   	int DesiredOutputReady;
   	int Learned;

} network_t ; /* _t to differentiate struct  */	

/*
 * network config for initialisation
 */
typedef struct {
	int NumInputs;
	int NumHidden;
	int NumOutputs;
	
	float StepSize;
	
	float Momentum;
	
	float Cost;
} config_t;

int create_network(network_t **n, config_t *config);
void destroy_network(network_t *n);
int set_training_set(network_t *n, int ntrainset, float *tinputvals, float *targetvals);
void clear_training_set(network_t *n);
int learn(network_t *n, int ntimes);
int evaluate(network_t *n, float *eoutputvals, int sizeofoutputvals);
int query(network_t *n,
      float *qlastlearningerror, float *qlastrmserror,
        float *qinputvals, float *qihweights, float *qhiddenvals,
        float *qhoweights, float *qoutputvals,
      float *qbhweights, float *qbiasvals, float *qboweights);
int set_input(network_t *n, int setall, float val, float *sinputvals);
int set_output(network_t *n, int setall, float val, float *soutputvals);
int loadfromfile(network_t **n, char *fname);
int savetofile(network_t *n, char *fname);


#endif
