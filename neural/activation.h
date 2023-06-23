#ifndef ACTIVATION_H
#define ACTIVATION_H

#include <iostream>
#include <string>

#include "matrix.h"

using namespace std;

// activation functions and their derivatives

double sigmoid(double v);
double sigmoidPrime(double v);

double reLu(double v);
double reLuPrime(double v);

double tanh(double v);
double tanhPrime(double v);

// other important functions

Matrix softMax(Matrix A);



#endif