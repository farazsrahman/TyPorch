#include "./../linalg/linalg.h"
#include "neural.h"
#include "functions.cpp"
#include <cmath>

using std::string;
using std::vector;
using std::cout;
using std::exp;
using std::tanh;

#define OUTPUT_NOTES 0
#define OUTPUT_WARNINGS 1

// **LAYER**

void Layer::setPrevLayer(Layer* i_prevLayer) {
    prevLayer = i_prevLayer;
    shape = prevLayer->shape;
}
void Layer::setNextLayer(Layer* i_nextLayer) {
    nextLayer = i_nextLayer;
}
void Layer::feedForward(const Tensor& input) {
    // TODO: Clarify this warning...
    cout << "WARNING: feedForward called on Blank Layer (OR FEEDFORWARD OVERRIDE NOT FOUND)\n";
    nextLayer->feedForward(input);
}
void Layer::backPropagate(const Tensor& J_output) {
    cout << "WARNING: backPropagate called on Blank Layer\n";
    nextLayer->backPropagate(J_output);
}
void Layer::updateWeights(Optimizer* optim) {
    // this general call only needs to be overriden
    // for Layers that actually have weights/biases
    if (OUTPUT_NOTES) cout << "NOTE: updateWeights called on " << name << "\n";
    prevLayer->updateWeights(optim);
}
void Layer::print() const {
    cout << name << ":\n";
        cout << "\tshape: ";
        for(int i = 0; i < shape.size()-1; i++) cout << shape[i] << " x ";
        cout << shape[shape.size()-1] << "\n";

}

// **INPUT LAYER**

InputLayer::InputLayer(vector<int> i_shape) {
    name = "Input Layer";
    shape = i_shape;
}
void InputLayer::setPrevLayer(Layer* i_prevLayer) {
    prevLayer = nullptr;
}
void InputLayer::feedForward(const Tensor& input) {
    if (OUTPUT_NOTES) cout << "NOTE: feedForward called on Input Layer\n";

    // this layer is really just a shape check
    if(!input.hasShape(shape)) {
        cout << "ERROR: Input Layer given input of wrong shape\n";
        exit(EXIT_FAILURE);
    }

    nextLayer->feedForward(input);
}
void InputLayer::backPropagate(const Tensor& J_output) {
    if (OUTPUT_NOTES) cout << "NOTE: backPropagate called on Input Layer\n";
    // nothing else to do here...
}
void InputLayer::updateWeights(Optimizer* optim) {
    if (OUTPUT_NOTES) cout << "NOTE: updateWeights called on " << name << "\n";
    // no more to be done
}

// **OUTPUT LAYER**

OutputLayer::OutputLayer() {
    name = "Output Layer";
}
void OutputLayer::setPrevLayer(Layer* i_prevLayer) {
    prevLayer = i_prevLayer;
    shape = i_prevLayer->shape; // same shape
}
void OutputLayer::setNextLayer(Layer* i_nextLayer) {
    nextLayer = nullptr;
}
void OutputLayer::feedForward(const Tensor& input) {
    if (OUTPUT_NOTES) cout << "NOTE: feedForward called on Output Layer\n";

    // saves the input tensor as to the lastOutput field
    lastOutput = input;
}
void OutputLayer::backPropagate(const Tensor& J_output) {
    if (OUTPUT_NOTES) cout << "NOTE: backPropagate called on Output Layer\n";

    // similar to how inputs feedForward just feed the input forward lol
    prevLayer->backPropagate(J_output);

}
Tensor OutputLayer::getLastOutput() {
    return lastOutput;
}


// **FLATTEN LAYER** 

