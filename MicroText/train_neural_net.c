#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "neural_netX.h"
#include <string.h>

#ifndef DEBUG
#define DEBUG 1
#endif

static int verbose = 3;

static void print_all_network_info(int epoch);
//static int get_number(float *vals);
static int parse_args(int argc, char **argv);


#define NUMINPUTS      576 /* number of input units */
#define NUMHIDDEN      576 /* number of hidden units */
#define NUMOUTPUTS     90 /* number of output units */
#define NUMINTRAINSET  90 /* number of values/epochs in the training set */
#define NUMOFEVALS     90 /* number of values in the test set */

/*
static float InputVals[NUMINTRAINSET][NUMINPUTS] = {
   { 0.0, 0.0 },
   { 0.0, 1.0 },
   { 1.0, 0.0 },
   { 1.0, 1.0 }
};
static float TargetVals[NUMINTRAINSET][NUMOUTPUTS] = {
   { 0.0 },
   { 1.0 },
   { 1.0 },
   { 0.0 }
};
static float TestInputVals[NUMOFEVALS][NUMINPUTS] = {
   { 0.0, 0.0 },
   { 0.0, 1.0 },
   { 1.0, 0.0 },
   { 1.0, 1.0 }
};
*/

float LastLearningError;
float LastRMSError;
float ResultInputVals[NUMINPUTS];
float ResultIHWeights[NUMINPUTS*NUMHIDDEN];
float ResultHiddenVals[NUMHIDDEN];
float ResultHOWeights[NUMHIDDEN*NUMOUTPUTS];
float ResultOutputVals[NUMOUTPUTS];
float ResultBHWeights[1*NUMHIDDEN];
float ResultBiasVals[1];
float ResultBOWeights[1*NUMOUTPUTS];

