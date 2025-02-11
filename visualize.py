#!/usr/bin/env python3
"""
This script generates synthetic data and then runs two external programs:
  - multiVariableGabrielGraph
  - lowDegreeVerticesFilter
It then plots:
  (1) the synthetic data,
  (2) the graph constructed by the Gabriel Graph program,
  (3) the graph filtered by the Low Degree Vertices Filter.
  
Usage:
    python3 visualize.py <dimensions:2_or_3> <num_classes> <num_vertices>

Example:
    python3 visualize.py 2 3 100
"""

# TODO: allow to change the relative position between clusters

import os
import sys
import argparse
import numpy as np
import matplotlib.pyplot as plt
import subprocess

def generate_synthetic_data(dimensions, num_classes, num_vertices, spread=0.5):
    """
    Generate synthetic data by creating one Gaussian cluster per class.
    Each vertex is a point in the given dimensions.
    Returns:
       features: a (num_vertices x dimensions) numpy array.
       targets: a (num_vertices,) numpy array of integer class labels.
    """
    features_list = []
    targets_list = []
    # Divide vertices as evenly as possible among the classes.
    base = num_vertices // num_classes
    rem = num_vertices % num_classes
    for c in range(num_classes):
        n_points = base + (1 if c < rem else 0)
        # Choose a random center for this class (within 0 to 10 in each dimension)
        center = np.random.uniform(0, 10, size=(dimensions,))
        # Generate points with some spread
        points = center + np.random.randn(n_points, dimensions) * spread
        features_list.append(points)
        targets_list.extend([c] * n_points)
    features = np.vstack(features_list)
    targets = np.array(targets_list)
    return features, targets

def save_csv(data, filepath, fmt="%.4f"):
    """Save a numpy array to CSV (no header)."""
    np.savetxt(filepath, data, delimiter=",", fmt=fmt)

def load_csv(filepath, dtype=float):
    """Load CSV file (without header) into a numpy array."""
    return np.loadtxt(filepath, delimiter=",", dtype=dtype)

def plot_data(ax, features, targets, num_classes, title, dimensions):
    """
    Plot data points (colored by class) on the given axes.
    For 2D, it creates a scatter plot; for 3D, a 3D scatter plot.
    """
    cmap = plt.get_cmap("viridis", num_classes)
    if dimensions == 2:
        for c in range(num_classes):
            idx = targets == c
            ax.scatter(features[idx, 0], features[idx, 1], color=cmap(c), label=f"Class {c}")
        ax.set_xlabel("X")
        ax.set_ylabel("Y")
    elif dimensions == 3:
        for c in range(num_classes):
            idx = targets == c
            ax.scatter(features[idx, 0], features[idx, 1], features[idx, 2],
                       color=cmap(c), label=f"Class {c}")
        ax.set_xlabel("X")
        ax.set_ylabel("Y")
        ax.set_zlabel("Z")
    ax.set_title(title)
    ax.legend()

def plot_graph(ax, features, edges, num_classes, targets, title, dimensions):
    """
    Plot the graph by first plotting the data points (using plot_data)
    and then drawing a line for each edge (each row in edges contains two vertex indices).
    """
    # Plot the data (points)
    plot_data(ax, features, targets, num_classes, title, dimensions)
    # Now draw the edges
    for edge in edges:
        i, j = int(edge[0]), int(edge[1])
        if dimensions == 2:
            x_coords = [features[i, 0], features[j, 0]]
            y_coords = [features[i, 1], features[j, 1]]
            ax.plot(x_coords, y_coords, 'k-', linewidth=0.8)
        elif dimensions == 3:
            x_coords = [features[i, 0], features[j, 0]]
            y_coords = [features[i, 1], features[j, 1]]
            z_coords = [features[i, 2], features[j, 2]]
            ax.plot(x_coords, y_coords, z_coords, 'k-', linewidth=0.8)

