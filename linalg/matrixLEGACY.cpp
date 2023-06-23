// #ifndef MATRIX_CPP
// #define MATRIX_CPP

// #include <iostream>
// #include <string>
// #include <time.h>
// #include <math.h>

// // the load and save functionality depends on the platform supporting
// // this library
// #include <filesystem> 

// #include "matrix.h"

// using namespace std;

// #define MAXCHAR 100
// #define MATRIX_SAVE_DIR "saved_matrices"

// // CONSTRUCTORS / DESTRUCTORS

// Matrix::Matrix(int i_rows, int i_cols) {

//     numRows = i_rows;
//     numCols = i_cols;

//     entries = new double*[numRows];

//     for(int i = 0; i < numRows; i++) {
//         entries[i] = new double[numCols];

//         for(int j = 0; j < numCols; j++) {
//             entries[i][j] = 0.0;

//         }
//     }

// }

// Matrix::Matrix(const Matrix& original) {

//     numRows = original.numRows;
//     numCols = original.numCols;

//     entries = new double*[numRows];

//     for(int i = 0; i < numRows; i++) {
//         entries[i] = new double[numCols];

//         for(int j = 0; j < numCols; j++) {
//             entries[i][j] = original.entries[i][j];

//         }
//     }

// }

// Matrix::Matrix(string file_name) {

//     string full_path = string(MATRIX_SAVE_DIR) + "/" + file_name;

//     FILE* file = fopen(full_path.c_str(), "r");

//     if(file == NULL) {
//         printf("ERROR: FILE \"%s\" NOT FOUND on matrix load\n\n", full_path.c_str());
//         exit(EXIT_FAILURE);
//     }
    
//     else {
//         char entry[MAXCHAR];

//         fgets(entry, MAXCHAR, file);
//         numRows = atoi(entry);

//         fgets(entry, MAXCHAR, file);
//         numCols = atoi(entry);

//         entries = new double*[numRows];

//         for(int i = 0; i < numRows; i++) {
//             entries[i] = new double[numCols];

//             for(int j = 0; j < numCols; j++) {
//                 // each entry stored on a new line for easy retrieval
//                 fgets(entry, MAXCHAR, file);
//                 entries[i][j] = strtod(entry, NULL);

//             }
//         }

//         fclose(file);
//         printf("Sucessfully loaded matrix from %s\n", full_path.c_str());

//     }
// }

// void Matrix::saveAs(string file_name) {

//     string full_path = string(MATRIX_SAVE_DIR) + "/" + file_name;
//     FILE* file = fopen(full_path.c_str(), "w"); // open file writer

//     fprintf(file, "%d \n", numRows);
//     fprintf(file, "%d \n", numCols);

//     for(int i = 0; i < numRows; i++) {
//         for(int j = 0; j < numCols; j++) {

//             // each entry stored on a new line for easy retrieval
// 			fprintf(file, "%.6f\n", entries[i][j]);

//         }
//     }

//     fclose(file);
//     printf("Matrix has been saved to %s in working directory", full_path.c_str());

// }

// Matrix::~Matrix() {

//     //deallocate memory from the dynamically allocated array
//     for (int i = 0; i < numRows; ++i)
//         delete[] entries[i];
//     delete[] entries;  

// }


// // IN-PLACE METHODS




// // SETTERS / GETTERS

// void Matrix::setEntry(int row, int col, double v) {

//     if(row < 0 || row >= numRows || col < 0 || col >= numCols) {
//         printf("ERROR: arg to setEntry OUT OF BOUNDS\n");
//         return;
//     }

//     entries[row][col] = v;

// }

// double Matrix::getEntry(int row, int col) const {

//     if(row < 0 || row >= numRows || col < 0 || col >= numCols) {
//         printf("ERROR: OUT OF BOUNDS argument to getEntry \n");
//         exit(EXIT_FAILURE);
//     }

//     return entries[row][col];

// }

// void Matrix::print() const {

//     printf("Rows: %d \tCols: %d\n", numRows, numCols);

