#include "linalg.h"

#include <math.h>
#include <cmath>
#include <random>
#include <thread>
#include <functional>

#define MAXCHAR 100
#define SAVE_DIR "saved"
#define NUM_THREADS 4

using std::string;
using std::cout;
using std::thread;
using std::function;

// HELPERS

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
/**
 * @brief returns random double from low to high. I think..
 * 
 * @param low 
 * @param high 
 * @return double 
 */
double uniform_distribution(double low, double high) {
	std::random_device rd;   // Obtain a random seed from hardware
    std::mt19937 gen(rd());  // Seed the Mersenne Twister PRNG
    std::uniform_real_distribution<> dis(low, high);
    return dis(gen);
}


// CONSTRUCTORS / DESTRUCTORS

Tensor::Tensor() {
    // all members of tensor are protected or public so 
    // the subclasses construcor can handle the rest
    // this also allows for lazy construction using
    // the setShape method.
}
Tensor::Tensor(vector<int> i_shape) {
    // cout << "Compiled with: " << COMPILER_USED << std::endl;

    shape = i_shape; // shallow copy

    distToAdjacentEntry.resize(shape.size());

    // the prefix sum of the shape vector will 
    // be cached to help with retrieving indices 
    // from coords and vice versa

    int dist = 1;
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
Tensor::~Tensor() {

    // all objects are simple types or vectors, so memory is already managed

}


// SETTERS / GETTERS

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
void Tensor::setEntries(vector<double> i_entries) {

    if(i_entries.size() != getSize()) {
        cout << "ERROR: setEntries passed an input of incorrect size \n";
        exit(EXIT_FAILURE);
    }

    entries = i_entries;

}
vector<double> Tensor::getEntries() const {
    return entries;
}
int Tensor::getSize() const {
    return entries.size();
}
int Tensor::getDimensionality() const {
    return shape.size();
}
void Tensor::setShape(vector<int> i_shape) {
    
    shape = i_shape; // shallow copy

    distToAdjacentEntry.resize(shape.size());
    
    // disToAdjacentEntry is a prefix sum
    int dist = 1;
    for(int i = shape.size()-1; i >= 0; i--) {
        distToAdjacentEntry[i] = dist;
        dist *= shape[i];
    }

    entries.resize(dist); 

}
vector<int> Tensor::getShape() const {
    return shape;
}

void Tensor::print() const {

    int sizeLastDim = shape[shape.size()-1];
    vector<int> coord;

    cout << "Tensor Shape: ";
    for(int i = 0; i < shape.size() - 1; i++) {
        cout << shape[i] << " x ";
    }
    cout << shape[shape.size()-1] << "";
    
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

// IN-PLACE METHODS

void Tensor::fill(double v) {

    for(int i = 0; i < entries.size(); i++) {
        entries[i] = v;
    }

}
void Tensor::randomize(double min, double max) {
    srand(time(NULL)); // using time as seed parameter for random generator
    
    double (*uni_dist)(double, double) = &uniform_distribution;
    vector<double>* entries_p = &entries; 

    int n_entries = getSize();

    int entriesPerThread = n_entries / (NUM_THREADS - 1);
    vector<thread> threads; threads.reserve(NUM_THREADS);
        
    for(int t = 0; t < NUM_THREADS; t++) {
        threads.emplace_back([t, entriesPerThread, entries_p, uni_dist, min, max, n_entries](){
            
            int index;
            for(int i = 0; i < entriesPerThread; i++) {

                index = t * entriesPerThread + i;
                if(index < n_entries) (*entries_p)[index] = uniform_distribution(min, max);

            }

        });
    }

    for(auto &thread: threads) thread.join();
    threads.clear();

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
bool Tensor::hasShape(vector<int> otherShape) const {
    // TODO: TEST
    if(otherShape.size() != shape.size()) {
        return false;
    }

    for(int i = 0; i < shape.size(); i++) {
        if(shape[i] != otherShape[i]) {
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
bool Tensor::operator!=(const Tensor& other) const {
    
    return !(*this == other); // * is dereferencing this
}


// (TENSOR x TENSOR) BINARY OPERATORS

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

        // if dividing by zero, approx with the smallest possible float
        if(other.entries[i] == 0) {
            cout << "0 approx used\n";
            result.entries[i] = entries[i] / std::numeric_limits<float>::min();

        } else {
            result.entries[i] = entries[i] / other.entries[i];

        }

    }

    return result;

}

// INPLACE (INCREMENT) OPERATORS - skipped Tests on these...

void Tensor::operator+=(const Tensor& other) {
    if (!sameShape(other)) {
        printf("ERROR: dimension mismatch on += operator");
        exit(EXIT_FAILURE);
    }
    
    vector<double> otherEntries = other.getEntries();
        
    for(int i = 0; i < getSize(); i ++) {
        entries[i] += otherEntries[i];
    }
}
void Tensor::operator-=(const Tensor& other) {
    if (!sameShape(other)) {
        printf("ERROR: dimension mismatch on -= operator");
        exit(EXIT_FAILURE);
    }
    
    vector<double> otherEntries = other.getEntries();
    
    for(int i = 0; i < getSize(); i ++) {
        entries[i] -= otherEntries[i];
    }
}
void Tensor::operator*=(const Tensor& other) {
    if (!sameShape(other)) {
        printf("ERROR: dimension mismatch on *= operator");
        exit(EXIT_FAILURE);
    }
    
    vector<double> otherEntries = other.getEntries();
    vector<double>* entries_p = &entries;
    
    int entriesPerThread = getSize() / NUM_THREADS;

    vector<thread> threads;
    threads.reserve(NUM_THREADS + 1);
        
    for(int t = 0; t < NUM_THREADS; t++) {
        threads.emplace_back([t, entriesPerThread, &otherEntries, entries_p](){
            
            int index;
            for(int i = 0; i < entriesPerThread; i++) {

                index = t * entriesPerThread + i;
                (*entries_p)[index] *= otherEntries[index];

            }
        });
    }

    // for the remaining elements
    threads.emplace_back([entriesPerThread, &otherEntries, entries_p](){
            // cout << "NOTE: STARTING THREAD " << NUM_THREADS << "with " << entriesPerThread << " entries" << "\n";

            int index;
            int numRemaining = otherEntries.size() - (NUM_THREADS * entriesPerThread);
            for(int i = 0; i < numRemaining; i ++) {

                index = NUM_THREADS * entriesPerThread + i;
                (*entries_p)[index] *= otherEntries[index];

            }
    });

    for(auto &thread: threads) thread.join();
    threads.clear();
    
}
void Tensor::operator/=(const Tensor& other) {
    if (!sameShape(other)) {
        printf("ERROR: dimension mismatch on /= operator");
        exit(EXIT_FAILURE);
    }
    
    vector<double> otherEntries = other.getEntries();
    
    for(int i = 0; i < getSize(); i ++) {
        entries[i] /= otherEntries[i];
    }
}


// (TENSOR x SCALAR) BINARY OPERATORS

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
Tensor Tensor::operator/(double v) const {
    return scaleBy((1/v));
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
Tensor Tensor::operator-(double v) const {
    return addScalar(-1*v);
}


// Unary Operators

Tensor Tensor::apply(double (*func)(double)) const {

    Tensor result(shape);
    for(int i = 0; i < entries.size(); i++) {
        result.entries[i] = (*func)(entries[i]);
    }

    return result;

}
double Tensor::getSum() const {
    double sum = 0;
    for(double entry: getEntries()) {
        sum += entry;
    }
    return sum;
}
double Tensor::getMean() const {
    return getSum()/getSize();
}


// CUSTOM BINARY OPERATORS

Tensor computeElementWise(const function<double(double, double)>& lambda,
                          const Tensor& prediction, const Tensor& target) {

    vector<double> result_e(target.getSize());
    vector<double> prediction_e = prediction.getEntries();
    vector<double> target_e = target.getEntries();

    for(int i = 0; i < target.getSize(); i++) {
        result_e[i] = lambda(prediction_e[i], target_e[i]);
    }
    
    Tensor result(target.getShape());
    result.setEntries(result_e);

    return result;
}

