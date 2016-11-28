/* Neural Net X 
 * AUTHORS:
 * Behan Remoshan
 * Nicolas

 Last Modified: Today

 */

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

