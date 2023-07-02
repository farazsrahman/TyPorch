#ifndef TENSOR_H
#define TENSOR_H

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

class Tensor {

    protected:

        // no longer need these because we are using vectors that store size

        // int numberOfDims;
        // int size;

        vector<double> entries; 
        vector<int> shape;

        /**
         * @brief This array caches the number of entries to jump
         * when trying to get to the next "row" of the ith dimension.
         * This information will help getIndexOfCoord.
         * 
         */
        vector<int> distToAdjacentEntry;

        /**
         * @brief default constructor for derived classes
         * 
         */
        Tensor();

    public:

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
         * Idk why but this was the very specific distribution that the guy on YouTube used.
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
         * @brief Override of equality operator ==. Will
         * return True if matrices are of the same size and have the exact same
         * values at every corresponding entry.
         * 
         * @param other 
         * @return bool 
         */
        bool operator==(const Tensor& other) const;



        // TENSOR - TENSOR BINARY OPERATORS
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
         * @brief apply returns a matrix of the same size
         * where all entries have undergone a certain operation
         * 
         * @param func 
         * @return Matrix 
         */
        Tensor apply(double (*func)(double)) const;


};

#endif