//     for(int i = 0; i < numRows; i++) {
//         for(int j = 0; j < numCols; j++) {
//             printf("%1.3f   ", entries[i][j]);
//         }
//         printf("\n");
//     }
 
//     printf("\n");

// }

// // CONSIDER MOVING TO A SEPERATE FILE FOR SIMPLICITY


// // EQUALITY OPERATORS

// bool Matrix::sameDimensions(const Matrix& other) const { 

//     if(numRows == other.numRows && numCols == other.numCols) {
//         return true;
//     } 

//     return false;

// }

// bool Matrix::operator==(const Matrix& other) const {

//     if (!sameDimensions(other)) {
//         return false; // Matrices with different dimensions are not equal
//     }

//     for (int i = 0; i < numRows; i++) {
//         for (int j = 0; j < numCols; j++) {
//             if (entries[i][j] != other.entries[i][j]) {
//                 return false; // If any entry is different, matrices are not equal
//             }
//         }
//     }

//     return true; // Matrices are equal

// }


// // BINARY MATRIX MANIPULATION OPERATORS

// Matrix& Matrix::operator=(const Matrix& other) {

//     if(this == &other) { // i.e. they are the EXACT same locatiion in memory
//         return *this; // return a reference and make no other changes
//     }

//     // Deallocate existing memory
//     for (int i = 0; i < numRows; ++i) {
//         delete[] entries[i];
//     }
//     delete[] entries;

//     // Copy dimensions from the other matrix
//     numRows = other.numRows;
//     numCols = other.numCols;

//     // Allocate new memory
//     entries = new double*[numRows];
//     for (int i = 0; i < numRows; ++i) {
//         entries[i] = new double[numCols];
//     }

//     // Copy the entries from the other matrix
//     for (int i = 0; i < numRows; ++i) {
//         for (int j = 0; j < numCols; ++j) {
//             entries[i][j] = other.entries[i][j];
//         }
//     }

//     return *this;
// }

// Matrix Matrix::operator+(const Matrix& other) const {

//     if(!sameDimensions(other)) {
//         printf("ERROR: DIMENSION MISMATCH Matrix addition attempted with matrices of different size\n\n");
//         exit(EXIT_FAILURE);
//     }

//     Matrix sumMatrix(numRows, numCols);

//     for(int i = 0; i < numRows; i++) {
//         for(int j = 0; j < numCols; j++) {

//             sumMatrix.entries[i][j] = entries[i][j] + other.entries[i][j];
//         }
//     }
//     return sumMatrix;
// }

// Matrix Matrix::operator-(const Matrix& other) const {

//     if(!sameDimensions(other)) {
//         printf("ERROR: DIMENSION MISMATCH Matrix addition attempted with matrices of different size\n\n");
//         exit(EXIT_FAILURE);
//     }

//     Matrix differenceMatrix(numRows, numCols);

//     for(int i = 0; i < numRows; i++) {
//         for(int j = 0; j < numCols; j++) {

//             differenceMatrix.setEntry(i, j, entries[i][j] - other.entries[i][j]);
//         }
//     }
//     return differenceMatrix;
// }

// Matrix Matrix::operator*(const Matrix& other) const {

//     if(numCols == other.numRows) {

//         Matrix productMatrix(numRows, other.numCols);

//         for(int i = 0; i < numRows; i++) {
//             for(int j = 0; j < other.numCols; j++) {

//                 double sum = 0.0;

//                 for(int k = 0; k < other.numRows; k++) {

//                     sum += (entries[i][k] * other.entries[k][j]);

//                 }

//                 productMatrix.entries[i][j] = sum;
//             }
//         }

//         return productMatrix;

//     } else {

//         printf("ERROR: DIMENSION MISMATCH %dx%d and %dx%d cannot be matrix multiplied\n\n", numRows, numCols, other.numRows, other.numCols);
// 		exit(EXIT_FAILURE);

//     }

// }

// Matrix Matrix::elementWiseMultiply(const Matrix& other) const {

//     if(!sameDimensions(other)) {
//         printf("ERROR: DIMENSION MISMATCH Matrix elementWiseMultiply attempted with matrices of different size\n\n");
//         exit(EXIT_FAILURE);
//     }