def main():
    # Parse command-line arguments.
    parser = argparse.ArgumentParser(
        description="Generate synthetic data, run Gabriel Graph and Low Degree Vertices Filter.")
    parser.add_argument("dimensions", type=int, choices=[2, 3],
                        help="Number of dimensions (2 or 3)")
    parser.add_argument("num_classes", type=int, help="Number of classes")
    parser.add_argument("num_vertices", type=int, help="Number of vertices")
    parser.add_argument("spread", type=float, nargs="?", default=0.5, help="Spread of data points")
    args = parser.parse_args()

    dimensions = args.dimensions
    num_classes = args.num_classes
    num_vertices = args.num_vertices
    spread = args.spread

    # Use 'synthetic' as the dataset name to preserve naming/structure.
    dataset_name = "synthetic"
    data_dir = os.path.join("data", dataset_name)
    os.makedirs(data_dir, exist_ok=True)

    # Generate synthetic features and targets.
    features, targets = generate_synthetic_data(dimensions, num_classes, num_vertices, spread)
    features_path = os.path.join(data_dir, f"{dataset_name}-features.csv")
    targets_path  = os.path.join(data_dir, f"{dataset_name}-targets.csv")

    # Save the synthetic data (no headers).
    save_csv(features, features_path, fmt="%.4f")
    save_csv(targets, targets_path, fmt="%d")

    print("=== Synthetic Data ===")
    print("Features:")
    print(features)
    print("Targets:")
    print(targets)

    # Prepare a figure with three subplots: (1) raw data, (2) constructed graph, (3) filtered graph.
    if dimensions == 2:
        fig, axes = plt.subplots(1, 3, figsize=(18, 6))
    else:  # For 3D we need to specify a 3D projection for each subplot.
        fig = plt.figure(figsize=(18, 6))
        axes = []
        axes.append(fig.add_subplot(131, projection='3d'))
        axes.append(fig.add_subplot(132, projection='3d'))
        axes.append(fig.add_subplot(133, projection='3d'))

    # Plot the original synthetic data.
    plot_data(axes[0], features, targets, num_classes, "Synthetic Data", dimensions)

    # === Run the Gabriel Graph program ===
    features_path_to_gabriel = "../../" + features_path
    gabriel_cmd = ["./multiVariableGabrielGraph", features_path_to_gabriel]
    print("\nRunning Gabriel Graph program...")
    try:
        subprocess.run(gabriel_cmd, check=True, cwd="golden/GabrielGraph")
    except subprocess.CalledProcessError as e:
        print("Error running Gabriel Graph program:", e)
        sys.exit(1)

    # Expect the Gabriel Graph program to create an edges file named <dataset>-edges.csv
    gabriel_output_dir = os.path.join("golden", "GabrielGraph", "output")
    gabriel_edges_file = os.path.join(gabriel_output_dir, f"{dataset_name}-edges.csv")
    if not os.path.exists(gabriel_edges_file):
        print("Gabriel Graph output file not found:", gabriel_edges_file)
        sys.exit(1)
    edges = load_csv(gabriel_edges_file)

    # Plot the constructed graph (data points + Gabriel Graph edges).
    plot_graph(axes[1], features, edges, num_classes, targets, "Constructed Gabriel Graph", dimensions)

    # === Run the Low Degree Vertices Filter program ===
    features_path_to_filter = "../../" + features_path
    targets_path_to_filter = "../../" + targets_path
    gabriel_edges_file_to_filter = "../../" + gabriel_edges_file
    low_degree_cmd = [
        "./lowDegreeVerticesFilter",
        features_path_to_filter, targets_path_to_filter, gabriel_edges_file_to_filter
    ]
    print("\nRunning Low Degree Vertices Filter program...")
    try:
        subprocess.run(low_degree_cmd, check=True, cwd="golden/LowDegreeVerticesFilter")
    except subprocess.CalledProcessError as e:
        print("Error running Low Degree Vertices Filter program:", e)
        sys.exit(1)

    # Expect the filter to produce three files in golden/LowDegreeVerticesFilter/output:
    filter_output_dir = os.path.join("golden", "LowDegreeVerticesFilter", "output")
    filtered_features_file = os.path.join(filter_output_dir, f"{dataset_name}-features_filtered.csv")
    filtered_targets_file  = os.path.join(filter_output_dir, f"{dataset_name}-targets_filtered.csv")
    filtered_edges_file    = os.path.join(filter_output_dir, f"{dataset_name}-edges_filtered.csv")
    if not (os.path.exists(filtered_features_file) and 
            os.path.exists(filtered_targets_file) and 
            os.path.exists(filtered_edges_file)):
        print("Filtered output files not found in", filter_output_dir)
        sys.exit(1)
    filtered_features = load_csv(filtered_features_file)
    filtered_targets  = load_csv(filtered_targets_file, dtype=int)
    filtered_edges    = load_csv(filtered_edges_file)

    # Plot the filtered graph.
    plot_graph(axes[2], filtered_features, filtered_edges, num_classes,
               filtered_targets, "Filtered Graph", dimensions)

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    main()
