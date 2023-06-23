#include "tensor.h"
#include <math.h>

#define MAXCHAR 100
#define SAVE_DIR "saved"

using std::string;

// CONSTRUCTORS / DESTRUCTORS

Tensor::Tensor() {
    // all members of tensor are protected or public so the sub classes construcor
    // can handle the rest
}
Tensor::Tensor(int i_dims, int* i_shape) {

    numberOfDims = i_dims;

    shape = new int[i_dims];
    distToAdjacentEntry = new int[i_dims];
    

    int dist = 1;

    // we are going to iterate down rows, and save the number entries
    // in the sub-tensor starting from dimension i onward to distToAdjacentEntry 
    // in order to help with getIndexOfCoord() later on.
    for(int i = numberOfDims-1; i >= 0; i--) {

        distToAdjacentEntry[i] = dist;
        dist *= shape[i];
    
    }

    // after the iteration the final value of dist will be the 
    // cumalative product of all the shape values
    // this is the overall size of our tensor
    size = dist; // think of dist as the number of spaces to jump to get past our entries array

    entries = new double[size]; // 1d representation of n-dimensional matrix

}
Tensor::Tensor(const Tensor& original) {

    numberOfDims = original.numberOfDims;
    size = original.size;

    shape = new int[numberOfDims];
    distToAdjacentEntry = new int[numberOfDims];
    entries = new double[size];

    for(int i = 0; i < numberOfDims; i++) {
        shape[i] = original.shape[i];
        distToAdjacentEntry[i] = original.distToAdjacentEntry[i];
    }

    for(int i = 0; i < size; i++) {
        entries[i] = original.entries[i];
    }

} // copy constructor (auto invoked when using equal sign)

Tensor::Tensor(string file_string) {

    // im goin to cry...
    printf("ERROR: Tensor load not implemented");
    exit(EXIT_FAILURE);

} // load from file
void Tensor::saveAs(string file_string) {

    // im goin to cry...
    printf("ERROR: Tensor save not implemented");
    exit(EXIT_FAILURE);

}

Tensor::~Tensor() {

    // items to deallocate

    // shape
    delete[] shape;
    // entries
    delete[] entries;
    // distToAdjacentEntry
    delete[] distToAdjacentEntry;

}

// SETTERS / GETTERS

int Tensor::getIndexOfCoord(vector<int> coord) const {

    int index = 0;
    for(int i = 0; i < numberOfDims; i++) {
        index += coord[i] + distToAdjacentEntry[i];
    }

    return index;
}
vector<int> Tensor::getCoordOfIndex(int index) const {

    vector<int> coord;

    for(int dimension = 0; dimension < numberOfDims; dimension++) {

        coord[dimension] = index / distToAdjacentEntry[dimension];
        index %= distToAdjacentEntry[dimension];

    }

    return coord;
}

void Tensor::setEntry(vector<int> coord, double v) {

    if(coord.size() != numberOfDims) {
        printf("ERROR: input coord to setEntry() had a dimension mismatch\n");
        exit(EXIT_FAILURE);
    }

    entries[getIndexOfCoord(coord)] = v;

}
double Tensor::getEntry(vector<int> coord) const {

    if(coord.size() != numberOfDims) {
        printf("ERROR: input coord to getEntry() had a dimension mismatch\n");
        exit(EXIT_FAILURE);
    }

    return entries[getIndexOfCoord(coord)];
}

void Tensor::print() const {

    // this print will format the output as follows
    // n = numberOfDims

    // a1 x a2 x ... x a(n-1) x 0 x
    // { row of all numbers with fixed coordinates a1 ... a(n-1) }
    // ^^ this organization is easy as all the items are 


    // ... so on for all combinations of a1 x ... x a(n-1)

    int sizeLastDim = shape[numberOfDims-1];
    
    for(int i = 0; i < size; i++) {

        // everytime we complete a "row"
        if(i % sizeLastDim == 0) {
            printf("\n\n ");
            for (const int& coordEntry : getCoordOfIndex(i)) {
                printf("%d x ", coordEntry);
            }
            printf("\n");
        }

        // printing the ith entry
        printf("%1.3f  ", entries[i]);

    }


}


// IN-PLACE METHODS - These methods are very straightforward thanks to 1d entries array

void Tensor::fill(double v) {

    for(int i = 0; i < size; i++) {
        entries[i] = v;
    }

}

/**
 * @brief returns random double from low to high. I think..
 * 
 * @param low 
 * @param high 
 * @return double 
 */
