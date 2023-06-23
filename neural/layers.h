#ifndef ACTIVATION_H
#define ACTIVATION_H

#include <iostream>
#include <string>

#include "matrix.h"
using std::string;



class Layer {

    private:

        Layer* prevLayer;
        Layer* nextLayer;

        // number of dimensions (size of the shape* member)
        int numberDimensions;
        int* shape;

    public:

        // CONSTRUCTORS AND SETUP

        Layer(int numberDimensions, int* shape);

        void setPrevLayer(Layer* prevLayer);
        void setNextLayer(Layer* prevLayer);


        // EXTERNAL METHODS

        /**
         * @brief This method should be called by a Model object or 
         * by the layer behind it (this Layer's prevLayer) in a Model. 
         * input should therefore be the original input to the model 
         * or the output of this Layer's prevLayer.
         * 
         * @param input the input vector Matrix
         * @return Matrix& a reference to the final output vector Matrix.
         */
        void feedForward(const Matrix& input);
        /**
         * @brief This method should be called by a Model object or
         * by the layer in front of it (this Layer's nextLayer). d will 
         * therefore be the partial of error with respect to this 
         * layer's output (nextLayer's input).
         * 
         * @param d partial derivative to send backwards TO this layer 
         * for back propagation. 
         */
        void backpropagate(double d);


};

class DenseLayer : public Layer {

    private:


        int size;

};


#endif