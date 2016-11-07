#include <math.h> //import numpy as np
#include <stdlib.h>


float sigmoid(float z) //def sigmoid(z):
{
        return 1/(1+ exp(-z));                    // return 1/(1+np.exp(-z))
}
float s_prime(float z) //def s_prime(z):
{
        return sigmoid(z)*(sigmoid(1.0-z)); //   return np.multiply(sigmoid(z), sigmoid(1.0-z))
}


float * init_weights(int layers,float epsilon)//def init_weights(layers, epsilon):
{
	float weights[layers-1];
    	for(int i =0;  i <layers -  1; i++)
	{
	       	float w = (float)rand()/(float)(RAND_MAX/2);  //w = np.random.rand(layers[i+1], layers[i]+1)
        	w = w * 2*epsilon - epsilon;
        	weights[i]=w;
    	}
	return weights;
}
float fit(float X[][], float Y[], float w[], bool predict=false, float x[] = {NULL} )//def fit(X, Y, w, predict=False, x=None):
{   
 w_grad = ([np.mat(np.zeros(np.shape(w[i]))) 
              for i in range(len(w))])
    for i in range(len(X)):
        x = x if predict else X[i]
        y = Y[0,i]
        # forward propagate
        a = x
        a_s = []
        for j in range(len(w)):
            a = np.mat(np.append(1, a)).T
            a_s.append(a)
            z = w[j] * a
            a = sigmoid(z)
        if predict: return a
        # backpropagate
        delta = a - y.T
        w_grad[-1] += delta * a_s[-1].T
        for j in reversed(range(1, len(w))):
            delta = np.multiply(w[j].T*delta, s_prime(a_s[j]))
            w_grad[j-1] += (delta[1:] * a_s[j-1].T)
    return [w_grad[i]/len(X) for i in range(len(w))]
}
float predict(float x[])   //def predict(x):
{    return fit(X, Y, w, True, x);}

int main()
{
	float X[4][2]={{0,0},{0,1},{1,0},{1,1}};
        float Y[4]      = {0,1,1,0}; //Y = np.mat([0,1,1,0])
        int layers[3] = {2,2,1};
        float epochs = 10000;
        float alpha = 0.5;
        float *w = init_weights(3, 1);


	for(int i=0; i<epochs; i++)
	{
		w_grad = fit(X, Y, w);
    		print w_grad
    		for(int  j=0 ; j< 2; j++):
        	{	
			w[j] -= alpha * w_grad[j]
		}
	}
for (int i=0; i <4; i++ )
    float x[2] = X[i];
    float guess = predict(x);
    printf("%d%s%d%s%s",x[0],"-",x[1],"  >  ", guess); //print x, ":", guess

return 0;
}
