#include "./../../linalg/linalg.h"
#include "./../neural.h"
#include "./../functions.cpp"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>

using std::cout;
using std::vector;
using std::string;
using std::endl;

struct MNISTImage {
    double label;  // 1 for 7s, 0 for non-7s
    vector<double> pixels;
};

MNISTImage getImageOfLine(const string& s) {
    char delimiter = ',';
    string token;
    MNISTImage result;
    std::istringstream tokenStream(s);

    bool first = true;
    while (std::getline(tokenStream, token, delimiter)) {
        if(first) { 
            result.label = double(std::stoi(token)); 
            first = false;
        } else {
            result.pixels.push_back(double(std::stoi(token))/255.0);  // Normalize to [0,1]
        }
    }
    return result;
}

void readMNISTCSV(const string& filename, vector<MNISTImage>& images, int max_samples = -1) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    string line; int n = 0;
    std::getline(file, line); // first line is column headers
    while (std::getline(file, line)) {
        if(max_samples > 0 && n >= max_samples) break;
        images.push_back(getImageOfLine(line)); 
        n++;
    }
    file.close();
}

void readMNISTData(const string& filename, vector<Matrix>& train_x, vector<Matrix>& train_y) {
    vector<MNISTImage> images;
    readMNISTCSV(filename, images);
    
    train_x.reserve(images.size());
    train_y.reserve(images.size());
    
    for(const auto& img : images) {
        // Create input matrix (flattened image)
        Matrix x(1, 784);
        for(size_t i = 0; i < img.pixels.size(); i++) {
            x.setEntry({0, static_cast<int>(i)}, img.pixels[i]);
        }
        train_x.push_back(x);
        
        // Create output matrix (binary label)
        Matrix y(1, 1);
        y.setEntry({0, 0}, img.label);
        train_y.push_back(y);
    }
}

// Function to get indices of 7s and non-7s
void splitDatasetIndices(const vector<Matrix>& train_y, vector<size_t>& seven_indices, vector<size_t>& non_seven_indices) {
    for(size_t i = 0; i < train_y.size(); i++) {
        if(train_y[i].getEntry({0, 0}) > 0.5) {  // 1 represents 7
            seven_indices.push_back(i);
        } else {  // 0 represents non-7
            non_seven_indices.push_back(i);
        }
    }
}

// Function to get a balanced batch of indices
vector<size_t> getBalancedBatchIndices(const vector<size_t>& seven_indices, 
                                     const vector<size_t>& non_seven_indices, 
                                     int batch_size,
                                     std::mt19937& rng) {
    vector<size_t> batch_indices;
    size_t samples_per_class = static_cast<size_t>(batch_size / 2);
    
    // Create temporary vectors for shuffling
    vector<size_t> temp_sevens = seven_indices;
    vector<size_t> temp_non_sevens = non_seven_indices;
    
    // Shuffle both vectors
    std::shuffle(temp_sevens.begin(), temp_sevens.end(), rng);
    std::shuffle(temp_non_sevens.begin(), temp_non_sevens.end(), rng);
    
    // Take first samples_per_class elements from each
    for(size_t i = 0; i < samples_per_class && i < temp_sevens.size(); i++) {
        batch_indices.push_back(temp_sevens[i]);
    }
    for(size_t i = 0; i < samples_per_class && i < temp_non_sevens.size(); i++) {
        batch_indices.push_back(temp_non_sevens[i]);
    }
    
    // Shuffle the final batch indices
    std::shuffle(batch_indices.begin(), batch_indices.end(), rng);
    
    return batch_indices;
}

