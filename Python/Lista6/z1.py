import numpy as np

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

def make_networks(y):
    x = np.array([[0, 0, 1], [0, 1, 1], [1, 0, 1], [1, 1, 1]])
    return [NeuralNetwork("sigmoid", "sigmoid", x, y, 4, ),
            NeuralNetwork("sigmoid", "relu", x, y, 4 ),
            NeuralNetwork("relu", "sigmoid", x, y, 4 ),
            NeuralNetwork( "relu", "relu", x, y, 4)
            ]

def mse(x, y):  #Mean squared error - Blad Srednio-kwadratowy
    rtr = 0
    for i in zip(x, y):
        rtr += (i[0] - i[1]) ** 2
    return rtr / len(x)

class NeuralNetwork:
    """
            ostatnie 2(ukryta_warstwa, wyjsciowa_warstwa) parametry moga byc 'sigmoid' lub 'relu'
    """
    def __init__(self, ukryta_warstwa, wyjsciowa_warstwa,  x, y, ukryte):
        self.input = x  # data
        self.ukryta_warstwa = ukryta_warstwa            #ukryta
        self.wyjsciowa_warstwa = wyjsciowa_warstwa      #wyjsciowa
        if ukryta_warstwa == "relu":
            np.random.seed(17)
        else:
            np.random.seed(3)

        self.weights1 = np.random.rand(ukryte, int(self.input.size / self.input.shape[0]))

        if wyjsciowa_warstwa == "relu":
            np.random.seed(17)
        else:
            np.random.seed(3)
        self.weights2 = np.random.rand(1, ukryte)
        self.y = y
        self.output = np.zeros(self.y.shape)
        if ukryta_warstwa == "sigmoid":
            self.eta1 = 0.05
        else:
            self.eta1 = 0.05
        self.layer1 = np.zeros(ukryte)

    def forward(self):
        if self.ukryta_warstwa == "sigmoid":
            self.layer1 = sigmoid(np.dot(self.input, self.weights1.T))
        else:
            self.layer1 = relu(np.dot(self.input, self.weights1.T))
        if self.wyjsciowa_warstwa == "sigmoid":
            self.output = sigmoid(np.dot(self.layer1, self.weights2.T))
        else:
            self.output = relu(np.dot(self.layer1, self.weights2.T))

    def back(self):
        if self.wyjsciowa_warstwa == "relu":
            delta2 = (self.y - self.output) * relu_der(self.output)
        else:
            delta2 = (self.y - self.output) * sigmoid_der(self.output)

        d_weights2 = self.eta1 * np.dot(delta2.T, self.layer1)

        if self.ukryta_warstwa == "relu":
            delta1 = relu_der(self.layer1) *  np.dot(delta2, self.weights2)
        else:
            delta1 = sigmoid_der(self.layer1) * np.dot(delta2, self.weights2)

        d_weights1 = self.eta1 * np.dot(delta1.T, self.input)

        self.weights1 += d_weights1
        self.weights2 += d_weights2

"""     #####    """
"""     MAIN     """
"""     #####    """

y_xor = np.array([[0], [1], [1], [0]])
y_and = np.array([[0], [0], [0], [1]])
y_or = np.array([[0], [1], [1], [1]])

xor_networks = make_networks(y_xor)
and_networks = make_networks(y_and)
or_networks = make_networks(y_or)

for i in range(50000):
    for nn in xor_networks + or_networks + and_networks:
        nn.forward()
        nn.back()

print("--------------------\n------  XOR   ------\n--------------------")
print("Oczekiwane:\n", y_xor,"\n-------------")
for nn in xor_networks:
    print("\nwartstwa ukryta:",nn.ukryta_warstwa ,
          "  |  wartstwa wyjsciowa: ",nn.wyjsciowa_warstwa,
          "\n",nn.output)
    print("mse:",mse(nn.output,y_xor))


print("\n\n--------------------\n------  AND   ------\n--------------------")
print("Oczekiwane:\n", y_and,"\n-------------")
for nn in and_networks:
    print("\nwartstwa ukryta:",nn.ukryta_warstwa ,
          "  |  wartstwa wyjsciowa: ",nn.wyjsciowa_warstwa,
          "\n",nn.output)
    print("mse:",mse(nn.output,y_and))


print("\n\n-------------------\n------  OR   ------\n-------------------")
print("Oczekiwane:\n", y_or,"\n-------------")
for nn in or_networks:
    print("\nwartstwa ukryta:",nn.ukryta_warstwa ,
          "  |  wartstwa wyjsciowa: ",nn.wyjsciowa_warstwa,
          "\n",nn.output)
    print("mse:",mse(nn.output,y_or))

"""
    Odpowiedz na pytanie postawione w zadaniu

    Ostatnie wartości '1' to tak zwane wartości odchylenia, które umożliwiają przesunięcie całej krzywej wynikowej przez manipulowanie wagami połączeń z tego dodatkowego neuronu (węzła) do warstwy ukrytej
"""
