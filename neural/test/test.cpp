#include "./../../linalg/linalg.h"
#include "./../neural.h"
#include "./../functions.cpp"
#include <vector>
#include <./gtest/gtest.h> // this shows up as an error but it shouldnt...
                           // avoid clicking here..

using std::cout;
using std::vector; 


// TEST(BasicFunctionality, testLinearFit) {

//     // THIS MODEL TRAINS ON FITTING LINEAR FUNCTIONS
//     int inputSize = 4;
//     int outputSize = 4;

//     Matrix weights(outputSize, inputSize);
//     weights.randomize(-1, 1);

//     Matrix biases(outputSize, 1);
//     biases.randomize(-1, 1);

//     vector<Matrix> train_x;
//     vector<Matrix> train_y;

//     int numExamples = 2000; 
//     for(int i = 0; i < numExamples; i++) {
//         train_x.emplace_back(inputSize, 1);
//         train_y.emplace_back(outputSize, 1);

//         train_x[i].randomize(-10, 10);
//         // train_x[i]->transpose().print();

//         train_y[i] = weights.matMul(train_x[i]) + biases;
//         // train_y[i]->print();
//     }

//     // saving a pointer to access the weights later
//     DenseLayer* denseLayer = new DenseLayer(outputSize);

//     // create Model
//     double learningRate = 0.2;

//     Model model({inputSize, 1}, 
//                 {new FlattenLayer(), denseLayer}, 
//                 new LossFunction("MSE"),
//                 new Optimizer("GD", {learningRate, true})
//                 );

//     int i = 0; 
//     double totalLoss = 0;
//     double loss = 0;
//     int batchSize = 200;
//     int iter = 20000;
//     for(int batch = 0; i < iter; batch++) {
//         totalLoss = 0;

//         // train on batch
//         cout << "\nBATCH: " << batch << "\n";
//         for(int j = 0; j < batchSize; j++) {
//             loss = model.train(train_x[i % numExamples], 
//                                train_y[i % numExamples]);            
//             totalLoss += loss;
//             i++;
//         }
//         cout << "AVG LOSS: " << totalLoss / batchSize << "\n";
//         model.update();
//     }

//     cout << "Original Weights:";
//     weights.print();
//     cout << "Original Biases:";
//     biases.print();

//     cout << "Model Weights:";
//     denseLayer->print();

// }  

TEST(BasicFunctionality, testLinearReLUFit) {

    // THIS MODEL TRAINS ON FITTING LINEAR FUNCTIONS WITH ReLU AT THE END END
    int inputSize = 30;
    int outputSize = 4;

    Matrix weights(outputSize, inputSize);
    weights.randomize(-1, 1);

    Matrix biases(outputSize, 1);
    biases.randomize(-1, 1);

    vector<Matrix> train_x;
    vector<Matrix> train_y;

    int numExamples = 2000; 
    for(int i = 0; i < numExamples; i++) {
        train_x.emplace_back(inputSize, 1);
        train_y.emplace_back(outputSize, 1);

        train_x[i].randomize(-10, 10);
        // train_x[i]->transpose().print();

        train_y[i] = (weights.matMul(train_x[i]) + biases).apply(ReLU);
        // for now just ignore the ReLU
        // train_y[i] = (weights.matMul(train_x[i]) + biases); 
        // train_y[i]->print();

    }

    // saving a pointer to access the weights later
    DenseLayer* denseLayer = new DenseLayer(outputSize);

    // create Model
    double learningRate = 0.01;

    Model model({inputSize, 1}, 
                {
                    new FlattenLayer(), 
                    denseLayer, 
                    new ActivationLayer("ReLU")
                }, 
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
            loss = model.train(train_x[i % numExamples], 
                               train_y[i % numExamples]);            
            totalLoss += loss;
            i++;
        }
        cout << "AVG LOSS: " << totalLoss / batchSize << "\n";
        model.update();
    }

    cout << "Original Weights:";
    weights.print();
    cout << "Original Biases:";
    biases.print();

    cout << "Model Weights:";
    denseLayer->print();


    cout << "\n\n";
    model.print();
}

// TEST(BasicFunctioinality, testBasicClassification) {

//     // THIS MODEL TRAINS ON FITTING A BASIC CLASSIFIER
//     int inputSize = 20;
//     int hiddenSize = 10;
//     int outputSize = 2;

//     Matrix weights1(hiddenSize, inputSize);
//     weights1.randomize(-1, 1);

//     Matrix biases1(hiddenSize, 1);
//     biases1.randomize(-1, 1);

//     Matrix weights2(outputSize, hiddenSize);
//     weights2.randomize(-1, 1);

//     Matrix biases2(outputSize, hiddenSize);
//     biases2.randomize(-1, 1);

//     vector<Matrix> train_x;
//     vector<Matrix> train_y;

//     int numExamples = 2000; 
//     for(int i = 0; i < numExamples; i++) {
//         train_x.emplace_back(inputSize, 1);
//         train_y.emplace_back(outputSize, 1);

//         train_x[i].randomize(-10, 10);
//         // train_x[i]->transpose().print();

//         train_y[i] = weights1.matMul(train_x[i]) + biases1;


//         train_y[i] = weights1.matMul(train_x[i]) + biases1;

        
//         // train_y[i]->print();
//     }

// }


