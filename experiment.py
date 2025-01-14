import numpy as np

class Perceptron:

    def __init__(self, num_inputs, activation_function):
        self.weights = np.random.rand(num_inputs)
        self.bias = np.random.rand(1)
        self.activation_function = activation_function
    
    def predict(self, inputs):
        weighted_sum = np.dot(inputs, self.weights) + self.bias
        return self.activation_function(weighted_sum)
    
    def train(self, inputs, desired_output, learning_rate):
        output = self.predict(inputs)
        error = desired_output - output
        self.weights += learning_rate * error * inputs
        self.bias += learning_rate * error
        
def step_function(x):
    if x >= 0:
        return 1
    else:
        return 0

# Initialize the perceptron with 2 inputs and the step function as the activation function
perceptron = Perceptron(2, step_function)

# Inputs for the AND gate
inputs = np.array([[1, 0], [0, 0], [0, 1], [1, 1]])

# Desired outputs for the AND gate
desired_outputs = np.array([1, 0, 1, 1])

# Training the perceptron
for i in range(1000):
    for input, desired_output in zip(inputs, desired_outputs):
        perceptron.train(input, desired_output, 0.1)

print(perceptron.predict([0, 0]))
print(perceptron.predict([1, 0]))
print(perceptron.predict([0, 1]))
print(perceptron.predict([1, 1]))