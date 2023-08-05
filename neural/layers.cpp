#include "./../linalg/linalg.h"
#include "neural.h"
#include "activation.h"

using std::string;
using std::vector;
using std::cout;

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
void Layer::backPropagate(double d) {
    nextLayer->backPropagate(d);
    cout << "NOTE: Blank layer called on backProp";
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


// **OUTPUT LAYER**

OutputLayer::OutputLayer() { /* shape set with prev layer */}
void OutputLayer::setPrevLayer(Layer* i_prevLayer) {
    prevLayer = i_prevLayer;
    shape = i_prevLayer->shape; // same shape
}
void OutputLayer::setNextLayer(Layer* i_nextLayer) {
    nextLayer = nullptr;
}
// void OutputLayer::feedForward(const Matrix& input) {
//     cout << "NOTE: feedForward called on Output Layer\n";

//     // saves the input tensor as to the lastOutput field
//     lastOutput = input;
// }
void OutputLayer::feedForward(const Tensor& input) {
    cout << "NOTE: feedForward called on Output Layer\n";

    // saves the input tensor as to the lastOutput field
    lastOutput = input;
}
Tensor OutputLayer::getLastOutput() {
    return lastOutput;
}


// **FLATTEN LAYER** 

FlattenLayer::FlattenLayer() { /* shape set with prev layer */ }
void FlattenLayer::feedForward(const Tensor& input) {
    cout << "NOTE: feedForward called on Flatten Layer\n";
    
    nextLayer->feedForward(flatten(input));
}
void FlattenLayer::setPrevLayer(Layer* i_prevLayer) { 

    prevLayer = i_prevLayer;

    int size = 1;
    for(int i = 0; i < i_prevLayer->shape.size(); i++ ) {
        size *= i_prevLayer->shape[i];
    }

    // 1d tensor
    shape = { size };

}


// **DENSE LAYER**

DenseLayer::DenseLayer(int i_size) {
    shape = { i_size };
}
void DenseLayer::setPrevLayer(Layer* i_prevLayer) {

    if(i_prevLayer->shape.size() != 1) {
        cout << "ERROR: non-1D tensor input into Dense Layer please use\n" 
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
// void DenseLayer::feedForward(const Matrix& input) {

//     cout << "NOTE: feedForward called on denseLayer\n";

//     // TODO: MAKE SURE USING DEREFERENCE CORRECTLY HERE..
//     nextLayer->feedForward((weights->matMul(input))); // TODO: REMEMBER TO ADD BACK THE BIASES
    
// }
void DenseLayer::feedForward(const Tensor& input) {
    cout << "NOTE: feedForward called on denseLayer\n";

    if(input.getDimensionality() != 2) {
        cout << "ERROR: non 2d-Tensor passed to feedForward method of Dense Layer. Expected 2d Matrix\n";
        exit(EXIT_FAILURE);
    }

    
    nextLayer->feedForward((weights->matMul(Matrix(input)))); // TODO: REMEMBER TO ADD BACK THE BIASES

}