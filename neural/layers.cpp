#include "./../linalg/linalg.h"
#include "neural.h"
#include <cmath>

using std::string;
using std::vector;
using std::cout;
using std::exp;
using std::tanh;

// **LAYER**

void Layer::setPrevLayer(Layer* i_prevLayer) {
    prevLayer = i_prevLayer;
    shape = prevLayer->shape;
}
void Layer::setNextLayer(Layer* i_nextLayer) {
    nextLayer = i_nextLayer;
}
// void Layer::feedForward(const Matrix& input) {
//     cout << "WARNING: feedForward called on Blank Layer\n";
//     nextLayer->feedForward(input);
// }
void Layer::feedForward(const Tensor& input) {
    cout << "WARNING: feedForward called on Blank Layer\n";
    nextLayer->feedForward(input);
}
void Layer::backPropagate(const Tensor& J_output) {
    cout << "WARNING: backPropagate called on Blank Layer\n";
    nextLayer->backPropagate(J_output);
}


// **INPUT LAYER**

InputLayer::InputLayer(vector<int> i_shape) {
    shape = i_shape;
}
void InputLayer::setPrevLayer(Layer* i_prevLayer) {
    prevLayer = nullptr;
}
void InputLayer::feedForward(const Tensor& input) {
    cout << "NOTE: feedForward called on Input Layer\n";

    // this layer is really just a shape check
    if(!input.hasShape(shape)) {
        cout << "ERROR: Input Layer given input of wrong shape\n";
        exit(EXIT_FAILURE);
    }

    nextLayer->feedForward(input);
}
void InputLayer::backPropagate(const Tensor& J_output) {
    cout << "NOTE: backPropagate called on Input Layer\n";
    // nothing else to do here...
}

// **OUTPUT LAYER**

OutputLayer::OutputLayer() { /* shape set with prev layer */}
void OutputLayer::setPrevLayer(Layer* i_prevLayer) {
    prevLayer = i_prevLayer;
    shape = i_prevLayer->shape; // same shape
}
void OutputLayer::setNextLayer(Layer* i_nextLayer) {
    nextLayer = nullptr;
}
void OutputLayer::feedForward(const Tensor& input) {
    cout << "NOTE: feedForward called on Output Layer\n";

    // saves the input tensor as to the lastOutput field
    lastOutput = input;
}
void OutputLayer::backPropagate(const Tensor& J_output) {
    cout << "NOTE: backPropagate called on Output Layer\n";

    // similar to how inputs feedForward just feed the input forward lol
    prevLayer->backPropagate(J_output);

}
Tensor OutputLayer::getLastOutput() {
    return lastOutput;
}


// **FLATTEN LAYER** 

FlattenLayer::FlattenLayer() { /* shape set with prev layer */ }
void FlattenLayer::setPrevLayer(Layer* i_prevLayer) { 

    prevLayer = i_prevLayer;

    int size = 1;
    for(int i = 0; i < i_prevLayer->shape.size(); i++ ) {
        size *= i_prevLayer->shape[i];
    }

    // 1d tensor
    shape = { size };

}
void FlattenLayer::feedForward(const Tensor& input) {
    cout << "NOTE: feedForward called on Flatten Layer\n";
    
    nextLayer->feedForward(flatten(input));
}
void FlattenLayer::backPropagate(const Tensor& J_output) {
    cout << "NOTE: backPropagate called on Flatten Layer\n";

    // we have to unflatten(reshape) the J_output into the 
    // shape that it was in the prev layer.

    // LOL COMPLETELY RAW DOGGIN RN COME BACK WHEN LESS TIRED
    prevLayer->backPropagate(reshape(prevLayer->shape, J_output));

}

// **DENSE LAYER**

DenseLayer::DenseLayer(int i_size) {
    shape = { i_size };
}
void DenseLayer::setPrevLayer(Layer* i_prevLayer) {

    if(i_prevLayer->shape.size() != 1) {
        cout << "ERROR: non-column vector Tensor passed into Dense Layer\n" 
             << "Flatten Layer first.";
        exit(EXIT_FAILURE);
    }
    
    inputShape = i_prevLayer->shape;
    cout << "NOTE: connecting dense layer to flattened input of size " << inputShape[0] << "\n";

    weights = new Matrix(shape[0], inputShape[0]); 
    weights->simpleXavierInit(); // use simple xavier dist for init TODO: COME BACK TO EXPLORE MORE

    cout<< "NOTE: initialized dense layer with the following matrix:\n";
    weights->print();

    biases = new Matrix(shape[0], 1);
    biases->fill(0); // IGNORE BIASES FOR NOW

    // here we want to initialize the matrices according to
    // a xavier distribution to void vanishing/exploding gradient  

}
void DenseLayer::feedForward(const Tensor& input) {

    cout << "NOTE: feedForward called on denseLayer\n";

    if(input.getDimensionality() != 2) {
        cout << "ERROR: non 2d-Tensor passed to feedForward method of Dense Layer. Expected 2d Matrix\n";
        exit(EXIT_FAILURE);
    }

    
    nextLayer->feedForward((weights->matMul(Matrix(input)))); // TODO: REMEMBER TO ADD BACK THE BIASES

}


// **ACTIVATION FUNCTIONS**
double sigmoid(double x) { 
    return 1.0 / (1.0 + exp(-x)); 
}
double sigmoidPrime(double x) { 
    double sigx = sigmoid(x);
    return sigx * (1.0 - sigx);
}
Tensor sigmoid(const Tensor& input) {
    return input.apply(sigmoid);
}
Tensor sigmoidPrime(const Tensor& input) {
    return input.apply(sigmoidPrime);
}

double tanhPrime(double x){
    double tanhx = tanh(x);
    return 1.0 - (tanhx * tanhx);
}
Tensor tanh(const Tensor& input) {
    return input.apply(tanh);
}
Tensor tanhPrime(const Tensor& input) {
    return input.apply(tanhPrime);
}

// **ACTIVATION LAYER**

ActivationLayer::ActivationLayer(string i_name) {
    name = i_name;

    if(i_name == "sigmoid") {
        activation = &sigmoid;
        activationPrime = &sigmoidPrime;

    }
    if(i_name == "tanh") {
        
        activation = &tanh;
        activationPrime = &tanhPrime;

    }

    else { 
        cout << "ERROR: \"" << i_name << "\" Activation function not found\n";
        exit(EXIT_FAILURE);
    }

}

void ActivationLayer::feedForward(const Tensor& input) {
    cout << "NOTE: feedForward called on " << name << " Activation Layer\n";
    nextLayer->feedForward(activation(input));
}




