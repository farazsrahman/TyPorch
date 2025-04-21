#include "./../../linalg/linalg.h"
#include "./../neural.h"
#include "./../functions.cpp"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <./gtest/gtest.h>

using std::cout;
using std::vector;
using std::string;

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
            result.pixels.push_back(double(std::stoi(token)/256));
        }
    }
    return result;
}

void readMNISTCSV(const string& filename, vector<MNISTImage>& images, int max_samples = 5000) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    string line; int n = 0;
    std::getline(file, line); // first line is column headers
    while (std::getline(file, line)) {
        if(n >= max_samples) break;
        images.push_back(getImageOfLine(line)); 
        n++;
    }
    file.close();
}

TEST(MNISTBinary, test7sClassifier) {
    // Load MNIST data
    string train_filename = "./data/mnist_binary_train.csv";
    vector<MNISTImage> train_images;
    readMNISTCSV(train_filename, train_images, 5000);

    // Convert to tensors
    vector<Matrix> train_x; train_x.reserve(train_images.size());
    vector<Matrix> train_y; train_y.reserve(train_images.size());

    for(int i = 0; i < train_images.size(); i++) {
        // creating tensors for images
        train_x.emplace_back(28, 28);
        train_x[i].setEntries(train_images[i].pixels);

        // Binary classification - just one output neuron
        train_y.emplace_back(1, 1); 
        train_y[i].setEntry({0, 0}, train_images[i].label);
    }

    // Create model
    double learningRate = 0.01;
    Model model({28, 28}, 
                {
                    new FlattenLayer(),
                    new DenseLayer(128),
                    new ActivationLayer("ReLU"),
                    new DenseLayer(1),
                    new ActivationLayer("sigmoid")
                },
                new LossFunction("MSE"),
                new Optimizer("SGD", {learningRate, true})
                );

    // Training parameters
    int batchSize = 50;
    int numEpochs = 10;
    double totalLoss = 0;
    int correct = 0;
    int total = 0;

    // Training loop
    for(int epoch = 0; epoch < numEpochs; epoch++) {
        totalLoss = 0;
        correct = 0;
        total = 0;

        cout << "\nEPOCH: " << epoch << "\n";
        for(int i = 0; i < train_images.size(); i++) {
            // Train on single example
            double loss = model.train(train_x[i], train_y[i]);
            totalLoss += loss;

            // Calculate accuracy
            Matrix prediction = model.predict(train_x[i]);
            bool predicted_seven = prediction.getEntry({0, 0}) > 0.5;
            bool is_seven = train_y[i].getEntry({0, 0}) > 0.5;
            if(predicted_seven == is_seven) correct++;
            total++;

            // Update weights every batch
            if((i + 1) % batchSize == 0) {
                model.update();
                cout << "Batch " << (i + 1)/batchSize << " - ";
                cout << "Loss: " << totalLoss/batchSize << " - ";
                cout << "Accuracy: " << (double)correct/total * 100 << "%\n";
                totalLoss = 0;
                correct = 0;
                total = 0;
            }
        }
    }

    // Test on a few examples
    cout << "\nTesting a few examples:" << endl;
    for(int i = 0; i < 5; i++) {
        Matrix prediction = model.predict(train_x[i]);
        cout << "Example " << i + 1 << ":" << endl;
        cout << "True label: " << (train_y[i].getEntry({0, 0}) > 0.5 ? "7" : "not 7") << endl;
        cout << "Predicted probability: " << prediction.getEntry({0, 0}) << endl;
        cout << "Predicted class: " << (prediction.getEntry({0, 0}) > 0.5 ? "7" : "not 7") << endl;
        cout << "-------------------" << endl;
    }
} 