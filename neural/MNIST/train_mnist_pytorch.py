import torch
import torch.nn as nn
import torch.optim as optim
import pandas as pd
import numpy as np
from torch.utils.data import Dataset, DataLoader
from sklearn.metrics import accuracy_score
import time

class MNISTDataset(Dataset):
    def __init__(self, csv_file):
        # Read the CSV file
        df = pd.read_csv(csv_file)
        
        # Extract features and labels
        self.features = df.iloc[:, 1:].values.astype(np.float32) / 255.0  # Normalize to [0,1]
        self.labels = df.iloc[:, 0].values.astype(np.float32)
        
    def __len__(self):
        return len(self.labels)
    
    def __getitem__(self, idx):
        return torch.FloatTensor(self.features[idx]), torch.FloatTensor([self.labels[idx]])

class MNISTModel(nn.Module):
    def __init__(self):
        super(MNISTModel, self).__init__()
        self.network = nn.Sequential(
            nn.Flatten(),
            nn.Linear(784, 512, bias=True),
            nn.ReLU(),
            nn.Linear(512, 256, bias=True),
            nn.ReLU(),
            nn.Linear(256, 128, bias=True),
            nn.ReLU(),
            nn.Linear(128, 1, bias=True),
            nn.Sigmoid()
        )
        
        # Initialize weights with Xavier/Glorot
        for m in self.modules():
            if isinstance(m, nn.Linear):
                nn.init.xavier_uniform_(m.weight)
                nn.init.zeros_(m.bias)
    
    def forward(self, x):
        return self.network(x)

def train_model(model, train_loader, criterion, optimizer, num_epochs):
    model.train()
    for epoch in range(num_epochs):
        epoch_loss = 0.0
        correct_predictions = 0
        total_samples = 0
        
        start_time = time.time()
        
        for batch_idx, (data, target) in enumerate(train_loader):
            # Zero the gradients
            optimizer.zero_grad()
            
            # Forward pass
            output = model(data)
            loss = criterion(output, target)
            
            # Backward pass and optimize
            loss.backward()
            optimizer.step()
            
            # Update statistics
            epoch_loss += loss.item() * data.size(0)  # Scale loss by batch size
            predictions = (output > 0.5).float()
            correct_predictions += (predictions == target).sum().item()
            total_samples += target.size(0)
            
            if batch_idx % 10 == 0:
                batch_accuracy = (predictions == target).float().mean().item()
                print(f"Batch {batch_idx + 1}/{len(train_loader)} - "
                      f"Loss: {loss.item():.6f} - "
                      f"Accuracy: {batch_accuracy:.4f}")
        
        # Compute epoch statistics
        epoch_time = time.time() - start_time
        avg_loss = epoch_loss / total_samples
        accuracy = correct_predictions / total_samples
        
        print(f"\nEpoch {epoch + 1}/{num_epochs} - "
              f"Loss: {avg_loss:.6f} - "
              f"Accuracy: {accuracy:.4f} - "
              f"Time: {epoch_time:.2f}s")

def main():
    # Set random seed for reproducibility
    torch.manual_seed(42)
    
    # Create datasets
    train_dataset = MNISTDataset("data/mnist_binary_train.csv")
    test_dataset = MNISTDataset("data/mnist_binary_test.csv")
    
    # Create data loaders
    train_loader = DataLoader(train_dataset, batch_size=64, shuffle=True)
    test_loader = DataLoader(test_dataset, batch_size=64, shuffle=False)
    
    # Initialize model, loss function, and optimizer
    model = MNISTModel()
    criterion = nn.BCELoss()
    optimizer = optim.SGD(model.parameters(), lr=0.01, momentum=0.9)  # Using SGD with momentum
    
    # Print model summary
    print("Model architecture:")
    print(model)
    print(f"\nNumber of trainable parameters: {sum(p.numel() for p in model.parameters() if p.requires_grad)}")
    print(f"\nTraining on {len(train_dataset)} examples")
    print(f"Testing on {len(test_dataset)} examples")
    
    # Train the model
    print("\nStarting training...")
    train_model(model, train_loader, criterion, optimizer, num_epochs=30)
    
    # Evaluate on test set
    model.eval()
    test_loss = 0
    correct = 0
    with torch.no_grad():
        for data, target in test_loader:
            output = model(data)
            test_loss += criterion(output, target).item()
            pred = (output > 0.5).float()
            correct += (pred == target).sum().item()
    
    test_loss /= len(test_loader)
    accuracy = correct / len(test_dataset)
    print(f"\nTest set: Average loss: {test_loss:.4f}, Accuracy: {accuracy:.4f}")

if __name__ == "__main__":
    main() 