# TyPorch

TyPorch is a C++ library for implementing and training neural networks, featuring two main components:
1. A linear algebra library (`linalg`) for tensor and matrix operations
2. A neural network library (`neural`) for building and training neural networks

## Installation

### Prerequisites
- C++ compiler with C++14 support
- CMake (version 3.13 or higher)
- Make

### Installation Steps

1. Clone the repository:
```bash
git clone https://github.com/yourusername/TyPorch.git
cd TyPorch
```

2. Install CMake (if not already installed):
```bash
# On macOS
brew install cmake

# On Ubuntu/Debian
sudo apt-get install cmake

# On Windows (using Chocolatey)
choco install cmake
```

3. Build the libraries:
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
