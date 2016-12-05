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

/* Fill Arrays  */
int fillArrays(float InputVals[][NUMINPUTS], float TargetVals[][NUMOUTPUTS], float TestInputVals[][NUMINPUTS])
{
	FILE *myFile;
	float f;
	for (int fn = 0 ;fn < NUMINTRAINSET;fn++)
	{
		char buf[12];
        	sprintf(buf, "%d.txt", fn);
		myFile = fopen(concat("train.bmp/",buf), "r");
	        for (int k = 0; k < 576; k++)
        	{
			fscanf(myFile, "%1f", &f);
			InputVals[fn][k] = f;
                	TestInputVals[fn][k] = f;
        	}
		
		for (int k = 0; k < 576; k++)
      		{
              		printf("%d-%d  Num is: %f\n\n",fn,k, InputVals[fn][k]);
      		}
	
        	TargetVals[fn][fn]=1.0;		
	}
	return 0;
}

int main(int argc, char **argv)
{

if (argc!=2)
{
	printf("Invalid number of arguments\n");
	return 1;
}
int ar = atoi(argv[1]);

printf("%d\n",ar);

float InputVals[NUMINTRAINSET][NUMINPUTS];
float TargetVals[NUMINTRAINSET][NUMOUTPUTS];
float TestInputVals[NUMOFEVALS][NUMINPUTS];
memset(TargetVals, 0, sizeof TargetVals);
if(DEBUG){printf("After Array init");}

	if (fillArrays(InputVals,TargetVals,TestInputVals)!=0)
	{
		printf("Failed to Fill Arrays");
		return 1;
	}

   int i, j, epoch;//fd
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
   
	if (set_training_set(net, NUMINTRAINSET, (float *) InputVals, (float *) TargetVals) == -1) {
      		perror("set_training_set() failed");
      		exit(EXIT_FAILURE);
   	}

   LastRMSError = 99;
   if(ar==0){
	if(DEBUG){printf("Load Initiated\n");}

	if(loadfromfile(&net,"English-Basic.txt")!=-1)
	{printf("Loaded Successfully\n");}
	else
	{printf("Failed to Load"); return 1;}
	
   }

if(DEBUG){printf("Next: Epoch Start\n");}
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
if (ar==1){
   if (savetofile(net, "English-Basic.txt")!=0)
	{printf("Failed to save data");}
   else {printf("Saved!");}
}


   printf("=======================================================\n");
   destroy_network(net);
   return 0;
}
