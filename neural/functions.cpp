#ifndef FUNCTIONS_CPP
#define FUNCTIONS_CPP

#include "./../linalg/linalg.h"

#include <cmath>
#include <functional>


using std::vector;
using std::function;
using std::exp;
using std::tanh;

// LOSS FUNCTIONS

inline double squaredError(double prediction, double target) {
    return 0.5 * pow(prediction - target, 2); 
}
inline double squaredErrorPrime(double prediction, double target) {
    return prediction - target;
}
inline double crossEntropy(double prediction, double target) {
    return -target * (log(prediction) / log(2));
}
inline double crossEntropyPrime(double prediction, double target) {
    return -target / (prediction * log(2));
}

// ACTIVATION FUNCTIONS

inline double sigmoid(double x) { 
    return 1.0 / (1.0 + exp(-x)); 
}
inline double sigmoidPrime(double x) { 
    double sigx = sigmoid(x);
    return sigx * (1.0 - sigx);
}
inline double ReLU(double x) {
    return x > 0 ? x : 0;
}
inline double ReLUPrime(double x) {
    return x > 0 ? 1 : 0;
}
inline double tanhPrime(double x) {
    double tanhx = tanh(x);
    return 1.0 - (tanhx * tanhx);
}
inline vector<double> softmax(vector<double> input) {
    int n = input.size();

    vector<double> result = input;

    double sumExp = 0.0;
    for(int i = 0; i < n; i++) 
        sumExp += exp(input[i]);

    for(int i = 0; i < n; i++) 
        result[i] = exp(input[i]) / sumExp;
    
    return result;
}
inline vector<double> softmaxPrime(vector<double> input) {
    int n = input.size();

    vector<double> softmaxed = softmax(input);
    vector<double> jacobian_e; jacobian_e.resize(n * n);

    int index; double s_i, s_j;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {

            index = i * n + j; 
            s_i = softmaxed[i];
            s_j = softmaxed[j];

            if(i == j) jacobian_e[index] =  s_i * (1 - s_i);
            else       jacobian_e[index] = -s_i * s_j;
            
        }
    }

    Matrix inputVec(n, 1); inputVec.setEntries(input);
    Matrix jacobian(n, n); jacobian.setEntries(jacobian_e);

    return (jacobian.matMul(inputVec)).getEntries();
}

#endif

