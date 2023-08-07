#include "./../linalg/linalg.h"
#include "neural.h"

using std::string;
using std::vector;
using std::cout;

// **LOSS FUNCTION**
double meanSquaredError(double prediction, double actual) {

    return (1/2) * (prediction - actual) ** 2; 

}
LossFunction::LossFunction(string i_name) {
    name = i_name;
    
    if(i_name == "MSE" || i_name = "Mean Squared Error") {


    }

}


// **MODEL**

Model::Model(vector<int> shape, vector<Layer*> hiddenLayers) {

    inputLayer = new InputLayer(shape);
    outputLayer = new OutputLayer();
    

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

Tensor Model::feedForward(const Tensor& input) {
     
     inputLayer->feedForward(input);
     return outputLayer->getLastOutput();

}