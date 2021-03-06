#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "neural_netX.h"
#include <string.h>
#include "get_char.h"
#ifndef DEBUG
#define DEBUG 0 
#endif








#define NUMINPUTS      576 /* number of input units */
#define NUMHIDDEN      576 /* number of hidden units */
#define NUMOUTPUTS     89 /* number of output units */
#define NUMINTRAINSET  1 /* number of values/epochs in the training set */
#define NUMOFEVALS     1 /* number of values in the test set */


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


char getChar(float InputArray[][NUMINPUTS])
{

float TargetVals[NUMINTRAINSET][NUMOUTPUTS];
memset(TargetVals, 0, sizeof TargetVals);
if(DEBUG)
{
	printf("After Array init\n");
	for (int i = 0;  i < 1;  i++) 
	{
		int k =0;
	      	printf("Evaluating inputs:");
      		for ( int j = 0;  j < NUMINPUTS;  j++)
	        {
			printf(" %.1f", InputArray[i][j]);
			if (k==24)
			{k=0; printf("\n");}
			k++;
		}
	}	
}
  network_t *net;
   config_t config;





  // config.Type = BACKPROP_TYPE_NORMAL;
   config.NumInputs = NUMINPUTS;
   config.NumHidden = NUMHIDDEN;
   config.NumOutputs = NUMOUTPUTS;
   config.StepSize = 0.25;
   config.Momentum = 0.90;
   config.Cost = 0.0; 

   if(DEBUG){printf("Net Init\n");}


   if (create_network(&net, &config) == -1) {
      perror("create_network() failed");
      exit(EXIT_FAILURE);
   }
   
	if (set_training_set(net, NUMINTRAINSET, (float *) InputArray, (float *) TargetVals) == -1) {
      		perror("set_training_set() failed");
      		exit(EXIT_FAILURE);
   	}

   	LastRMSError = 99;

	if(DEBUG){printf("Load Initiated\n");}

	if(loadfromfile(&net,"English-Basic.txt")!=-1)
	{if(DEBUG){printf("Loaded Successfully");}}
	else
	{if(DEBUG){printf("Failed to Load"); return '\0';}}
	


if(DEBUG){printf("Next: Epoch Start\n");}
/*
if (ar==1){
for (epoch = 1;  LastRMSError > 0.05  &&  epoch <= 1000000;  epoch++) {
	if(DEBUG){printf("Learning Loop: %d RMSError: %f\n", epoch,LastRMSError);}
      	if (learn(net, 1) == -1)
	{
        	perror("learn() failed");
         	exit(EXIT_FAILURE);
      	}
       	LastRMSError = net->LastRMSError;
}

   printf("Stopped learning at epoch: %d, RMS Error: %f.\n",
   epoch-1, LastRMSError);

   printf("\n");
   printf("=======================================================\n");
}
*/
 
    if (set_input(net, 0, 0.0, InputArray[0]) == -1) {
         perror("set_input() failed");
         exit(EXIT_FAILURE);
      }

      if (evaluate(net, ResultOutputVals, NUMOUTPUTS*sizeof(float)) == -1) {
         perror("evaluate() failed");
         exit(EXIT_FAILURE);
      }     
	int maxVal = 0;
      	for (int j = 0;  j < NUMOUTPUTS;  j++)
	{
        	if (ResultOutputVals[maxVal]<ResultOutputVals[j])
		{
			maxVal = j;
		} 
		if(DEBUG){
		printf(" %f\n", ResultOutputVals[j]);}
      	}
	if(DEBUG){
	printf("%c " , maxVal  + 34);
	printf("\n");}

 /*  if (savetofile(net, "English-Basic.txt")!=0)
	{printf("Failed to save data");}
   else {printf("Saved!");}
*/

   if(DEBUG){
   printf("=======================================================\n");
  }
   destroy_network(net);
   char c = (char)(maxVal +34);
   return c;
  // return 0;
}
