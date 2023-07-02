#ifndef MATRIX_H
#define MATRIX_H // this has the same effect of Pragma Once

#include <iostream>
#include <string>

#include "tensor.h"

using std::string;


class Matrix : public Tensor {

    public:

        // CONSTRUCTORS/DESTRUCTORS
        Matrix(int i_rows, int i_cols);
        Matrix(const Matrix& original);
        
        void loadFrom(string file_name);
        void saveAs(string file_name);
      
        // IO
        /**
         * @brief prints Matrix in standard row x col grid
         * 
         */
        void print() const override;

        // Binary operators
        /**
         * @brief Matrix Multiplication
         * nxa * axm -> nxm
         * 
         * @return Matrix 
         */
        Matrix matMul() const;

        // UNARY OPERATORS
        /**
         * @brief returns a tranposed matrix
         * 
         * @return Matrix 
         */
        Matrix tranpose() const;
        /**
         * @brief flatten matrix to just a row or column vector
         * resultant matrix is of size 1x(m*n) or (m*n)x1. The default is 
         * column or (m*n)x1.
         * 
         * @param axis 0 will output a row vector, 1 (default) will output a column vector
         * @return Matrix 
         */
        Matrix flattenMatrix(int axis) const; 
        /**
         * @brief given a n x m matrix, this will return an 1 x m row vector with 
         * the row index of the maximum value for each column
         * 
         * @return int 
         */
        Matrix argMaxMatrix() const;


};

#endif