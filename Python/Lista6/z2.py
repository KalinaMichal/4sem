import numpy as np
import matplotlib.pyplot as plt
import time

"""  SIGMOID  """
def sigmoid(a):
    return 1.0 / (1 + np.exp(-a))

def sigmoid_der(a):
    return a * (1.0 - a)


"""  RELU  """
def relu(a):
    return np.maximum(0, a)

def relu_der(a):
    return 1. * (a > 0)


"""  TANH  """
def tanh(a):
    return (np.exp(a) - np.exp(-a)) / (np.exp(a) + np.exp(-a))

def tanh_der(a):
    return 1.0 - tanh(a) ** 2


def mse(x, y):  #Mean squared error - Blad Srednio-kwadratowy
    rtr = 0
    for i in zip(x, y):
        rtr += (i[0] - i[1]) ** 2
    return rtr / len(x)

time = 1 #czas wyswietlania

class NeuralNetwork:
    def __init__(self, x, y, eta):
        self.input = x
        self.weights1 = np.random.rand(10, self.input.shape[1])
        self.weights2 = np.random.rand(1, 10)
        self.y = y
        self.output = np.zeros(self.y.shape)
        self.eta = eta


    def forward(self):
        self.layer1 = tanh(np.dot(self.input, self.weights1.T))
        self.output = tanh(np.dot(self.layer1, self.weights2.T))

    def back(self):
        delta2 = (self.y - self.output) * tanh_der(self.output)
        d_weights2 = self.eta * np.dot(delta2.T, self.layer1)
        delta1 = tanh_der(self.layer1) * np.dot(delta2, self.weights2)
        d_weights1 = self.eta * np.dot(delta1.T, self.input)
        self.weights1 += d_weights1
        self.weights2 += d_weights2

#funkcje wybralem tanh
wybor = input("Parabola wpisz: 1\nsinus wpisz: 2\n wybor=> ")
if(wybor=='1'):    # parabola
    n = 101
    r = 50
    X = np.linspace(start=-r, stop=r, num=n)
    X = X / r  #przeskalowanie
    y = X ** 2
    plt.scatter(X * r, y * r ** 2)
    plt.show()
    z = np.hstack((X.reshape(n, 1), np.array([[1] for i in range(n)])))
    nn = NeuralNetwork(z, y.reshape(n, 1), 0.001)
    for step in range(8000):
        nn.forward()
        nn.back()
        if step % 200 == 0:
            plt.scatter(X * r, nn.output * r ** 2)
            print("krok: {}, blad srednio-kadratowy = {}".format(step, mse( nn.output,y)))
            plt.title("krok: {}, mse = {}".format(step, mse(nn.output,y) * r ** 2))
            plt.pause(time)
            plt.close()

else:       #sinus
    n = 161
    r = 2
    X = np.linspace(start=-r, stop=r, num=n)
    y = np.sin((3 * np.pi / 2) * X)
    plt.scatter(X, y)
    plt.show()
    X = X / r  # przeskalowanie
    z = np.hstack((X.reshape(n, 1), np.array([[1] for i in range(n)])))
    nn = NeuralNetwork(z, y.reshape(n, 1), 0.001)
    for step in range(75000):
        nn.forward()
        nn.back()
        if step % 5000 == 0:
            plt.scatter(X * r, nn.output)
            print("krok: ",step ," blad srednio-kadratowy = {}".format( mse(nn.output,y)))
            plt.title("krok: {}, mse = {}".format(step, mse(nn.output,y)))
            plt.pause(time)
            plt.close()
