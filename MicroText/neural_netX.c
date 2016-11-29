
/**/

#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include "neural_netX.h"

/* The following sigmoid returns values from 0.0 to 1.0 */
#define sigmoid(x)           (1.0 / (1.0 + (float)exp(-(double)(x))))
#define sigmoidDerivative(x) ((float)(x)*(1.0-(x))) 
/* random() for -1 to +1 */
#define random()             ((((float)rand()/(RAND_MAX)) * 2.0) - 1.0)

int create_network(network_t **n, config_t *config)
{
	if ((*n = (bkp_network_t *) malloc(sizeof(bkp_network_t))) == NULL) 
	{
      		errno = ENOMEM;
      		return -1;
   	}

	(*n)->NumInputs = config->NumInputs;
   	(*n)->NumHidden = config->NumHidden;
   	(*n)->NumOutputs = config->NumOutputs;
   	(*n)->NumConsecConverged = 0;
   	(*n)->Epoch = (*n)->LastRMSError = (*n)->RMSSquareOfOutputBetas = 0.0;
   	(*n)->NumBias = 1;
   	if (config->StepSize == 0){(*n)->StepSize = 0.5;}
   	else {(*n)->StepSize = config->StepSize;}
	
	if (config->Momentum == -1){(*n)->Momentum = 0.5;}
	else{(*n)->Momentum = config->Momentum;}
	
	(*n)->Cost = config->Cost;

	/* Memory allocation for the arrays  */

   if (((*n)->GivenInputVals = (float *) malloc((*n)->NumInputs * sizeof(float))) == NULL)
      goto memerrorout;
   if (((*n)->GivenDesiredOutputVals = (float *) malloc((*n)->NumOutputs * sizeof(float))) == NULL)
      goto memerrorout;
   if (((*n)->IHWeights = (float *) malloc((*n)->NumInputs * (*n)->NumHidden * sizeof(float))) == NULL)
      goto memerrorout;
   if (((*n)->PrevDeltaIH = (float *) malloc((*n)->NumInputs * (*n)->NumHidden * sizeof(float))) == NULL)
      goto memerrorout;
   if (((*n)->PrevDeltaHO = (float *) malloc((*n)->NumHidden * (*n)->NumOutputs * sizeof(float))) == NULL)
      goto memerrorout;
   if (((*n)->PrevDeltaBH = (float *) malloc((*n)->NumBias * (*n)->NumHidden * sizeof(float))) == NULL)
      goto memerrorout;
   if (((*n)->PrevDeltaBO = (float *) malloc((*n)->NumBias * (*n)->NumOutputs * sizeof(float))) == NULL)
      goto memerrorout;
   if (((*n)->HiddenVals = (float *) malloc((*n)->NumHidden * sizeof(float))) == NULL)
      goto memerrorout;
   if (((*n)->HiddenBetas = (float *) malloc((*n)->NumHidden * sizeof(float))) == NULL)
      goto memerrorout;
   if (((*n)->HOWeights = (float *) malloc((*n)->NumHidden * (*n)->NumOutputs * sizeof(float))) == NULL)
      goto memerrorout;
   if (((*n)->BiasVals = (float *) malloc((*n)->NumBias * sizeof(float))) == NULL)
      goto memerrorout;
   if (((*n)->BHWeights = (float *) malloc((*n)->NumBias * (*n)->NumHidden * sizeof(float))) == NULL)
      goto memerrorout;
   if (((*n)->BOWeights = (float *) malloc((*n)->NumBias * (*n)->NumOutputs * sizeof(float))) == NULL)
      goto memerrorout;
   if (((*n)->OutputVals = (float *) malloc((*n)->NumOutputs * sizeof(float))) == NULL)
      goto memerrorout;
   if (((*n)->OutputBetas = (float *) malloc((*n)->NumOutputs * sizeof(float))) == NULL)
      goto memerrorout;
   bkp_setup_all(*n);
   return 0;

memerrorout:
   bkp_destroy_network(*n);
   errno = ENOMEM;
   return -1;

}
