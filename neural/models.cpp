#include "./../linalg/linalg.h"
#include "layers.h"
#include "activation.h"

using std::string;
using std::vector;
using std::cout;


Model::Model(vector<Layer*> layers) {

    if(layers.size() < 2) {
        cout << "ERROR: input layer list of size less than 2";
        exit(EXIT_FAILURE);
    }

    inputLayer = layers[0];
    outputLayer = layers[layers.size()-1];

    inputLayer->setNextLayer(layers[1]);
    outputLayer->setPrevLayer(layers[layers.size()-2]);

    for(int i = 0; i < layers.size(); i++) {
    
        layers[i]->setNextLayer(layers[i+1]);
        layers[i]->setPrevLayer(layers[i-1]);
    
    }
}