#include "./../../linalg/linalg.h"
#include "./../neural.h"
#include <vector>
#include <./gtest/gtest.h> // this shows up as an error but it shouldnt...
                           // avoid clicking here..

using std::cout;
using std::vector; 


TEST(BasicFunctionality, testLinearFit) {

    // THIS MODEL TRAINS ON FITTING LINEAR FUNCTIONS
    int inputSize = 30;
    int outputSize = 30;

    Matrix weights(outputSize, inputSize);
    weights.randomize(-1, 1);

    Matrix biases(outputSize, 1);
    biases.randomize(-1, 1);

    vector<Matrix*> train_x;
    vector<Matrix*> train_y;

    int numExamples = 2000; 
    for(int i = 0; i < numExamples; i++) {
        train_x.push_back(new Matrix(inputSize, 1));
        train_y.push_back(new Matrix(outputSize, 1));

        train_x[i]->randomize(-10, 10);
        // train_x[i]->transpose().print();

        *train_y[i] = weights.matMul(*train_x[i]) + biases;
        // train_y[i]->print();
    }

    DenseLayer* denseLayer = new DenseLayer(outputSize);

    // create Model
    double learningRate = 0.2;

    Model model({inputSize, 1}, 
                {new FlattenLayer(), denseLayer}, 
                new LossFunction("MSE"),
                new Optimizer("GD", {learningRate, true})
                );

    int i = 0; 
    double totalLoss = 0;
    double loss = 0;
    int batchSize = 200;
    int iter = 20000;
    for(int batch = 0; i < iter; batch++) {
        totalLoss = 0;

        // train on batch
        cout << "\nBATCH: " << batch << "\n";
        for(int j = 0; j < batchSize; j++) {
            loss = model.train(*(train_x[i % numExamples]), 
                                     *(train_y[i % numExamples]));            
            totalLoss += loss;
            i++;
        }
        cout << "AVG LOSS: " << totalLoss/batchSize << "\n";
        model.update();
    }

    // model.print(); 

    // cout << "TARGET WEIGHT/BIASES: \n";
    // weights.print();
    // biases.print(); 
    // cout << "\n\n";
    // denseLayer->print();

}  