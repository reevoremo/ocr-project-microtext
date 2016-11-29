
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
   setup_all(*n);
   return 0;

memerrorout:
   destroy_network(*n);
   errno = ENOMEM;
   return -1;

}

void destroy_network(network_t *n)
{
   if (n == NULL)
      return;

   if (n->GivenInputVals == NULL) return;
   clear_training_set(n);//remember to create this function 
   free(n->GivenInputVals);
   if (n->GivenDesiredOutputVals != NULL) { free(n->GivenDesiredOutputVals); n->GivenDesiredOutputVals = NULL; }
   if (n->IHWeights != NULL) { free(n->IHWeights); n->IHWeights = NULL; }
   if (n->PrevDeltaIH != NULL) { free(n->PrevDeltaIH); n->PrevDeltaIH = NULL; }
   if (n->PrevDeltaHO != NULL) { free(n->PrevDeltaHO); n->PrevDeltaHO = NULL; }
   if (n->PrevDeltaBH != NULL) { free(n->PrevDeltaBH); n->PrevDeltaBH = NULL; }
   if (n->PrevDeltaBO != NULL) { free(n->PrevDeltaBO); n->PrevDeltaBO = NULL; }
   if (n->HiddenVals != NULL) { free(n->HiddenVals); n->HiddenVals = NULL; }
   if (n->HiddenBetas != NULL) { free(n->HiddenBetas); n->HiddenBetas = NULL; }
   if (n->HOWeights != NULL) { free(n->HOWeights); n->HOWeights = NULL; }
   if (n->BiasVals != NULL) { free(n->BiasVals); n->BiasVals = NULL; }
   if (n->BHWeights != NULL) { free(n->BHWeights); n->BHWeights = NULL; }
   if (n->BOWeights != NULL) { free(n->BOWeights); n->BOWeights = NULL; }
   if (n->OutputVals != NULL) { free(n->OutputVals); n->OutputVals = NULL; }
   if (n->OutputBetas != NULL) { free(n->OutputBetas); n->OutputBetas = NULL; }
   n->GivenInputVals = NULL;
   free(n);
}






int set_training_set(network_t *n, int ntrainset, float *tinputvals, float *targetvals)
{
   if (!n) {
      errno = ENOENT;
      return -1;
   }

   clear_training_set(n);

   n->NumInTrainSet = ntrainset;
   n->TrainSetInputVals = tinputvals;
   n->TrainSetDesiredOutputVals = targetvals;

   return 0;
}


void clear_training_set(network_t *n)
{
   if (n->NumInTrainSet > 0) {
      n->TrainSetInputVals = NULL;
      n->TrainSetDesiredOutputVals = NULL;
      n->NumInTrainSet = 0;
   }
}


static void setup_all(network_t *n)
{
   int i, h, o, b;

   n->InputReady = n->DesiredOutputReady = n->Learned = 0;

   n->LearningError = 0.0;

   for (i = 0;  i < n->NumInputs;  i++)
      n->GivenInputVals[i] = 0.0;

   for(h = 0;  h < n->NumHidden;  h++) {
      n->HiddenVals[h] = 0.0;
      for (i = 0;  i < n->NumInputs;  i++) {
         n->IHWeights[i+(h*n->NumInputs)] = random();
         n->PrevDeltaIH[i+(h*n->NumInputs)] = 0.0;
      }
      for (b = 0;  b < n->NumBias;  b++) {
         n->BHWeights[b+(h*n->NumBias)] = random();
         n->PrevDeltaBH[b+(h*n->NumBias)] = 0.0;
      }
   }

   for(o = 0;  o < n->NumOutputs;  o++) {
      n->OutputVals[o] = 0.0;
      for (h = 0;  h < n->NumHidden;  h++) {
         n->HOWeights[h+(o*n->NumHidden)] = random();
         n->PrevDeltaHO[h+(o*n->NumHidden)] = 0.0;
      }
      for (b = 0;  b < n->NumBias;  b++) {
         n->BOWeights[b+(o*n->NumBias)] = random();
         n->PrevDeltaBO[b+(o*n->NumBias)] = 0.0;
      }
   }

   for (b = 0;  b < n->NumBias;  b++)
      n->BiasVals[b] = 1.0;
}





int learn(network_t *n, int ntimes)
{
   int item, run;

   if (!n) {
      errno = ENOENT;
      return -1;
   }
   if (n->NumInTrainSet == 0) {
      errno = ESRCH;
      return -1;
   }

   for (run = 0;  run < ntimes;  run++) {
      for (item = 0;  item < n->NumInTrainSet;  item++) {
         /* set up for the given set item */
         n->InputVals = &(n->TrainSetInputVals[item*n->NumInputs]);
         n->DesiredOutputVals = &(n->TrainSetDesiredOutputVals[item*n->NumOutputs]);

         /* now do the learning */
         forward(n);
         backward(n);
      }

      /* now that we have gone through the entire training set, calculate the
         RMS to see how well we have learned */

      n->Epoch++;

      /* calculate the RMS error for the epoch just completed */
      n->LastRMSError = sqrt(n->RMSSquareOfOutputBetas / (n->NumInTrainSet * n->NumOutputs));
      n->RMSSquareOfOutputBetas = 0.0;
	}
 n->Learned = 1;
   return 0;

}

