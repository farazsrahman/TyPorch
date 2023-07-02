#include "matrix.h"
#include "tensor.h"

using std::string;
using std::vector;

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

// Binary Operators
