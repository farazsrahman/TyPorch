#ifndef LAYERS_H
#define LAYERS_H

#include <iostream>
#include <string>

#include "./../linalg/linalg.h"
#include "activation.h"
using std::string;

class Layer {

    protected:

        Layer* prevLayer;
        Layer* nextLayer;

    public:
        vector<int> shape;

        virtual void setPrevLayer(Layer* prevLayer);
        virtual void setNextLayer(Layer* nextLayer);


        // if a specific class of Layers handles only 
        // Matrices as input (like Dense layers) 
        // then they should be individually declared 
        // and defined in the Layer's name space
        // virtual void feedForward(const Matrix& input); 
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
        // void feedForward(const Matrix&) override;
        void feedForward(const Tensor&) override;
        Tensor getLastOutput();

};

class FlattenLayer : public Layer {

    public:
        /**
         * @brief Construct a new Flatten Layer object
         * when a model is initialized with a Flatten Layer
         * the Flatten Layer object will adopt the size of the
         * layer behind it. 
         * 
         * NOTE: Flatten will convert the input from an 
         * object of type Tensor to an object of type
         * Matrix. This allows the input to be passed
         * to a Dense Layer.
         * 
         */
        FlattenLayer();
        void feedForward(const Tensor& input) override;
        void setPrevLayer(Layer* i_prevLayer) override;

};

class DenseLayer : public Layer {

    protected:

        //TODO: ENSURE THAT THERE ARE NOT MEM MANAGEMENT OVERSIGHTS WITH POINTERS
        vector<int> inputShape;
        Matrix* weights;
        Matrix* biases;

    public: 
        DenseLayer(int i_size);
        void setPrevLayer(Layer* i_prevLayer) override;
        // void feedForward(const Matrix& input); // this method must come first, so the compiler doesn't find the Tensor input method first
        void feedForward(const Tensor& input) override; // this override catches the error of passing a Tensor into a DenseLayer




};


class Model {

    protected: 

        InputLayer* inputLayer;
        OutputLayer* outputLayer;

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
        Model(vector<int> shape, vector<Layer*> hiddenLayers);

        /**
         * @brief essentially calls feed forward on the input layer 
         * and then returns the final output layer Tensor 
         * 
         */
        Tensor feedForward(const Tensor& input);

        

};


#endif