//     Matrix productsMatrix(numRows, numCols);

//     for(int i = 0; i < numRows; i++) {
//         for(int j = 0; j < numCols; j++) {

//             productsMatrix.entries[i][j] = entries[i][j] * other.entries[i][j];
//         }
//     }
//     return productsMatrix;

// }


// // MATRIX - SCALAR BINARY OPERATORS

// // FIGURE OUT IF USING REFERENCES WOULD BE A MORE EFFICIENT WAY
// // TO PASS THE INFORMATION AROUND THEN RAW MATRIX RETURNS

// Matrix Matrix::scaleBy(double v) const {

//     Matrix scaledMatrix(numRows, numCols);

// 	for(int i = 0; i < scaledMatrix.numRows; i++) {
// 		for(int j = 0; j < scaledMatrix.numCols; j++) {

// 			scaledMatrix.entries[i][j] = entries[i][j] * v;
// 		}
// 	}
// 	return scaledMatrix;

// }
// Matrix operator*(const Matrix& A, double v) {
//     return A.scaleBy(v);
// }
// Matrix operator*(double v, const Matrix& A) {
//     return A.scaleBy(v);
// }


// Matrix Matrix::addScalar(double v) const {

//     Matrix addedMatrix(numRows, numCols);

//     for(int i = 0; i < addedMatrix.numRows; i++) {
// 		for(int j = 0; j < addedMatrix.numCols; j++) {

// 			addedMatrix.entries[i][j] = entries[i][j] + v;
// 		}
// 	}
// 	return addedMatrix;

// }
// Matrix operator+(const Matrix& A, double v) {
//     return A.addScalar(v);
// }
// Matrix operator+(double v, const Matrix& A) {
//     return A.addScalar(v);
// }

// // MATRIX UNARY OPERATORS

// Matrix Matrix::tranpose() const {

//     Matrix transposedMatrix(numCols, numRows);

//     for(int i = 0; i < numRows; i++) {
//         for(int j = 0; j < numCols; j++) {
            
//             transposedMatrix.entries[j][i] = entries[i][j];
//         }
//     }
//     return transposedMatrix;
// }

// Matrix Matrix::argMaxMatrix() const {

//     Matrix argMax(1, numCols);

//     for(int j = 0; j < numCols; j++) {
//         int max = entries[0][j];
//         int maxIndex = 0;

//         for(int i = 0; i < numRows; i++) {
//             if(entries[i][j] > max) { 
//                 max = entries[i][j];
//                 maxIndex = i;
//             }
//         }
//         argMax.setEntry(0, j, maxIndex);
//     }
//     return argMax;
// }

// Matrix Matrix::flattenMatrix(int axis = 1) const {

//     // case col
//     if(axis == 1) {
//         Matrix flattenedCol(numRows * numCols, 1);

//         for(int i = 0; i < numRows; i++) {
//             for(int j = 0; j < numCols; j++) {

//                 flattenedCol.setEntry(j * numRows + i, 0, entries[i][j]);
//             }
//         }
//         return flattenedCol;
//     }

//     // case row
//     if(axis == 0) {
//         Matrix flattenedRow(1, numRows * numCols);

//         for(int i = 0; i < numRows; i++) {
//             for(int j = 0; j < numCols; j++) {

//                 flattenedRow.setEntry(0, i * numCols + j, entries[i][j]);
//             }
//         }
//         return flattenedRow;
//     }

//     // fail state if neither ifs returned
//     printf("ERROR: axis argument to flattenMatrix must be 0 or 1\n");
// 	exit(EXIT_FAILURE);

// }

// Matrix Matrix::apply(double (*func)(double)) const {

//     Matrix appliedMatrix(numRows, numCols);
// 	for (int i = 0; i < numRows; i++) {
// 		for (int j = 0; j < numCols; j++) {

// 			appliedMatrix.entries[i][j] = (*func)(entries[i][j]);
// 		}
// 	}
// 	return appliedMatrix;

// }


// #endif
