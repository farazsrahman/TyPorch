#include "tensor.h"
#include <math.h>

#define MAXCHAR 100
#define SAVE_DIR "saved"

using std::string;
using std::cout;

// CONSTRUCTORS / DESTRUCTORS

Tensor::Tensor() {
    // all members of tensor are protected or public so the sub classes construcor
    // can handle the rest
}
Tensor::Tensor(vector<int> i_shape) {

    shape = i_shape; // shallow copy

    distToAdjacentEntry.resize(shape.size());

    int dist = 1;

    // the product of all dimensions to the right of the nth dimension will 
    // be cached to help with retrieving indices from coords and vice versa
    for(int i = shape.size()-1; i >= 0; i--) {
        distToAdjacentEntry[i] = dist;
        dist *= shape[i];
    }

    entries.resize(dist); 
}

Tensor::Tensor(const Tensor& original) {

    // shallow copies 
    shape = original.shape;
    distToAdjacentEntry = original.distToAdjacentEntry;
    entries = original.entries;

} // copy constructor (use =operator for already declared tensors)

void Tensor::loadFrom(string file_string) {

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

    // all objects are simple types or vectors, so memory is already managed

}

// SETTERS / GETTERS

int Tensor::getIndexOfCoord(vector<int> coord) const {

    int index = 0;

    for(int dimension = 0; dimension < shape.size(); dimension++) {

        // input error checjubg
        if(coord[dimension] >= shape[dimension] || coord[dimension] < 0) {
            printf("ERROR: input coord to getIndexOfCoord() out of bounds\n");
            exit(EXIT_FAILURE);
        }

        index += coord[dimension] * distToAdjacentEntry[dimension];
    }

    return index;
}
vector<int> Tensor::getCoordOfIndex(int index) const {

    vector<int> coord; coord.resize(shape.size());

    if(index >= entries.size() || index < 0) {
        printf("ERROR: input index to getCoordOfIndex() out of bounds\n");
        exit(EXIT_FAILURE);
    }

    for(int dimension = 0; dimension < shape.size(); dimension++) {

        coord[dimension] = index / distToAdjacentEntry[dimension];
        // cout << "for loop iter\n" << coord[dimension] << "\n";
        index %= distToAdjacentEntry[dimension];

    }

    return coord;
}

void Tensor::setEntry(vector<int> coord, double v) {

    if(coord.size() != shape.size()) {
        printf("ERROR: input coord to setEntry() had a dimension mismatch\n");
        exit(EXIT_FAILURE);
    }

    entries[getIndexOfCoord(coord)] = v;

}
double Tensor::getEntry(vector<int> coord) const {

    if(coord.size() != shape.size()) {
        printf("ERROR: input coord to getEntry() had a dimension mismatch\n");
        exit(EXIT_FAILURE);
    }

    return entries[getIndexOfCoord(coord)];
}

void Tensor::print() const {

    int sizeLastDim = shape[shape.size()-1];
    vector<int> coord;
    
    for(int i = 0; i < entries.size(); i++) {

        // everytime we complete a "row"
        if(i % sizeLastDim == 0) {
            
            cout << "\n\n";
            coord = getCoordOfIndex(i);

            for(int c = 0; c < shape.size() - 1; c++) {
                cout << coord[c] << " x ";
            }
            cout << "_\n";
        }

        // printing the ith entry
        printf("%1.3f  ", entries[i]);
 
    }
    cout << "\n";


}

// IN-PLACE METHODS - These methods are very straightforward thanks to 1d entries array

void Tensor::fill(double v) {

    for(int i = 0; i < entries.size(); i++) {
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
	
    for(int i = 0; i < entries.size(); i++) {
        entries[i] = uniform_distribution(min, max);
    } 
}

// EQUALITY OPERATORS

bool Tensor::sameShape(const Tensor& other) const {
    if(shape.size() != other.shape.size()) {
        return false;
    }

    for(int i = 0; i < shape.size(); i++) {
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

    for(int i = 0; i < entries.size(); i++) {
        if(entries[i] != other.entries[i]) {
            return false;
        }
    }

    return true;
}

// TENSOR - TENSOR BINARY OPERATORS

void Tensor::operator=(const Tensor& original) {

    // shallow copies 
    shape = original.shape;
    distToAdjacentEntry = original.distToAdjacentEntry;
    entries = original.entries;

}
Tensor Tensor::operator+(const Tensor& other) const {

    if (!sameShape(other)) {
        printf("ERROR: dimension mismatch on + operator");
        exit(EXIT_FAILURE);
    }

    Tensor result(shape);

    for(int i = 0; i < entries.size(); i++) {
        result.entries[i] = entries[i] + other.entries[i];
    }

    return result;
}
Tensor Tensor::operator-(const Tensor& other) const {

    if (!sameShape(other)) {
        printf("ERROR: dimension mismatch on - operator");
        exit(EXIT_FAILURE);
    }

    Tensor result(shape);

    for(int i = 0; i < entries.size(); i++) {
        result.entries[i] = entries[i] - other.entries[i];
    }

    return result;

}
Tensor Tensor::operator*(const Tensor& other) const {

    if (!sameShape(other)) {
        printf("ERROR: dimension mismatch on * operator");
        exit(EXIT_FAILURE);
    }

    Tensor result(shape);

    for(int i = 0; i < entries.size(); i++) {
        result.entries[i] = entries[i] * other.entries[i];
    }

    return result;

}
Tensor Tensor::operator/(const Tensor& other) const {

    if (!sameShape(other)) {
        printf("ERROR: dimension mismatch on / operator");
        exit(EXIT_FAILURE);
    }

    Tensor result(shape);

    for(int i = 0; i < entries.size(); i++) {
        result.entries[i] = entries[i] * other.entries[i];
    }

    return result;

}


// TENSOR - SCALAR BINARY OPERATORS

Tensor Tensor::scaleBy(double v) const {

    Tensor result(shape);

    for(int i = 0; i < entries.size(); i++) {
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
    
    Tensor result(shape);

    for(int i = 0; i < entries.size(); i++) {
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

    Tensor result(shape);
    for(int i = 0; i < entries.size(); i++) {
        result.entries[i] = (*func)(entries[i]);
    }

    return result;

}