FlattenLayer::FlattenLayer() { 
    name = "Flat Layer";
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
void FlattenLayer::feedForward(const Tensor& input) {
    if (OUTPUT_NOTES) cout << "NOTE: feedForward called on Flatten Layer\n";
    
    nextLayer->feedForward(flatten(input));
}
void FlattenLayer::backPropagate(const Tensor& J_output) {
    if (OUTPUT_NOTES) cout << "NOTE: backPropagate called on Flatten Layer\n";

    // we have to unflatten(reshape) the J_output into the 
    // shape that it was in the prev layer.

    // LOL COMPLETELY RAW DOGGIN RN COME BACK WHEN LESS TIRED
    prevLayer->backPropagate(reshape(prevLayer->shape, J_output));

}

// **DENSE LAYER**

DenseLayer::DenseLayer(int i_size) {
    name = "Dense Layer";
    shape = { i_size };
}
void DenseLayer::setPrevLayer(Layer* i_prevLayer) {
    prevLayer = i_prevLayer;

    vector<int> inputShape = i_prevLayer->shape;
    if(inputShape.size() != 1) {
        cout << "ERROR: non-column vector Tensor passed into Dense Layer\n" 
             << "Flatten Layer first.";
        exit(EXIT_FAILURE);
    }
    
    if (OUTPUT_NOTES) cout << "NOTE: connecting dense layer to flattened input of size " << inputShape[0] << "\n";

    // values
    weights = new Matrix(shape[0], inputShape[0]); 
    biases = new Matrix(shape[0], 1);

    // gradients
    sumJ_weights = new Matrix(shape[0], inputShape[0]);
    sumJ_biases = new Matrix(shape[0], 1);

    // caches
    C_weights = new Matrix(shape[0], inputShape[0]);
    C_biases = new Matrix(shape[0], 1);

    // queues 
        // no extra init needed here 
    
    // initialization
    weights->simpleXavierInit(); //TODO: COME BACK TO EXPLORE MORE
    biases->simpleXavierInit();
    for(Matrix* m : {sumJ_weights, sumJ_biases, C_weights, C_biases}) {
        m->fill(0);
    }

    // cout<< "NOTE: initialized dense layer with the following weight matrix:\n";
    // weights->print();

    // cout<< "NOTE: initialized dense layer with the following bias matrix:\n";
    // biases->print();

}
void DenseLayer::feedForward(const Tensor& input) {
    if (OUTPUT_NOTES) cout << "NOTE: feedForward called on Dense Layer\n";
    lastInput = input; // saving for backProp

    if(input.getDimensionality() != 2) {
        cout << "ERROR: non 2d-Tensor passed to feedForward method of Dense Layer. Expected 2d Matrix\n";
        exit(EXIT_FAILURE);
    }

    nextLayer->feedForward((weights->matMul(Matrix(input))) + *biases); 

}
void DenseLayer::backPropagate(const Tensor& J_output) {
    if (OUTPUT_NOTES) cout << "NOTE: backPropagate called on Dense Layer\n";
    
    if(J_output.getDimensionality() != 2) {
        cout << "ERROR: non 2d-Tensor passed to backPropagate method of Dense Layer. Expected 2d Matrix\n";
        exit(EXIT_FAILURE);
    }

    // cast to Matrix (this may be time inefficient but for now it is safe)
    Matrix J_outputM = Matrix(J_output);
    Matrix lastInputM = Matrix(lastInput);

    // calc J_weights for lastInput
    Matrix J_weights = J_outputM.matMul(lastInputM.transpose());

    // adding to the cumalative gradient
    (*sumJ_weights) += J_weights;
    (*sumJ_biases) += J_outputM; // J_bias = J_output
    iterSincelastUpdate += 1;

    Matrix J_input = (weights->transpose()).matMul(J_outputM);
    prevLayer->backPropagate(J_input);
}
void DenseLayer::updateWeights(Optimizer* optim) {
    if (OUTPUT_NOTES) cout << "NOTE: updateWeights called on Dense Layer\n";

    // update state according to given Optimization alg
    optim->updateAlg(weights, sumJ_weights, iterSincelastUpdate, C_weights, Q_weights);
    optim->updateAlg(biases, sumJ_biases, iterSincelastUpdate, C_biases, Q_biases);

    // reset the cumulative gradient
    sumJ_weights->fill(0);
    sumJ_biases->fill(0);
    iterSincelastUpdate = 0;

    prevLayer->updateWeights(optim);

}
void DenseLayer::print() const {
    cout << name << ": \n";
    cout << "\tweights:\n";
    weights->print();
    cout << "\tbiases:\n";
    biases->print();
}


// **ACTIVATION LAYER**

ActivationLayer::ActivationLayer(string i_name) {
    name = "Activation Layer";
    activationName = i_name; // i_name is supposed to be name of the function
    if (i_name == "sigmoid") {
        
        activation = [](const Tensor& input) -> Tensor {
            return input.apply(sigmoid);
        };
        activationPrime = [](const Tensor& input) -> Tensor {
            return input.apply(sigmoidPrime);
        };

    }
    else if (i_name == "tanh") {
        
        activation = [](const Tensor& input) -> Tensor {
            return input.apply(tanh);
        };
        activationPrime = [](const Tensor& input) -> Tensor {
            return input.apply(tanhPrime);
        };

    }
    else if (i_name == "softmax") {

        activation = [](const Tensor& input) -> Tensor {
            Tensor result(input.getShape());
            
            result.setEntries(softmax(input.getEntries()));
            return result;
        };
        activationPrime = [](const Tensor& input) -> Tensor {
            Tensor result(input.getShape());
            
            result.setEntries(softmaxPrime(input.getEntries())); 
            return result;    
        };

    }
    else if (i_name == "ReLU") {
    
        activation = [](const Tensor& input) -> Tensor {
            return input.apply(ReLU);
        };
        activationPrime = [](const Tensor& input) -> Tensor {
            return input.apply(ReLUPrime);
        };

    }






    else { 
        cout << "ERROR: \"" << i_name << "\" Activation function not found\n";
        exit(EXIT_FAILURE);
    }

}

void ActivationLayer::feedForward(const Tensor& input) {
    if (OUTPUT_NOTES) cout << "NOTE: feedForward called on " << activationName << " Activation Layer\n";
    lastInput = input; // saving for backProp calc
    nextLayer->feedForward(activation(input));
}
void ActivationLayer::backPropagate(const Tensor& J_output) {
    if (OUTPUT_NOTES) cout << "NOTE: backPropagate called on " << activationName << " Activation Layer\n";
    // simply multiplying by act derivative for chain rule
    prevLayer->backPropagate(J_output * (activationPrime(lastInput)));

}



