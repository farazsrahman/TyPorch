#include "./../../linalg/linalg.h"
#include "./../layers.h"
#include "./../activation.h"
#include <vector>
#include <./gtest/gtest.h> // this shows up as an error but it shouldnt...
                           // avoid clicking here..

using std::cout;
using std::vector;


TEST(BasicFunctionality, testBasicModelAndFlatten) {

    cout << "\n";

    Model m1({3, 3}, {new Layer(), 
                      new FlattenLayer(), 
                      new DenseLayer(5), 
                      new DenseLayer(5), 
                      new DenseLayer(2)
                    });

    Matrix input(3, 3);
    input.fill(2.0);

    cout << "\nPRINTING INPUT:\n";
    input.print();

    Tensor output = m1.feedForward(input);

    cout << "\nPRINTING OUTPUT:\n";
    Matrix(output).print();

}