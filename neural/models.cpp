#include "./../linalg/linalg.h"
#include "neural.h"
#include <cmath>

using std::string;
using std::vector;
using std::cout;

// **LOSS FUNCTION DEFINITIONS** helpers for CostFunction class

/**
 * @brief returns 1/2 (prediction - actual)^2 
 * 
 * @param prediction 
 * @param actual 
 * @return double 
 */
double squaredError(double prediction, double actual) {
    return (1/2) * pow(prediction - actual, 2); 
}
double squaredErrorPrime(double prediction, double actual) {
    return prediction - actual;
}
Tensor squaredError(const Tensor& prediction, const Tensor& actual) {
    // we will have an earlier error check to make sure prediction and actual 
    // are the same shape

    Tensor result(actual.getShape());
    vector<double> resultEntries(actual.getSize());
    vector<double> predictionEntries = prediction.getEntries();
    vector<double> actualEntries = actual.getEntries();

    for(int i = 0; i < actual.getSize(); i++) {
        resultEntries[i] = squaredError(predictionEntries[i], actualEntries[i]);
    }
    result.setEntries(resultEntries);

    return result;
}
Tensor squaredErrorPrime(const Tensor& prediction, const Tensor& actual) {
    Tensor result(actual.getShape());
    vector<double> resultEntries(actual.getSize());
    vector<double> predictionEntries = prediction.getEntries();
    vector<double> actualEntries = actual.getEntries();

    for(int i = 0; i < actual.getSize(); i++) {
        resultEntries[i] = squaredErrorPrime(predictionEntries[i], actualEntries[i]);
    }
    result.setEntries(resultEntries);

    return result;
}

// **COST FUNCTION**

CostFunction::CostFunction(string i_name) {
    name = i_name;
    
    if(i_name == "MSE" || i_name == "Mean Squared Error") {

        loss = [](const Tensor& pred,  const Tensor& actual) {
            return squaredError(pred, actual).getMean();
        };
        lossPrime = [](const Tensor& pred,  const Tensor& actual) {
            return squaredErrorPrime(pred, actual)/actual.getSize();
        };

    }

    else if(i_name == "SSE" || i_name == "Sum Squared Error") {

        loss = [](const Tensor& pred,  const Tensor& actual) {
            return squaredError(pred, actual).getSum();
        };
        lossPrime = [](const Tensor& pred,  const Tensor& actual) {
            return squaredErrorPrime(pred, actual);
        };

    }

    else {
        cout << "ERROR: \"" << i_name << "\" does not match any preset loss function\n";
        exit(EXIT_FAILURE);
    }

}
double CostFunction::getCost(const Tensor& pred, const Tensor& actual) const {
    if(pred.getShape() != actual.getShape()) {
        cout << "ERROR: prediction Tensor and actual Tensor shape mismatch\n";
        exit(EXIT_FAILURE);
    }
    
    return loss(pred, actual);
}
Tensor CostFunction::getNegativeGradient(const Tensor& pred, const Tensor& actual) const {
    return lossPrime(pred, actual);
}


// **OPTIMIZER**

Optimizer::Optimizer(string name, vector<double> parameters) {

    if(name == "GD") {
        if(parameters.size() != 2) {
            cout << "ERROR: incorrect number of prams passed to SGD optimizer\n";
            exit(EXIT_FAILURE);
        }

        // [parameters] captures a constant version of parameters for the lambda function
        updateAlg = [parameters](Tensor* weights, Tensor* accumalatedGrad, int iterSinceLastUpdate,  
                          Tensor* cache, vector<Tensor*> queue) {
                            
                            double learningRate = parameters[0];
                            bool useAvgGrad = parameters[1];

                            // we update the weights using the pointers


                            if(useAvgGrad) { // use average grad
                                (*weights) -= learningRate * ((*accumalatedGrad) / iterSinceLastUpdate);
                            } 
                            else { // use cumalative sum grad
                                (*weights) -= learningRate * (*accumalatedGrad);
                            }


                            // the layer's updateWeights() call will handle
                            // zeroing the gradient for us.
                          };

    }

    else {
        cout << "ERROR: input Optimizer name did not match any presets\n";
        exit(EXIT_FAILURE);
    }
}

// **MODEL**

Model::Model(vector<int> shape, vector<Layer*> hiddenLayers, CostFunction* i_costFunction) {

    inputLayer = new InputLayer(shape);
    outputLayer = new OutputLayer();
    costFunction = i_costFunction;
    

    // allowing no hidden layer models for the purposes of testing.
    if(hiddenLayers.size() == 0) {
        cout << "WARNING: no hidden layer inputted";
        inputLayer->setNextLayer(outputLayer);
        outputLayer->setPrevLayer(inputLayer);
    }
    
    else { // form hidden layers into a linked list
        
        // this logic can prolly be simplified later

        inputLayer->setNextLayer(hiddenLayers[0]);
        hiddenLayers[0]->setPrevLayer(inputLayer);

        for(int i = 0; i < hiddenLayers.size(); i++) {
    
        if(i < hiddenLayers.size()-1) { // if it is not the last hiddenLayer
            hiddenLayers[i]->setNextLayer(hiddenLayers[i+1]);
        }
        if(i > 0) { // if it is not the first hiddenLayer
            hiddenLayers[i]->setPrevLayer(hiddenLayers[i-1]);
        }

        outputLayer->setPrevLayer(hiddenLayers[hiddenLayers.size()-1]);
        hiddenLayers[hiddenLayers.size()-1]->setNextLayer(outputLayer);

        }

    }
    
}

Tensor Model::predict(const Tensor& input) {
     
     inputLayer->feedForward(input);
     return outputLayer->getLastOutput();

}