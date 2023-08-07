#ifndef LAYERS_H
#define LAYERS_H

#include <iostream>
#include <string>

#include "./../linalg/linalg.h"
using std::string;

class Layer {
    
    protected:
        Layer* prevLayer;
        Layer* nextLayer;

    public:
        vector<int> shape;
        virtual void setPrevLayer(Layer* prevLayer);
        virtual void setNextLayer(Layer* nextLayer);

        /**
         * @brief This method should be called by a Model 
         * object or by this Layer's prevLayer. 
         * 
         * NOTE: If a specific subclass of Layer handles only 
         * Matrices objects - including column vectors - as input 
         * (i.e. Dense layers) then the specific layer 
         * should manually cast the Tensor to a Matrix and 
         * catch for errors
         * 
         * @param input the input Tensor
         */
        virtual void feedForward(const Tensor& input);

        /**
         * @brief This method should be called by a Model object or 
         * this Layer's nextLayer. 
         * 
         * @param delta should be the partial with respect to the 
         * ouput Tensor of the layer backPropagate is being called from. 
         */
        virtual void backPropagate(const Tensor& delta);
};

class InputLayer : public Layer {
    
    public:
        InputLayer(vector<int> shape);
        void setPrevLayer(Layer* i_prevLayer) override;
        void feedForward(const Tensor& input) override;
        void backPropagate(const Tensor& J_output) override;

};

class OutputLayer : public Layer {
    
    public:
        Tensor lastOutput;
        OutputLayer();
        void setPrevLayer(Layer* i_prevLayer) override;
        void setNextLayer(Layer* i_nextLayer) override;
        void feedForward(const Tensor&) override;
        void backPropagate(const Tensor&) override;
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
        void backPropagate(const Tensor& J_output) override;
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
        void feedForward(const Tensor& input) override;




};

class ActivationLayer : public Layer {

    protected: 
        string name;
        Tensor (*activation)(const Tensor& input);
        Tensor (*activationPrime)(const Tensor& input);   

    public: 
        
        /**
         * @brief Construct a new Activation Layer object
         * using presets like "tanh" etc. a later feature 
         * that might be worth adding is adjusting 
         * distribution in last dense layer
         * 
         * NOTE: Currently implemented presets:
         * "sigmoid",
         * "tanh" 
         * 
         * @param name 
         */
        ActivationLayer(string name);

        void feedForward(const Tensor& input) override;
        // void backPropagate(const Tensor& J_output) override; 



};

class LossFunction {

    protected:
        string name;

        Tensor (*loss)(const Tensor&, const Tensor&);
        Tensor (*lossPrime)(const Tensor&, const Tensor&);

    public:

        /**
         * @brief Construct a new Loss Function object.
         * Similar to Activation Layers, you can pick
         * from a set of preset loss functions
         * 
         * NOTE: Currently implemented presets:
         * Mean Squared Error - "MSE",
         * 
         * @param i_name 
         */
        LossFunction(string i_name);

};

class Model {

    protected: 

        InputLayer* inputLayer;
        OutputLayer* outputLayer;

        LossFunction* loss;

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
         * @brief Calls feed forward on the input 
         * layer and then returns the final output layer 
         * Tensor 
         * 
         * @param input represents input to the model or in
         * this case the input to the model as a whole
         */
        Tensor feedForward(const Tensor& input);

        /**
         * @brief Calculates partial of Cost with respect
         * to the output
         * 
         * @param J_output is the partial derivative of
         * the cost function J with respect to the output
         * of the layer that this is being called from.
         */
        Tensor backPropagate(const Tensor& J_output);

        

};


#endif