char* concat(const char *s1, const char *s2)
{
  char *result = malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

int main(int argc, char **argv)
{


float InputVals[NUMINTRAINSET][NUMINPUTS];
float TargetVals[NUMINTRAINSET][NUMOUTPUTS];
float TestInputVals[NUMOFEVALS][NUMINPUTS];



FILE *myFile;
for (int fn = 0 ;fn < NUMINTRAINSET;fn++){

	char buf[12];
      	sprintf(buf, "%d.txt", fn); // puts string into buffer
//      	printf("%s\n", buf); // outputs so you can see it
   	
 	//char *openFile = );    
    	myFile = fopen(concat("train.bmp/",buf), "r");


	for (int k = 0; k < 576; k++)
	{
        	fscanf(myFile, "%1f", &InputVals[fn][k]);
		TestInputVals[fn][k] = InputVals[fn][k];
	}
//	for (int k = 0; k < 576; k++)
//	{
 //       	printf("Num is: %f\n\n", InputVals[fn][k]);
//	}
	TargetVals[fn][fn]=1.0;
}


   int i, j, epoch;//fd
   network_t *net;
   config_t config;

   if (parse_args(argc, argv) == -1)
      exit(EXIT_FAILURE);

  // config.Type = BACKPROP_TYPE_NORMAL;
   config.NumInputs = NUMINPUTS;
   config.NumHidden = NUMHIDDEN;
   config.NumOutputs = NUMOUTPUTS;
   config.StepSize = 0.25;
   config.Momentum = 0.90;
   config.Cost = 0.0;
   if (create_network(&net, &config) == -1) {
      perror("create_network() failed");
      exit(EXIT_FAILURE);
   }

   if (set_training_set(net, NUMINTRAINSET, (float *) InputVals, (float *) TargetVals) == -1) {
      perror("set_training_set() failed");
      exit(EXIT_FAILURE);
   }

   LastRMSError = 99;



   for (epoch = 1;  LastRMSError > 0.05  &&  epoch <= 1000000;  epoch++) {
	if(DEBUG){printf("Learning Loop: %d RMSError: %f\n", epoch,LastRMSError);}
      if (learn(net, 1) == -1) {
         perror("learn() failed");
         exit(EXIT_FAILURE);
      }
      if (verbose <= 1) {
       /*  if (query(net, NULL, &LastRMSError, NULL,
               NULL, NULL, NULL, NULL, NULL, NULL, NULL) == -1) {
            perror("bkp_query() failed");
         }*/
      } else  if (verbose == 2) {
         /* The following prints everything in the neural network for
            each step during the learning. It's just to illustrate
            that you can examine everything that's going on in order to
            debug or fine tune the network.
            Warning: Doing all this printing will slow things down
            significantly.  */
        /*  if (query(net, &LastLearningError, &LastRMSError, ResultInputVals,
               ResultIHWeights, ResultHiddenVals, ResultHOWeights, 
               ResultOutputVals,
               ResultBHWeights, ResultBiasVals, ResultBOWeights) == -1) {
            perror("bkp_query() failed");
         }*/
         print_all_network_info(epoch);
      }
        LastRMSError = net->LastRMSError;
   }


 if (verbose == 1) {
      /* The following prints everything in the neural network after
         the last learned training set. */
     /* if (query(net, &LastLearningError, &LastRMSError, ResultInputVals,
            ResultIHWeights, ResultHiddenVals, ResultHOWeights, 
            ResultOutputVals,
            ResultBHWeights, ResultBiasVals, ResultBOWeights) == -1) {
         perror("bkp_query() failed");
      }*/
      print_all_network_info(epoch);
   }

   printf("Stopped learning at epoch: %d, RMS Error: %f.\n",
      epoch-1, LastRMSError);

   printf("\n");
   printf("=======================================================\n");
   for (i = 0;  i < NUMOFEVALS;  i++) {
      printf("Evaluating inputs:");
      for (j = 0;  j < 1;  j++)
         printf(" %f", TestInputVals[i][j]);


      if (set_input(net, 0, 0.0, TestInputVals[i]) == -1) {
         perror("set_input() failed");
         exit(EXIT_FAILURE);
      }

      if (evaluate(net, ResultOutputVals, NUMOUTPUTS*sizeof(float)) == -1) {
         perror("evaluate() failed");
         exit(EXIT_FAILURE);
      }
      printf("Output values:");
	int maxVal = 0;
      	for (j = 0;  j < NUMOUTPUTS;  j++)
	{
        	if (ResultOutputVals[maxVal]<ResultOutputVals[j])
		{
			maxVal = j;
		} 
		//printf(" %f", ResultOutputVals[j]);
      	}
	printf("%c " , maxVal  + 33);
	printf("\n");
   }



   printf("=======================================================\n");
   destroy_network(net);
   return 0;
}

static void print_all_network_info(int epoch)
{
   int j;

   printf("Epoch: %d  RMS Error: %f  Learning Error: %f\nInput values: ",
         epoch, LastRMSError, LastLearningError);
   for (j = 0;  j < NUMINPUTS;  j++)
      printf("%f ", ResultInputVals[j]);
   printf("\nInput to Hidden Weights: ");
   for (j = 0;  j < NUMINPUTS*NUMHIDDEN;  j++)
      printf("%f ", ResultIHWeights[j]);
   printf("\nHidden unit values: ");
   for (j = 0;  j < NUMHIDDEN;  j++)
      printf("%f ", ResultHiddenVals[j]);
   printf("\nHidden to Output Weights: ");
   for (j = 0;  j < NUMHIDDEN*NUMOUTPUTS;  j++)
      printf("%f ", ResultHOWeights[j]);
   printf("\nLast Outputs values for the last Epoch: ");
   for (j = 0;  j < NUMOUTPUTS;  j++)
      printf("%f ", ResultOutputVals[j]);
   printf("\nBias to Hidden Weights: ");
   for (j = 0;  j < 1*NUMHIDDEN;  j++)
      printf("%f ", ResultBHWeights[j]);
   printf("\nBias unit values: ");
   for (j = 0;  j < 1;  j++)
      printf("%f ", ResultBiasVals[j]);
   printf("\nBias to Output Weights: ");
   for (j = 0;  j < 1*NUMOUTPUTS;  j++)
      printf("%f ", ResultBOWeights[j]);
   printf("\n\n");
}

static int parse_args(int argc, char **argv)
{
   int c;

   while ((c = getopt(argc, argv, "v")) != -1) {
      switch (c) {
      case 'v': verbose++; break;
      case '?': exit(EXIT_FAILURE);
      }
   }
   if (argv[optind] != NULL) {
      fprintf(stderr, "Invalid argument.\n");
      return -1;
   }
   return 0;
}

