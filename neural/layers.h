#ifndef ACTIVATION_H
#define ACTIVATION_H

#include <iostream>
#include <string>

#include "./../linalg/linalg.h"
#include "activation.h"
using std::string;

class Layer {

    protected:

        // double (*func)(double) activationFunction;

        Layer* prevLayer;
        Layer* nextLayer;

    


    public:
        vector<int> shape;

        virtual void setPrevLayer(Layer* prevLayer);
        virtual void setNextLayer(Layer* nextLayer);

        /**
         * @brief This method should be called by a Model object or 
         * by this Layer's prevLayer. 
         * 
         * input should therefore be the original input to the model 
         * or the output of this Layer's prevLayer.
         * 
         * @param input the input Tensor
         */
        virtual void feedForward(const Tensor& input);

        /**
         * @brief This method should be called by a Model object or 
         * this Layer's nextLayer). d will be the partial of error 
         * with respect to this layer's output (nextLayer's input).
         * 
         * @param d partial derivative to send backwards TO this layer 
         * for back propagation. 
         */
        virtual void backPropagate(double d);


};

class InputLayer : public Layer {
    
    public:
        InputLayer(vector<int> shape);
        void setPrevLayer(Layer* i_prevLayer) override;
        void feedForward(const Tensor& input) override;

};

class OutputLayer : public Layer {
    
    public:
        Tensor lastOutput;
        OutputLayer();
        void setPrevLayer(Layer* i_prevLayer) override;
        void setNextLayer(Layer* i_nextLayer) override;
        void feedForward(const Tensor&) override;
        Tensor getLastOutput();

};

class FlattenLayer : public Layer {

    public:
        // goal is for the flatten layer to automatically
        // read the size of the layer behind it
        FlattenLayer();
        void feedForward(const Tensor& input) override;
        void setPrevLayer(Layer* i_prevLayer) override;

};

class DenseLayer : public Layer {

    protected:
        vector<int> inputShape;
        Matrix weights;
        Matrix biases;

    public: 
        DenseLayer(int i_size);
        void setPrevLayer(Layer* i_prevLayer) override;
        void feedForward(const Matrix& input);




};


class Model {

    protected: 

        Layer* inputLayer;
        Layer* outputLayer;

    public: 

        /**
         * @brief Construct a new Model object, by 
         * inputting a vector of layers to be linked 
         * together
         * 
         * @param layers min size = 2, first must be
         * of type InputLayer, last must be of type
         * OutputLayer
         */
        Model(vector<Layer*> layers);

        

};


#endif