int main() {
    cout << "Starting MNIST 7 vs non-7 training program..." << endl;
    
    // Load MNIST data
    vector<Matrix> train_x, train_y;
    vector<Matrix> test_x, test_y;
    
    // Read training data
    cout << "Reading training data... (This may take a few seconds)" << endl;
    readMNISTData("data/mnist_binary_train.csv", train_x, train_y);
    cout << "Loaded " << train_x.size() << " training examples" << endl;
    
    // Read test data
    cout << "Reading test data... (This may take a few seconds)" << endl;
    readMNISTData("data/mnist_binary_test.csv", test_x, test_y);
    cout << "Loaded " << test_x.size() << " test examples" << endl;
    
    // Split dataset indices
    cout << "Splitting dataset indices... (Fast operation)" << endl;
    vector<size_t> seven_indices, non_seven_indices;
    splitDatasetIndices(train_y, seven_indices, non_seven_indices);
    cout << "Dataset split: " << seven_indices.size() << " sevens, " 
         << non_seven_indices.size() << " non-sevens" << endl;
    
    // Initialize random number generator
    cout << "Initializing random number generator... (Fast operation)" << endl;
    std::random_device rd;
    std::mt19937 rng(rd());
    
    // Model architecture
    cout << "Creating model architecture... (Fast operation)" << endl;
    Model model({1, 784}, 
                {
                    new FlattenLayer(),
                    new DenseLayer(512),  // First layer: 784 -> 512
                    new ActivationLayer("ReLU"),
                    new DenseLayer(256),  // Second layer: 512 -> 256
                    new ActivationLayer("ReLU"),
                    new DenseLayer(128),  // Third layer: 256 -> 128
                    new ActivationLayer("ReLU"),
                    new DenseLayer(1),    // Final layer: 128 -> 1
                    new ActivationLayer("sigmoid")
                },
                new LossFunction("BCE"),
                new Optimizer("SGD", {0.01, 0.9})  // SGD with learning rate 0.01 and momentum 0.9
                );
    cout << "Model created successfully" << endl;
    
    // Training parameters
    int batch_size = 64;  // Increased from 32 to match PyTorch
    int epochs = 30;
    
    // Training loop
    cout << "Starting training loop... (This will take several minutes)" << endl;
    cout << "Each epoch processes " << train_x.size() << " examples in batches of " << batch_size << endl;
    cout << "Total batches per epoch: " << std::max(1, static_cast<int>(train_x.size() / batch_size)) << endl;
    
    for(int epoch = 0; epoch < epochs; epoch++) {
        cout << "\nEpoch " << epoch + 1 << "/" << epochs << endl;
        double epoch_loss = 0.0;
        int correct_predictions = 0;
        int total_samples = 0;
        
        // Get multiple balanced batches per epoch
        int batches_per_epoch = std::max(1, static_cast<int>(train_x.size() / batch_size));
        cout << "Number of batches per epoch: " << batches_per_epoch << endl;
        
        for(int batch = 0; batch < batches_per_epoch; batch++) {
            cout << "Processing batch " << batch + 1 << "/" << batches_per_epoch << endl;
            
            cout << "Getting balanced batch indices..." << endl;
            vector<size_t> batch_indices = getBalancedBatchIndices(seven_indices, non_seven_indices, batch_size, rng);
            cout << "Balanced batch indices obtained. Batch size: " << batch_indices.size() << endl;
            
            for(size_t i = 0; i < batch_indices.size(); i++) {
                size_t idx = batch_indices[i];
                Matrix& x = train_x[idx];
                Matrix& y = train_y[idx];
                
                cout << "Training on sample " << i + 1 << "/" << batch_indices.size() << endl;
                // Forward pass (Fast operation)
                double loss = model.train(x, y);
                epoch_loss += loss;
                cout << "Sample loss: " << loss << endl;
                
                cout << "Calculating accuracy..." << endl;
                // Calculate accuracy (Fast operation)
                Matrix prediction = model.predict(x);
                if((prediction.getEntry({0, 0}) > 0.5) == (y.getEntry({0, 0}) > 0.5)) {
                    correct_predictions++;
                }
                total_samples++;
                cout << "Current accuracy: " << static_cast<double>(correct_predictions) / total_samples << endl;
            }
            
            cout << "Batch " << batch + 1 << " completed." << endl;
        }
        
        // Print epoch statistics
        double avg_loss = epoch_loss / (batches_per_epoch * batch_size);
        double accuracy = static_cast<double>(correct_predictions) / total_samples;
        cout << "Epoch " << epoch + 1 << "/" << epochs 
             << " - Loss: " << avg_loss 
             << " - Accuracy: " << accuracy << endl;
    }
    
    cout << "Training completed!" << endl;
    return 0;
} 