#ifndef TENSOR_H
#define TENSOR_H

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

class Tensor {

    protected:
    
        vector<double> entries; 
        vector<int> shape;

        /**
         * @brief This array caches the number of entries to jump
         * when trying to get to the next "row" of the ith dimension.
         * This information will help getIndexOfCoord.
         * 
         */
        vector<int> distToAdjacentEntry;


    public:

        /**
         * @brief default constructor for derived classes
         * 
         */
        Tensor();

        // Public for testing purposes

        /**
         * @brief Get the Coord Of Index object
         * 
         * @param index 
         * @return vector<int> 
         */
        vector<int> getCoordOfIndex(int index) const;
        /**
         * @brief Get the Index Of Coord object
         * 
         * @param coord 
         * @return int 
         */
        int getIndexOfCoord(vector<int> coord) const; 

        // CONSTRUCTORS / DESTRUCTORS

        /**
         * @brief Construct a new Tensor object
         * 
         * @param i_dims number of dimensions (i.e. 2 for marix)
         * @param i_shape specified number entries for each dimension
         */
        Tensor(vector<int> i_shape); // constructor that takes rows/calls and sets to fills Tensor with 0's
        
        /**
         * @brief Construct a new deep copied Tensor object
         * 
         * @param original the Tensor to be duplicated
         */
        Tensor(const Tensor& original); 

        /**
         * @brief Construct a new Tensor object from saved TENSOR
         * 
         * @param file_name filename in the Tensor_SAVE_DIR 
         * for Tensor data to be loaded from
         */
        void loadFrom(string file_string); // load from file
        
        /**
         * @brief saves Tensor with name file_name in TENSOR_SAVE_DIR
         * 
         * @param file_name 
         */
        void saveAs(string file_string);

        /**
         * @brief Destroy the Tensor object
         * deallocated memory for the entries of the Tensor
         */
        ~Tensor();

        // SETTERS / GETTERS - using vectors allows {c_1, c_2, ...} in line
        /**
         * @brief Setter method for the entry at the specific coordinate of the matrix
         * takes in the dims parameter to ensure that coordinate has the correct length.
         * 
         * Will Exit Failure if the coord is invalid
         * 
         * @param coord 
         * @param dims
         * @param v 
         */
        virtual void setEntry(vector<int> coord, double v);
        /**
         * @brief Getter method for the entry at the specific coordinate of the matrix
         * takes in the dims parameter to ensure that coordinate has the correct length
         * 
         * Will Exit Failure if the coord is invalid
         * 
         * @param coord
         * @param dims 
         */
        virtual double getEntry(vector<int> coord) const;
        
        // TODO: TEST THESE METHODS
        
        /**
         * @brief Directly set all the Entries object.
         * Well check to ensure that the input vector
         * has the same size as the Tensor when instantiated.
         * 
         * @param i_entries 
         */
        void setEntries(vector<double> i_entries);
        /**
         * @brief Directly get all the Entries 
         * of an object
         * 
         * @return vector<double> 
         */
        vector<double> getEntries() const;


        /**
         * @brief Get the number of entries in the tensor
         * 
         * @return int 
         */
        int getSize() const;
        /**
         * @brief Get the Dimensionality object
         * 
         * @return int 
         */
        int getDimensionality() const;
        /**
         * @brief Get the shape of the Tensor
         * 
         * @return vector<double> 
         */
        vector<int> getShape() const;
        /**
         * @brief neatly prints matrix values to console
         * 
         */
        virtual void print() const;


        // IN-PLACE METHODS
        /**
         * @brief fills Tensor with double value a
         * 
         * @param a 
         */
        void fill(double a);
        /**
         * @brief fills each entry in the Tensor it is called upon
         * with a random float value in range.
         * 
         * @param n range is within +- 1 / sqrt(n)
         */
        void randomize(int n);

        // EQUALITY OPERATORS
        /**
         * @brief returns 1 if object and other Tensor have the
         * same dimensions and 0 otherwise. To check if operations
         * such as Tensor addition or elementWiseMultiplication
         * are valid
         * 
         * @param other 
         * @return true same dimension
         * @return false different dimensions
         */
        bool sameShape(const Tensor& other) const;
        /**
         * @brief given a shape vector, returns wether 
         * the matrix has the same shape or not.
         * 
         * @param shape 
         * @return true 
         * @return false 
         */
        bool hasShape(vector<int> shape) const;
        /**
         * @brief Override of equality operator ==. Will
         * return True if matrices are of the same size and have the exact same
         * values at every corresponding entry.
         * 
         * @param other 
         * @return bool 
         */
        bool operator==(const Tensor& other) const;
        /**
         * @brief opposite of == operator
         * 
         */
        bool operator!=(const Tensor& other) const;



        // BINARY OPERATORS 
        /**
         * @brief Override of assignment operator. This ensures that the previous
         * memory allocation for the entries are deallocated. And that all the 
         * entries are properly deep copied.
         * 
         * @param other 
         * @return Tensor& 
         */
        void operator=(const Tensor& other);
        /**
         * @brief simple Tensor addition where matrices are
         * of the same size
         * 
         * @param other 
         * @return Tensor* 
         */
        Tensor operator+(const Tensor& other) const;
        /**
         * @brief simple Tensor subtraction where matrices are
         * of the same size
         * 
         * @param other 
         * @return Tensor* 
         */
        Tensor operator-(const Tensor& other) const;
        /**
         * @brief element-wise multiplication of two Tensors.
         * result will be of the same shape
         * 
         * @param B 
         * @return Tensor* 
         */
        Tensor operator*(const Tensor& other) const;
        /**
         * @brief elment-wise division of Tensors. 
         * Out put tensor will be of the same shape.
         * 
         * @param other 
         * @return Tensor 
         */
        Tensor operator/(const Tensor& other) const;

        