double uniform_distribution(double low, double high) {
	double difference = high - low; // The difference between the two
	int scale = 10000;
	int scaled_difference = (int)(difference * scale);
	return low + (1.0 * (rand() % scaled_difference) / scale);
}
void Tensor::randomize(int n) {
    srand(time(NULL)); // using time as seed parameter for random generator
	double min = -1.0 / sqrt(n);
	double max = 1.0 / sqrt(n);
	
    for(int i = 0; i < size; i++) {
        entries[i] = uniform_distribution(min, max);
    } 
}

// EQUALITY OPERATORS

bool Tensor::sameShape(const Tensor& other) const {
    if(numberOfDims != other.numberOfDims) {
        return false;
    }

    for(int i = 0; i < numberOfDims; i++) {
        if(shape[i] != other.shape[i]) {
            return false;
        }
    }

    return true;
}
bool Tensor::operator==(const Tensor& other) const {

    if(!sameShape(other)) {
        return false;
    }

    // TODO: ADD APROXIMATELY SAME FEATURE?

    for(int i = 0; i < size; i++) {
        if(entries[i] != other.entries[i]) {
            return false;
        }
    }

    return true;
}

// TENSOR - TENSOR BINARY OPERATORS

Tensor& Tensor::operator=(const Tensor& other) {

    // deallocation
    delete[] shape;
    delete[] entries;
    delete[] distToAdjacentEntry;

    numberOfDims = other.numberOfDims;
    size = other.size;

    shape = new int[numberOfDims];
    distToAdjacentEntry = new int[numberOfDims];

    shape = new int[size];

    for(int i = 0; i < numberOfDims; i++) {
        shape[i] = other.shape[i];
        distToAdjacentEntry[i] = other.distToAdjacentEntry[i];
    }

    for(int i = 0; i < size; i++) {
        entries[i] = other.entries[i];
    }

}
Tensor Tensor::operator+(const Tensor& other) const {

    if (!sameShape(other)) {
        printf("ERROR: dimension mismatch on + operator");
        exit(EXIT_FAILURE);
    }

    Tensor result(numberOfDims, shape);

    for(int i = 0; i < size; i++) {
        result.entries[i] = entries[i] + other.entries[i];
    }

    return result;
}
Tensor Tensor::operator-(const Tensor& other) const {

    if (!sameShape(other)) {
        printf("ERROR: dimension mismatch on - operator");
        exit(EXIT_FAILURE);
    }

    Tensor result(numberOfDims, shape);

    for(int i = 0; i < size; i++) {
        result.entries[i] = entries[i] - other.entries[i];
    }

    return result;

}
Tensor Tensor::operator*(const Tensor& other) const {

    if (!sameShape(other)) {
        printf("ERROR: dimension mismatch on * operator");
        exit(EXIT_FAILURE);
    }

    Tensor result(numberOfDims, shape);

    for(int i = 0; i < size; i++) {
        result.entries[i] = entries[i] * other.entries[i];
    }

    return result;

}
Tensor Tensor::operator/(const Tensor& other) const {

    if (!sameShape(other)) {
        printf("ERROR: dimension mismatch on / operator");
        exit(EXIT_FAILURE);
    }

    Tensor result(numberOfDims, shape);

    for(int i = 0; i < size; i++) {
        result.entries[i] = entries[i] * other.entries[i];
    }

    return result;

}


// TENSOR - SCALAR BINARY OPERATORS

Tensor Tensor::scaleBy(double v) const {

    Tensor result(numberOfDims, shape);

    for(int i = 0; i < size; i++) {
        result.entries[i] = entries[i] * v;
    }
    return result;

}
Tensor operator*(const Tensor& A, double v) {
    return A.scaleBy(v);
}
Tensor operator*(double v, const Tensor& A) {
    return A.scaleBy(v);
}

Tensor Tensor::addScalar(double v) const {
    
    Tensor result(numberOfDims, shape);

    for(int i = 0; i < size; i++) {
        result.entries[i] = entries[i] + v;
    }
    return result;

}
Tensor operator+(const Tensor& A, double v) {
    return A.addScalar(v);
}
Tensor operator+(double v, const Tensor& A) {
    return A.addScalar(v);
}

Tensor Tensor::apply(double (*func)(double)) const {

    Tensor result(numberOfDims, shape);
    for(int i = 0; i < size; i++) {
        result.entries[i] = (*func)(entries[i]);
    }

    return result;

}