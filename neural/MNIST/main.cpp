#include "./../../linalg/linalg.h"
#include "./../neural.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>

#define NUM_SAMPLES 5000 // this should be the number of examples we want to load
                         // out of the 60000 total.

using std::cout;
using std::endl;
using std::vector; 
using std::string;


struct MNISTImage {
    double label;
    vector<double> pixels;
};

// forward declaring so that the main function isnt hidden under a bunch of helpers defs
MNISTImage getImageOfLine(const string& s); 
void readMNISTCSV(const string& filename, vector<MNISTImage>& images);
void printMNISTImage(const MNISTImage& image);

// MAIN
int main() {
    string filename = "./../data/mnist_train.csv"; // Replace with your CSV file's path

    vector<MNISTImage> images;
    readMNISTCSV(filename, images);

    cout << "converting data to tensor objects: ";

    vector<Matrix> train_x; train_x.reserve(NUM_SAMPLES);
    vector<Matrix> train_y; train_y.reserve(NUM_SAMPLES);

    for(int i = 0; i < images.size(); i++) {
        // creating tensors for images
        train_x.emplace_back(28, 28);
        train_x[i].setEntries(images[i].pixels);

        // setting up one hot encoding
        train_y.emplace_back(10, 1); 
        train_y[i].fill(0);
        train_y[i].setEntry({int(images[i].label), 0}, 1);
    }
    
    cout << "done"  << endl;
    

    cout << "building model:" << endl;

    Model model({28, 28},  {new FlattenLayer(), 
                            new DenseLayer(200),
                            new ActivationLayer("sigmoid"),
                            new DenseLayer(10),
                            new ActivationLayer("softmax")},

                            new LossFunction("CE"), 
                            new Optimizer("GD", {.05, true}
                            
                            ));

    model.print();

    //prams
    int batchSize = 50;

    // train
    double loss, totalLoss; int iter = 0;
    for(int batch = 0; iter < NUM_SAMPLES; batch++) {
        totalLoss = 0;

        // train on batch
        cout << "\nBATCH: " << batch << "\n";
        for(int j = 0; j < batchSize && iter < NUM_SAMPLES; j++) {

            loss = model.train(train_x[iter % NUM_SAMPLES], 
                               train_y[iter % NUM_SAMPLES]);            
            totalLoss += loss;
            iter++;

        }

        cout << "AVG LOSS: " << totalLoss/batchSize << "\n";

        model.update();
    }


}


// HELPERS FOR DATA EXTRACTION

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
void readMNISTCSV(const string& filename, vector<MNISTImage>& images) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    string line; int n = 0;

    std::getline(file, line); // first line is column headers
    while (std::getline(file, line)) {

        if(n >= NUM_SAMPLES) break;

        images.push_back(getImageOfLine(line)); n++;
        cout << "loaded image " << n << " / 60000" << endl;
        // Move the cursor up one line
        cout << "\033[F";
    }

    cout << endl;

    file.close();
}
void printMNISTImage(const MNISTImage& image) {
    cout << "Label: " << image.label << endl;
    for (size_t i = 0; i < image.pixels.size(); ++i) {
        
        if (image.pixels[i] > .3) cout << "#";
        else cout << " ";

        if ((i + 1) % 28 == 0) {
            cout << endl; // Start a new line after every 28 pixels
        }
    }
    cout << endl;
}

