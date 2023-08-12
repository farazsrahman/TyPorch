#include "linalg.h"
#include <math.h>
#include <thread>
#include <functional>

using std::string;
using std::vector;
using std::cout;
using std::thread;

#define NUM_THREADS 4 // ideally this should be the same as TensorMP.cpp


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

    int n_rows = shape[0], n_cols = other.shape[1], n_cols_left = shape[1];

    Matrix result(n_rows, n_cols); result.fill(0);

    vector<double>* result_ep = &(result.entries);  // result entries pointer
    vector<double> left_e = entries;                // left entries 
    vector<double> right_e = other.entries;         // right entries 
    

    // multi-threading setup
    int rowsPerThread = n_rows / (NUM_THREADS - 1);
    int remainingRows = n_rows % (NUM_THREADS - 1);
    vector<thread> threads; threads.reserve(NUM_THREADS);

    // starting first NUM_THREADS - 1 threads
    for(int threadNum = 0; threadNum < NUM_THREADS - 1; threadNum++) {
        threads.emplace_back([threadNum, rowsPerThread, result_ep, &left_e, &right_e, 
                                n_rows, n_cols, n_cols_left](){
            
            // cout << "STARTING THREAD: " << threadNum << "\n";

            int r; double sum;
            for(int i = 0; i < rowsPerThread; i++) {
                r = threadNum * rowsPerThread + i;

                
                for(int c = 0; c < n_cols; c++) {
                    
                    sum = 0.0;
                    for(int k = 0; k < n_cols_left; k++) 
                        sum += left_e[r * n_cols_left + k] * right_e[k * n_cols + c];

                    (*result_ep)[r * n_cols + c] = sum;

                }
            } 
        });
    }

    // starting last thread
    threads.emplace_back([remainingRows, rowsPerThread, result_ep, &left_e, &right_e, 
                            n_rows, n_cols, n_cols_left]() {

        // cout << "STARTING THREAD: " << NUM_THREADS - 1 << "\n";
        
        int r; double sum;
        for(int i = 0; i < remainingRows; i++) {
            r = (NUM_THREADS - 1) * rowsPerThread + i;
            
            for(int c = 0; c < n_cols; c++) {
                sum = 0.0;
                
                for(int k = 0; k < n_cols_left; k++) 
                    sum += left_e[r * n_cols_left + k] * right_e[k * n_cols + c];

                (*result_ep)[r * n_cols + c] = sum;

            }
        }
    });

             

    for(auto &thread: threads) thread.join();
    threads.clear();

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

Tensor reshape(vector<int> shape, const Tensor& v) {

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