import random

class SingleLayerPerceptron:
    def __init__(self, input_size):
        self.weights = [random.uniform(-1, 1) for _ in range(input_size + 1)]

    def activation_function(self, x):
        return 1 if x >= 0 else 0

    def predict(self, inputs):
        inputs_with_bias = inputs + [1]  # 편향을 위한 1 추가
        weighted_sum = sum(w * i for w, i in zip(self.weights, inputs_with_bias))
        return self.activation_function(weighted_sum)

    def train(self, training_data, labels, epochs, learning_rate):
        for _ in range(epochs):
            for inputs, label in zip(training_data, labels):
                prediction = self.predict(inputs)
                error = label - prediction
                for i in range(len(self.weights) - 1):  # 편향 제외
                    self.weights[i] += learning_rate * error * inputs[i]
                self.weights[-1] += learning_rate * error  # 편향 업데이트

class MultiLayerPerceptron:
    def __init__(self, layers):
        self.layers = []
        for i in range(len(layers) - 1):
            self.layers.append([SingleLayerPerceptron(layers[i]) for _ in range(layers[i + 1])])

    def predict(self, inputs):
        for layer in self.layers:
            new_inputs = []
            for perceptron in layer:
                new_inputs.append(perceptron.predict(inputs))
            inputs = new_inputs
        return inputs

    def train(self, training_data, labels, epochs, learning_rate):
        for _ in range(epochs):
            for inputs, label in zip(training_data, labels):
                # 순전파
                outputs = self.predict(inputs)
                # 역전파
                for i in reversed(range(len(self.layers))):
                    layer = self.layers[i]
                    if i == len(self.layers) - 1:  # 마지막 층
                        for j, perceptron in enumerate(layer):
                            error = label[j] - outputs[j]
                            for k in range(len(perceptron.weights) - 1):  # 편향 제외
                                perceptron.weights[k] += learning_rate * error * inputs[k]
                            perceptron.weights[-1] += learning_rate * error  # 편향 업데이트
                    else:  # 중간 층
                        new_inputs = [perceptron.predict(inputs) for perceptron in self.layers[i + 1]]
                        for j, perceptron in enumerate(layer):
                            error = sum((self.layers[i + 1][k].weights[j] * (label[k] - new_inputs[k]) for k in range(len(new_inputs))))
                            for k in range(len(perceptron.weights) - 1):  # 편향 제외
                                perceptron.weights[k] += learning_rate * error * inputs[k]
                            perceptron.weights[-1] += learning_rate * error  # 편향 업데이트

# 사용 예시
if __name__ == "__main__":
    # XOR 게이트 학습 데이터
    training_data = [
        [0, 0],
        [0, 1],
        [1, 0],
        [1, 1]
    ]
    labels = [
        [0],  # 0 XOR 0 = 0
        [1],  # 0 XOR 1 = 1
        [1],  # 1 XOR 0 = 1
        [0]   # 1 XOR 1 = 0
    ]

    # 다층 퍼셉트론 초기화 (2 입력, 2 은닉층, 1 출력)
    mlp = MultiLayerPerceptron(layers=[2, 2, 1])
    
    for i in range(100):
        mlp.train(training_data, labels, epochs=1000, learning_rate=0.01)

        # 예측 테스트
        for inputs in training_data:
            print(f"Input: {inputs}, Prediction: {mlp.predict(inputs)}")
            
        print()
