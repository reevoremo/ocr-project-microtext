#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#define DEBUG  1

#define NUM_TRAINING 4 //XOR GATE has four sets of inputs
#define NUM_INPUTS 2
#define NUM_HIDDEN 2
#define NUM_OUTPUTS 1


/////////////////////////////////////////////////////////
/* Sigmoid returns values from 0.0 to 1.0  */
#define sigmoid(x)		(1.0 / (1.0 + (float)exp(-(double)(x))))
#define sigmoidDerivative(x)	((float)(x)*(1.0-(x)))	
/////////////////////////////////////////////////////////
int main()
{

	float inputs[NUM_TRAINING][NUM_INPUTS] = {{0,0},{0,1},{1,0},{1,1}};
	float outputs[NUM_TRAINING][NUM_OUTPUTS] = {{0},{1},{1},{0}};

	float learningrate = 0.03;
	int iterations = 1250;
	
	//Doctors TUTO
	//float rms = 0.0;


	float Wxh[NUM_INPUTS+1][NUM_HIDDEN]; //Weight Input - Hidden
	float Why[NUM_HIDDEN+1][NUM_OUTPUTS]; //Weight Hidden - output

	int t, i, h, o; //looping variables for iterations, input nodes, hidden nodes, output nodes
	for (i=0; i<NUM_INPUTS+1; i++)
	{
		for (h=0; h<NUM_HIDDEN; h++)
		{
			Wxh[i][h] = (((float)rand() / (double)RAND_MAX) * 0.2) - 0.1;
			//printf("%f", Wxh[i][h]);
		}
	}
	for (h=0; h<NUM_HIDDEN+1; h++)
	{
		for (o=0; o<NUM_OUTPUTS; o++) 
		{
			Why[h][o] = (((float)rand() / (double)RAND_MAX) * 0.2) - 0.1;
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

	//Training Loop
 t = 0;

while ( t<iterations) 
{
	
	 //this is how many times we will train the network
	x[0]=1; 
	memcpy (&x[1], inputs[t % NUM_TRAINING], sizeof (inputs[0])); //copy the input into x with the bias=1
	// &x[1] send as ref
	memcpy (y, outputs[t % NUM_TRAINING], sizeof (outputs[0])); //copy the output 
	++t;
	x[1] = sigmoid(x[1]);
	x[2] = sigmoid(x[2]);
	//Weighted Sumsfor Hidden nodes
	memset (zhWeightedSums, 0, sizeof (zhWeightedSums)); //set all the weighted sums to zero
	for (h=0; h<NUM_HIDDEN; h++)
	{
		for (i=0; i<NUM_INPUTS+1; i++)
		{
			 zhWeightedSums[h] += x[i] * Wxh[i][h]; //multiply and sum inputs * weights
		}
	}

	//Activation Values
	hActivationValues[0]=1; //set the bias for the first hidden node to 1
	for (h=0; h<NUM_HIDDEN; h++) 
	{
					//sigmoid
		hActivationValues[h+1] =(zhWeightedSums[h]); //apply activation function on other hidden nodes
	}

	//Weighted sum for output nodes
	memset (zyWeightedSums, 0, sizeof (zyWeightedSums)); //set all the weighted sums to zero
	//multiply and sum inputs * weights
	for (o=0; o<NUM_OUTPUTS; o++) 
	{
		for (h=0; h<NUM_HIDDEN+1; h++) 
		{	
			zyWeightedSums[o] += hActivationValues[h] * Why[h][o]; 
		}
	}


	//Probable Answers
	for (sum=0, o=0; o<NUM_OUTPUTS; o++) 
	{
				//sigmoid
		probabilities[o] = (zyWeightedSums[o]);
	 	sum += probabilities[o];
	} //compute exp(z) for softmax
	for (o=0; o<NUM_OUTPUTS; o++) 
	{
	//	printf("%f",sum);
	//	probabilities[o] /= sum; //apply softmax by dividing by the the sum all the exps
	}

	
	//output Error
	for (o=0; o<NUM_OUTPUTS; o++) 
	{
		
		outputErrors[o] =y[o] -  probabilities[o]; //the error for each output
	}
	//Loss
	for (loss=0, o=0; o<NUM_OUTPUTS; o++) 
	{
		loss += ((probabilities[o] - y[o])*(y[o] * probabilities[o])); //the loss
	}
	loss /= 2.0;
	/////////////////////////////////////////////////////////////////////////////////
	if (DEBUG) 
	{
		printf("%s%d%s","\n////////////////////////////////////////////////////   ",t,"  \n\n");
		
		printf("%s", "INPUTS\t");
		for (int i =0;i<NUM_INPUTS +1;i++)
		{
			printf("%f%s" , x[i], "\t");
		
		}
		printf("%s", "\nInput-Hidden Weights  ");
		for (int i = 0; i < NUM_INPUTS + 1 ;i++)
		{
			for (h=0;h<NUM_HIDDEN; h++)
			{
				printf("%f%s", Wxh[i][h], "  ");
			}
		}
		printf("%s", "\nHidden Weighted SUM              ");
		for (h=0;h<NUM_HIDDEN ; h++)
		{
			printf("%f%s",zhWeightedSums[h], "  ");
		}
		printf("%s", "\nHidden Sigmoid VAL   ");
		for (h=0;h<NUM_HIDDEN +1;h++)
		{
			printf("%f%s", hActivationValues[h], "  ");
		}
		printf("%s", "\nHidden-Output Weight SUM");
		for (h=0;h<NUM_OUTPUTS;h++)
		{
			printf("%f%s", zyWeightedSums[h], "  ");
		}
		printf("%s", "\nOutput Weight-Sigmoid ");
                for (h=0;h<NUM_OUTPUTS;h++)
                {
                        printf("%f%s", sigmoid(zyWeightedSums[h]), "  ");
                }
		printf("%s", "\nHidden-Output Weight");
                for (o=0;o<NUM_OUTPUTS;o++)
                {
			for (h=0;h<NUM_HIDDEN +1;h++)
			{
                        printf("%f%s", Why[h][o], "  ");
               		}
		}

		printf("%s%f","\nGUESS - ", probabilities[0]);
		printf("%s","\n////////////////////////////////////////////////////\n");
	}	

/////////////////////////////////////////////////////////////////////////////////
	//Finding the gradient
	for (h=0; h<NUM_HIDDEN+1; h++) 
	{
		for (int o=0; o<NUM_OUTPUTS; o++) 
		{
			deltaWhy[h][o] = (hActivationValues[h]) * outputErrors[o];//Sec Opt Exist
		}
	}
	//Backward propagation
	memset (hActivationValues, 0, sizeof (hActivationValues)); //set all the weighted sums to zero
	for (h=1; h<NUM_HIDDEN+1; h++) 
	{
		for (o=0; o<NUM_OUTPUTS; o++) 
		{
							    //SigDev(outErr)
			hActivationValues[h] = Why[h][o] * sigmoidDerivative(outputErrors[o]) * outputErrors[o];
		}
	}
	
	//apply activation function gradient
	for (h=0; h<NUM_HIDDEN; h++)
	{
		//zhWeightedSums[h] = hActivationValues[h+1] * (sigmoidDerivative(zhWeightedSums[h]));
	}
	//Multiply x*eh*zh to get the adjustments to deltaWxh, this does not include the bias node
	for (int i=0; i<NUM_INPUTS+1; i++) 
	{
		for (int h=0; h<NUM_HIDDEN; h++) 
		{
						//sigDev
			deltaWxh[i][h] = x[i] * sigmoidDerivative((zhWeightedSums[h]))*hActivationValues[h];
		}
	}
	//Changing Weights
	for (int h=0; h<NUM_HIDDEN+1; h++) 
	{	
		for (int o=0; o<NUM_OUTPUTS; o++)
		{
			 Why[h][o] += learningrate * deltaWhy[h][o];
		}
	}
	for (int i=0; i<NUM_INPUTS+1; i++) 
	{
		for (int h=0; h<NUM_HIDDEN; h++) 
		{
			Wxh[i][h] += learningrate * deltaWxh[i][h];
		}
	}
}



}