        // TENSOR - SCALAR BINARY OPERATORS
        /**
         * @brief returns new scaled Tensor
         * 
         * this is NOT IN PLACE
         * 
         * @param v 
         * @return Tensor* 
         */
        Tensor scaleBy(double v) const;
        /**
         * @brief returns A scaled by v, no changes made to A.
         * CURRENTLY THIS JUST CALLS and RETURNS A.scaleBy(v).
         * 
         * @param A 
         * @param v 
         * @return Tensor 
         */
        friend Tensor operator*(const Tensor& A, double v);
        /**
         * @brief returns A scaled by v without altering A.
         * CURRENTLY THIS JUST CALLS and RETURNS A.scaleBy(v).
         * 
         * @param v 
         * @param A 
         * @return Tensor 
         */
        friend Tensor operator*(double v, const Tensor& A);
        /**
         * @brief divide scalar
         * 
         * @param v 
         * @return Tensor 
         */
        Tensor operator/(double v) const;


        /**
         * @brief returns new summed Tensor
         * 
         * this is NOT IN PLACE
         * 
         * @param v 
         * @return Tensor* 
         */
        Tensor addScalar(double v) const;
        /**
         * @brief returns a Tensor with identical entries to A
         * but all with v added to them. A remains unchanged. 
         * CURRENTLY THIS JUST CALLS and RETURNS A.addScalar(v).
         * 
         * @param A 
         * @param v 
         * @return Tensor 
         */
        friend Tensor operator+(const Tensor& A, double v);
        /**
         * @brief returns a Tensor with identical entries to A
         * but all with v added to them. A remains unchanged.
         * CURRENTLY THIS JUST CALLS and RETURNS A.addScalar(v). 
         * 
         * @param v 
         * @param A 
         * @return Tensor 
         */
        friend Tensor operator+(double v, const Tensor& A);
        /**
         * @brief subtract scalar
         * 
         * @param v 
         * @return Tensor 
         */
        Tensor operator-(double v) const;

        /**
         * @brief apply returns a matrix of the same size
         * where all entries have undergone a certain operation
         * 
         * @param func 
         * @return Matrix 
         */
        Tensor apply(double (*func)(double)) const;

        // UNARY OPERATORS

  
};

class Matrix : public Tensor {

    public:

        // CONSTRUCTORS/DESTRUCTORS
        Matrix();
        Matrix(int i_rows, int i_cols);
        Matrix(const Matrix& original);
        Matrix(const Tensor& original);
        
        void loadFrom(string file_name);
        void saveAs(string file_name);
      
        // IO
        /**
         * @brief prints Matrix in standard row x col grid
         * 
         */
        void print() const override;

        /**
         * @brief Get the Num Rows in Matrix
         * 
         * @return int 
         */
        int getNumRows() const;
        /**
         * @brief Get the Num Rows in Matrix
         * 
         * @return int 
         */
        int getNumCols() const;

        /**
         * @brief fills matrix with random weights where 
         * weights are randomly choosen from range -1/sqrt(n)
         * to 1/sqrt(n). This protects from vanishing/exploding gradient
         * for neural models that use tanh or sigmoidal activation functions     
         * 
         * Note: normalXavierInit() is also available.
         * Note: He initialization (not yet implemented) is better suited
         * for models using the ReLU activation function.
         */
        void simpleXavierInit();

        /**
         * @brief 
         * 
         * Note: simpleXavierInit() is also available.
         * Note: He initialization (not yet implemented) is better suited
         * for models using the ReLU activation function.
         */
        void normalXavierInit();


        // Binary operators
        /**
         * @brief Matrix Multiplication
         * nxa * axm -> nxm
         * 
         * @return Matrix 
         */
        Matrix matMul(const Matrix& other) const;

        // UNARY OPERATORS
        /**
         * @brief returns a tranposed matrix
         * 
         * @return Matrix 
         */
        Matrix transpose() const;

        /**
         * @brief given a n x m matrix, this will return an 1 x m row vector with 
         * the row index of the maximum value for each column
         * 
         * FIGURE OUT IF THIS SHOULD BE GNERALIZED TO A TENSOR
         * 
         * @return int 
         */
        Matrix argMaxMatrix() const;


};

// Following methods written outside of class scope to 
// avoid circular dependencies

/**
 * @brief Flatten tensor to just a row or column vector matrix
 * resultant matrix is of size 1x(m*n) or (m*n)x1. The default format
 * is column or (m*n)x1.
 * 
 * @param t the input Tensor object to be flattened
 * @param axis 0 will output a row vector, 1 (default) will output a column vector
 * @return Matrix 
 */
Matrix flatten(const Tensor& t, int axis);
Matrix flatten(const Tensor&);


/**
 * @brief Reshape a flattened matrix back into a tensor of the shape 
 * inputted. Essentially creating a Tensor copy of the same size with
 * the given shape where the actual entries are all in the same order.
 * (Order of the entries field in the Tensor objects are preserved when
 * flattening a Tensor).
 * 
 * @param shape 0 will output a row vector, 1 (default) will output 
 * a column vector
 * @param v the input matrix (usually vector) that is to be reshaped
 * into a tensor of the shape shape
 * @return Tensor
 */
Tensor reshape(vector<int> shape, const Matrix& v);

/**
 * @brief Helper for all methods using
 * a uniform distribution
 * 
 */
double uniform_distribution(double low, double high);

#endif