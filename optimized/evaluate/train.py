import os
import pathlib
import argparse
import matplotlib.pyplot as plt
from classifier_pb2 import TrainingDataset, SupportVertices
from google.protobuf.message import DecodeError

def read_protobuf_file(filepath, message_type):
    message = message_type()
    try:
        with open(filepath, "rb") as f:
            message.ParseFromString(f.read())
        return message
    except FileNotFoundError:
        print(f"Error: File {filepath} not found.")
    except DecodeError:
        print(f"Error: Failed to decode the file {filepath}.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

def plot_data(dataset, support_vertices):
    fig, ax = plt.subplots()
    
    # Plot dataset vertices
    for entry in dataset.entries:
        features = entry.features
        ax.scatter(features[0], features[1], c='blue', label='Dataset Vertex', alpha=0.5, edgecolors='w', s=100)
    
    # Plot support vertices
    for entry in support_vertices.entries:
        features = entry.features
        ax.scatter(features[0], features[1], c='red', label='Support Vertex', alpha=0.5, edgecolors='k', s=100)
    
    plt.xlabel('Feature 1')
    plt.ylabel('Feature 2')
    plt.title('Dataset and Support Vertices')
    plt.legend()
    plt.show()

def main():
    parser = argparse.ArgumentParser(description="Train and plot data")
    parser.add_argument("--dataset", type=pathlib.Path, help="Path to the data file")
    parser.add_argument("--tolerance", type=float, help="Tolerance parameter")
    args = parser.parse_args()
    
    dataset = args.dataset
    tolerance = args.tolerance
    
    # Call the train function (assuming it is implemented in train.cpp and callable from Python)
    os.system(f"../nn/train/train {dataset} {tolerance}")
    
    # Read the output file
    output_file = pathlib.Path(f"../nn/train/trained/{dataset.name}")
    support_vertices = read_protobuf_file(output_file, SupportVertices)
    
    # Read the input dataset file
    dataset = read_protobuf_file(dataset, TrainingDataset)
    
    # Plot the data
    plot_data(dataset, support_vertices)

if __name__ == "__main__":
    main()