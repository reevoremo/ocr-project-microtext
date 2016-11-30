
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "neural_netX.h"

static int verbose = 3;

static void print_all_network_info(int epoch);
//static int get_number(float *vals);
static int parse_args(int argc, char **argv);


#define NUMINPUTS      2 /* number of input units */
#define NUMHIDDEN      2 /* number of hidden units */
#define NUMOUTPUTS     1 /* number of output units */
#define NUMINTRAINSET  4 /* number of values/epochs in the training set */
#define NUMOFEVALS     4 /* number of values in the test set */

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

int main(int argc, char **argv)
{
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
      perror("bkp_create_network() failed");
      exit(EXIT_FAILURE);
   }	

   if (set_training_set(net, NUMINTRAINSET, (float *) InputVals, (float *) TargetVals) == -1) {
      perror("bkp_set_training_set() failed");
      exit(EXIT_FAILURE);
   }

   LastRMSError = 99;



   for (epoch = 1;  LastRMSError > 0.0005  &&  epoch <= 1000000;  epoch++) {
      if (learn(net, 1) == -1) {
         perror("bkp_learn() failed");
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
      for (j = 0;  j < NUMINPUTS;  j++)
         printf(" %f", TestInputVals[i][j]);
      

      if (set_input(net, 0, 0.0, TestInputVals[i]) == -1) {
         perror("bkp_set_input() failed");
         exit(EXIT_FAILURE);
      }
      if (evaluate(net, ResultOutputVals, NUMOUTPUTS*sizeof(float)) == -1) {
         perror("bkp_evaluate() failed");
         exit(EXIT_FAILURE);
      }
      printf("Output values:");
      for (j = 0;  j < NUMOUTPUTS;  j++)
         printf(" %f", ResultOutputVals[j]);
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

/*
 * get_number - Given an array of three floating point values, each
 * in the range 0.0 to 1.0, this will convert any value <= 0.05 to
 * a 0 and any > 0.05 to a 1 and treat the resulting three 0s and 1s
 * as three binary digits. It will then return the digital equivalent.
 *
 * Example:
 * Given an array containing 0.00000, 0.94734, 0.02543, these will be 
 * converted to 0, 1, 0 or 010 which is a binary number that is 2 in 
 * digital. This will therefore return a 2.
 */
/*static int get_number(float *vals)
{
   int num = 0, i;

   for (i = 0; i < NUMOUTPUTS; i++)
      if (vals[i] > 0.05)
         num += 1 << ((NUMOUTPUTS-i)-1);
   return num;
}
*/
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
