#include "./../../linalg/linalg.h"
#include "./../neural.h"
#include <vector>
#include <./gtest/gtest.h> // this shows up as an error but it shouldnt...
                           // avoid clicking here..

using std::cout;
using std::vector;


TEST(BasicFunctionality, testBasicModelAndFlatten) {

    cout << "\n";

    Model m1({2, 2}, {new FlattenLayer(),
                      new DenseLayer(2), 
                      new ActivationLayer("tanh")
                      });

    Matrix input(2, 2);
    input.fill(1000.0);

    cout << "\nPRINTING INPUT:\n";
    input.print();

    Tensor output = m1.feedForward(input);

    cout << "\nPRINTING OUTPUT:\n";
    Matrix(output).print();

}