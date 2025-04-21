import numpy as np
import pandas as pd
import os
from pathlib import Path

def prepare_nine_vs_zero(input_file, output_file):
    """Prepare data for 9 vs 0 classification."""
    print(f"Processing {input_file} for 9 vs 0 classification...")
    
    # Read the MNIST data
    df = pd.read_csv(input_file)
    
    # Extract labels and features
    labels = df.iloc[:, 0].values
    features = df.iloc[:, 1:].values
    
    # Filter for only 9s and 0s
    mask = (labels == 9) | (labels == 0)
    filtered_features = features[mask]
    filtered_labels = labels[mask]
    
    # Convert to binary classification (9=1, 0=0)
    binary_labels = (filtered_labels == 9).astype(int)
    
    # Normalize pixel values to [0, 1]
    filtered_features = filtered_features / 255.0
    
    # Create output DataFrame
    output_df = pd.DataFrame(filtered_features)
    output_df.insert(0, 'label', binary_labels)
    
    # Save to CSV
    output_df.to_csv(output_file, index=False)
    
    # Print statistics
    total_examples = len(binary_labels)
    nines_count = np.sum(binary_labels)
    zeros_count = total_examples - nines_count
    
    print(f"Created {output_file}")
    print(f"Total examples: {total_examples}")
    print(f"9s count: {nines_count}")
    print(f"0s count: {zeros_count}")
    print()

def main():
    # Create data directory if it doesn't exist
    data_dir = Path("data")
    data_dir.mkdir(exist_ok=True)
    
    # Check for training data
    train_file = Path("data/archive/mnist_train.csv")
    if train_file.exists():
        print("Processing training data...")
        prepare_nine_vs_zero(train_file, "data/mnist_9vs0_train.csv")
    else:
        print(f"Error: Training data file not found at {train_file}")
    
    # Check for test data
    test_file = Path("data/archive/mnist_test.csv")
    if test_file.exists():
        print("Processing test data...")
        prepare_nine_vs_zero(test_file, "data/mnist_9vs0_test.csv")
    else:
        print(f"Error: Test data file not found at {test_file}")

if __name__ == "__main__":
    main() 