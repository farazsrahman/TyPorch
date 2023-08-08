#include "./../../linalg/linalg.h"
#include "./../neural.h"
#include <vector>
#include <./gtest/gtest.h> // this shows up as an error but it shouldnt...
                           // avoid clicking here..

using std::cout;
using std::vector;


TEST(BasicFunctionality, testLinearFit) {

    Matrix weights(1, 5);
    weights.setEntries({1.0, 0.2, -0.5, 0.7, 0.05});

    vector<Matrix*> train_x;
    vector<Matrix*> train_y;

    int numExamples = 2000;
    for(int i = 0; i < numExamples; i++) {
        train_x.push_back(new Matrix(5, 1));
        train_y.push_back(new Matrix(1, 1));

        train_x[i]->randomize(1);
        // train_x[i]->transpose().print();

        *train_y[i] = weights.matMul(*train_x[i]);
        // train_y[i]->print();
    }

    DenseLayer* denseLayer = new DenseLayer(1);

    // create Model
    Model model({5, 1}, 
                {new FlattenLayer(), denseLayer}, 
                new LossFunction("MSE"),
                new Optimizer("GD", {.1, true})
                );
 
    // we will train over numExamples examples
    // in batches of size batchSize for iter iterations
    // iter/batchSize batches


    int i = 0; 
    double totalLoss = 0;
    int batchSize = 200;
    int iter = 20000;
    for(int batch = 0; i < iter; batch++) {
        totalLoss = 0;

        // train on batch
        cout << "\nBATCH: " << batch << "\n";
        for(int j = 0; j < batchSize; j++) {
            totalLoss += model.train(*(train_x[i % numExamples]), 
                                     *(train_y[i % numExamples]));

            i++;
        }
        cout << "AVG LOSS: " << totalLoss/batchSize << "\n";
        model.update();
    }

    model.print(); 
    denseLayer->print();

}  