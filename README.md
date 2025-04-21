# TyPorch

TyPorch is a C++ library for implementing and training neural networks, featuring two main components:
1. A linear algebra library (`linalg`) for tensor and matrix operations
2. A neural network library (`neural`) for building and training neural networks

## Installation

### Prerequisites
- C++ compiler with C++14 support
- CMake (version 3.13 or higher)
- Make
- Git LFS (for downloading training data)

### Git LFS Setup
This repository uses Git Large File Storage (LFS) for managing training datasets. To properly clone and use the repository:

1. Install Git LFS:
```bash
# On macOS
brew install git-lfs

# On Ubuntu/Debian
sudo apt-get install git-lfs

# On Windows (using Chocolatey)
choco install git-lfs
```

2. Enable Git LFS:
```bash
git lfs install
```

3. Clone the repository:
```bash
git clone https://github.com/yourusername/TyPorch.git
cd TyPorch
```

4. Pull LFS files (if they weren't downloaded during clone):
```bash
git lfs pull
```

> **Note**: If you clone the repository without Git LFS installed, you'll still get the code, but the data files will be pointer files instead of actual content. If you try to run the MNIST examples without proper LFS setup, you'll get errors when the code tries to read the data files.

### Alternative Data Setup
If you don't want to use Git LFS, you can manually download the required datasets:
1. Download the MNIST dataset from [source URL]
2. Place the following files in the `data/` directory:
   - `mnist_binary_train.csv`
   - `mnist_binary_test.csv`
   - `mnist_train.csv`
   - `mnist_test.csv`

### Building the Libraries

1. Install CMake (if not already installed):
```bash
# On macOS
brew install cmake

# On Ubuntu/Debian
sudo apt-get install cmake

# On Windows (using Chocolatey)
choco install cmake
```

2. Build the libraries:
```bash
# Build linalg library
cd linalg
mkdir build && cd build
cmake ..
make

# Build neural library
cd ../../neural
mkdir build && cd build
cmake ..
make
```

## Features

### Linear Algebra Library (`linalg`)
- **Tensor Operations**
  - Multi-dimensional tensor support
  - Element-wise operations (addition, subtraction, multiplication, division)
  - Tensor reshaping and flattening
  - Random initialization
  - Coordinate-based indexing
  - Shape manipulation

- **Matrix Operations**
  - Matrix multiplication
  - Transposition
  - Xavier initialization (simple and normal)
  - File I/O for matrices
  - Column/row vector operations
  - Argmax operations

### Neural Network Library (`neural`)
- **Layers**
  - Input Layer
  - Dense Layer (fully connected)
  - Flatten Layer
  - Activation Layer (ReLU, sigmoid, tanh)
  - Output Layer

- **Optimizers**
  - Stochastic Gradient Descent (SGD)
  - Support for average gradient accumulation

- **Loss Functions**
  - Mean Squared Error (MSE)
  - Sum of Squared Error (SSE)
  - Cross Entropy (CE)

- **Model Features**
  - Sequential model architecture
  - Batch training
  - Forward and backward propagation
  - Weight updates
  - Model inspection and debugging

## Testing

The project includes comprehensive tests for both libraries. To run the tests:

1. Build and run linalg tests:
```bash
cd linalg/test
mkdir build && cd build
cmake ..
make
./linalgtest
```

2. Build and run neural tests:
```bash
cd ../../neural/test
mkdir build && cd build
cmake ..
make
./neuraltest
```

## Usage Example

Here's a simple example of creating and training a neural network:

```cpp
#include "linalg/linalg.h"
#include "neural/neural.h"

int main() {
    // Create a model with input size 30, output size 4
    int inputSize = 30;
    int outputSize = 4;
    
    // Create model with layers
    Model model({inputSize, 1}, 
                {
                    new FlattenLayer(), 
                    new DenseLayer(outputSize), 
                    new ActivationLayer("ReLU")
                }, 
                new LossFunction("MSE"),
                new Optimizer("SGD", {0.01, true})
                );
                
    // Training loop
    for(int batch = 0; batch < 100; batch++) {
        // Your training data here
        Matrix input(inputSize, 1);
        Matrix target(outputSize, 1);
        
        // Train on a single example
        double loss = model.train(input, target);
        model.update();
    }
    
    return 0;
}
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the LICENSE file for details.