static void forward(network_t *n)
{
   int i, h, o, b;
        
   n->LearningError = 0.0;

   /*
    * Apply input unit values to weights between input units and hidden units
    * Apply bias unit values to weights between bias units and hidden units
    */ 
   for (h = 0;  h < n->NumHidden;  h++) {
      n->HiddenVals[h] = 0.0;
      n->HiddenBetas[h] = 0.0; /* needed if doing a backward pass later */
      for (i = 0;  i < n->NumInputs;  i++)
         n->HiddenVals[h] = n->HiddenVals[h] + (n->InputVals[i] * n->IHWeights[i+(h*n->NumInputs)]);
      for (b = 0;  b < n->NumBias;  b++)
         n->HiddenVals[h] = n->HiddenVals[h] + (n->BiasVals[b] * n->BHWeights[b+(h*n->NumBias)]);
      n->HiddenVals[h] = sigmoid(n->HiddenVals[h]);
   }
     
   /*
    * Apply hidden unit values to weights between hidden units and outputs
    * Apply bias unit values to weights between bias units and outputs
    */ 
   for (o = 0;  o < n->NumOutputs;  o++) {
      n->OutputVals[o] = 0.0;
      for (h = 0;  h < n->NumHidden;  h++)
         n->OutputVals[o] = n->OutputVals[o] + (n->HiddenVals[h] * n->HOWeights[h+(o*n->NumHidden)]);
      for (b = 0;  b < n->NumBias;  b++)
         n->OutputVals[o] = n->OutputVals[o] + (n->BiasVals[b] * n->BOWeights[b+(o*n->NumBias)]);
      n->OutputVals[o] = sigmoid(n->OutputVals[o]);
      n->LearningError = n->LearningError + 
         ((n->OutputVals[o] - n->DesiredOutputVals[o]) * (n->OutputVals[o] - n->DesiredOutputVals[o]));
   }
   n->LearningError = n->LearningError / 2.0;
}




static void backward(network_t *n)
{
   float deltaweight;
   int i, h, o, b;

   for (o = 0;  o < n->NumOutputs;  o++) {
      /* calculate beta for output units */
      n->OutputBetas[o] = n->DesiredOutputVals[o] - n->OutputVals[o];

      /* update for RMS error */
      n->RMSSquareOfOutputBetas += (n->OutputBetas[o] * n->OutputBetas[o]);

      /* update hidden to output weights */
      for (h = 0;  h < n->NumHidden;  h++) {
         /* calculate beta for hidden units for later */
         n->HiddenBetas[h] = n->HiddenBetas[h] +
            (n->HOWeights[h+(o*n->NumHidden)] * sigmoidDerivative(n->OutputVals[o]) * n->OutputBetas[o]);

//#if CMDIFFSTEPSIZE
//         deltaweight = n->HiddenVals[h] * n->OutputBetas[o];
//#else
         deltaweight = n->HiddenVals[h] * n->OutputBetas[o] *
            sigmoidDerivative(n->OutputVals[o]);
//#endif
         n->HOWeights[h+(o*n->NumHidden)] = n->HOWeights[h+(o*n->NumHidden)] +
            (n->StepSize * deltaweight) +
            (n->Momentum * n->PrevDeltaHO[h+(o*n->NumHidden)]);
         n->PrevDeltaHO[h+(o*n->NumHidden)] = deltaweight;
      }
      /* update bias to output weights */
      for (b = 0;  b < n->NumBias;  b++) {
//#if CMDIFFSTEPSIZE
//         deltaweight = n->BiasVals[b] * n->OutputBetas[o];
//#else
         deltaweight = n->BiasVals[b] * n->OutputBetas[o] +
            sigmoidDerivative(n->OutputVals[o]);
//#endif
         n->BOWeights[b+(o*n->NumBias)] = n->BOWeights[b+(o*n->NumBias)] +
            (n->StepSize * deltaweight) +
            (n->Momentum * n->PrevDeltaBO[b+(o*n->NumBias)]);
         n->PrevDeltaBO[b+(o*n->NumBias)] = deltaweight;
      }
   }
 for (h = 0;  h < n->NumHidden;  h++) {
      /* update input to hidden weights */
      for (i = 0;  i < n->NumInputs;  i++) {
         deltaweight = n->InputVals[i] * sigmoidDerivative(n->HiddenVals[h]) *
            n->HiddenBetas[h];
         n->IHWeights[i+(h*n->NumInputs)] = n->IHWeights[i+(h*n->NumInputs)] +
//#if CMDIFFSTEPSIZE
//            (n->HStepSize * deltaweight) +
//#else
            (n->StepSize * deltaweight) +
//#endif
            (n->Momentum * n->PrevDeltaIH[i+(h*n->NumInputs)]);
         n->PrevDeltaIH[i+(h*n->NumInputs)] = deltaweight;
         if (n->Cost)
            n->IHWeights[i+(h*n->NumInputs)] = n->IHWeights[i+(h*n->NumInputs)] -
               (n->Cost * n->IHWeights[i+(h*n->NumInputs)]);
      }
      /* update bias to hidden weights */
      for (b = 0;  b < n->NumBias;  b++) {
         deltaweight = n->BiasVals[b] * n->HiddenBetas[h] *
            sigmoidDerivative(n->HiddenVals[h]);
         n->BHWeights[b+(h*n->NumBias)] = n->BHWeights[b+(h*n->NumBias)] +
#if CMDIFFSTEPSIZE
            (n->HStepSize * deltaweight) +
#else
            (n->StepSize * deltaweight) +
#endif
            (n->Momentum * n->PrevDeltaBH[b+(h*n->NumBias)]);
         n->PrevDeltaBH[b+(h*n->NumBias)] = deltaweight;
         if (n->Cost)
            n->BHWeights[b+(h*n->NumBias)] = n->BHWeights[b+(h*n->NumBias)] -
               (n->Cost * n->BHWeights[b+(h*n->NumBias)]);
      }
   }
}


