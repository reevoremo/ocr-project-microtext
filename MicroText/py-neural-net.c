// Python XOR GATE
import numpy as np

def sigmoid(z):
    return 1/(1+np.exp(-z))

def s_prime(z):
    return np.multiply(sigmoid(z), sigmoid(1.0-z))

def init_weights(layers, epsilon):
    weights = []
    for i in range(len(layers)-1):
        w = np.random.rand(layers[i+1], layers[i]+1)
        w = w * 2*epsilon - epsilon
        weights.append(np.mat(w))
    return weights

def fit(X, Y, w, predict=False, x=None):
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

def predict(x):
    return fit(X, Y, w, True, x)

####

X = np.mat([[0,0],
            [0,1],
            [1,0],
            [1,1]])
Y = np.mat([0,1,1,0])
layers = [2,2,1]
epochs = 10000
alpha = 0.5
w = init_weights(layers, 1)
:w
for i in range(epochs):
    w_grad = fit(X, Y, w)
    print w_grad
    for j in range(len(w)):
        w[j] -= alpha * w_grad[j]

for i in range(len(X)):
    x = X[i]
    guess = predict(x)
    print x, ":", guess
