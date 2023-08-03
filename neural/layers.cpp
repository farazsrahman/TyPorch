#include "./../linalg/linalg.h"
#include "layers.h"
#include "activation.h"

using std::string;
using std::vector;
using std::cout;

// **LAYER**

void Layer::setPrevLayer(Layer* i_prevLayer) {
    prevLayer = i_prevLayer;
}

void Layer::setNextLayer(Layer* i_nextLayer) {
    nextLayer = i_nextLayer;
}


// **INPUT LAYER**

InputLayer::InputLayer(vector<int> i_shape) {
    shape = i_shape;
}

void InputLayer::setPrevLayer(Layer* i_prevLayer) {
    prevLayer = nullptr;
}

void InputLayer::feedForward(const Tensor& input) {

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
void OutputLayer::feedForward(const Tensor& input) {

    // saves the input tensor as to the lastOutput field
    lastOutput = input;
}
Tensor OutputLayer::getLastOutput() {
    return lastOutput;
}


// **FLATTEN LAYER** 

FlattenLayer::FlattenLayer() { /* shape set with prev layer */ }
void FlattenLayer::feedForward(const Tensor& input) {

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

    // in matrix transform rows are output cols are input
    Matrix W(shape[0], inputShape[0]);
    weights = W;

    Matrix B(shape[0], 1);
    biases = B;

    // NOTE THAT THESE WILL ALL START OFF AS ZERO
    // THERE MUST BE SOME MODEL INIT THAT RANDOMIZES
    // THE INITIAL WEIGHTS / BIASES

}

void DenseLayer::feedForward(const Matrix& input) {

    nextLayer->feedForward((weights.matMul(input)) + biases);
    
}