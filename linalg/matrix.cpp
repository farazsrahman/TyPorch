#include "linalg.h"
#include <math.h>


using std::string;
using std::vector;
using std::cout;


Matrix::Matrix(int i_rows, int i_cols) {

    shape.resize(2);
    distToAdjacentEntry.resize(2);

    shape[0] = i_rows;
    shape[1] = i_cols;

    distToAdjacentEntry[0] = i_cols;
    distToAdjacentEntry[1] = 1;

    entries.resize(i_rows * i_cols);

}
Matrix::Matrix(const Matrix& original) : Tensor(original) {

    // no more to be done

}
Matrix::Matrix(const Tensor& original) : Tensor(original) {

    // no more to be done

}

// IO
void Matrix::print() const {

    printf("Rows: %d \tCols: %d\n", shape[0], shape[1]);
    for(int i = 0; i < shape[0]; i++) {
        for(int j = 0; j < shape[1]; j++) {
            printf("%1.3f   ", getEntry({i, j}));
        }
        printf("\n");
    }
 
    printf("\n");

}
int Matrix::getNumRows() const {
    return shape[0];
}
int Matrix::getNumCols() const {
    return shape[1];
}

void Matrix::simpleXavierInit() {
    srand(time(NULL)); // using time as seed parameter for random generator
	double min = -1.0 / sqrt(getNumCols());
	double max = 1.0 / sqrt(getNumCols());
	
    for(int i = 0; i < entries.size(); i++) {
        entries[i] = uniform_distribution(min, max);
    } 
}


// Binary Operators

Matrix Matrix::matMul(const Matrix& other) const {

    if(shape[1] != other.shape[0]) {
        cout << "ERROR: matrix multiplication attempted between non axn nxb matrices\n";
        exit(EXIT_FAILURE);
    }

    Matrix result(shape[0], other.shape[1]);

    double sum;
    for(int i = 0; i < shape[0]; i++) {
        for(int j = 0; j < other.shape[1]; j++) {

            sum = 0.0;
            for(int k = 0; k < shape[1]; k++) {
                sum += (getEntry({i, k}) * other.getEntry({k, j}));
            }

            result.setEntry({i, j}, sum);
        }

    }

    return result;

}


// Unary Operators

Matrix Matrix::transpose() const {

    Matrix result(shape[1], shape[0]);

    for(int r = 0; r < shape[0]; r++) {
        for(int c = 0; c < shape[1]; c++) {  
            result.setEntry({c, r}, getEntry({r, c}));
        }
    }
    return result;

}


// Conversion / General Methods

Matrix flatten(const Tensor& t, int axis) {

    int length = t.getSize();
    vector<int> shape; shape.resize(2);
    if(axis == 0) { // row vecror
        shape[0] = 1;
        shape[1] = length;
    } else {
        shape[0] = length;
        shape[1] = 1;
    }
    
    Matrix result(shape[0], shape[1]);
    
    for(int i = 0; i < length; i++) {

        result.setEntry(result.getCoordOfIndex(i), t.getEntry(t.getCoordOfIndex(i)));

    }

    return result;

}
Matrix flatten(const Tensor& t) {
    return flatten(t, 1);
}

Tensor reshape(vector<int> shape, const Matrix& v) {

    int s = 1;
    for(int i = 0; i < shape.size(); i++) {
        s *= shape[i];
    }

    if(s != v.getSize()) {
        cout << "ERROR: shape and Matrix object size do not match in reshape call\n";
        exit(EXIT_FAILURE);
    }

    Tensor result(shape);

    result.setEntries(v.getEntries());

    return result;

}