#!/usr/bin/env python3
import argparse
import os
import subprocess
import numpy as np
import matplotlib.pyplot as plt

from mpl_toolkits.mplot3d import Axes3D

def generate_synthetic_data(num_dimensions, num_classes, num_vertices, spread_factor, seed, output_file):
    """
    Generate synthetic data in num_dimensions with num_classes clusters.
    Each cluster is a Gaussian with a random center in [0,10]^d.
    The spread_factor is used as the standard deviation.
    Saves the data (features followed by class label) with no header.
    """
    np.random.seed(seed)
    # Determine number of vertices per class
    base = num_vertices // num_classes
    counts = [base] * num_classes
    # Add remainder to the first few clusters if needed
    for i in range(num_vertices - base * num_classes):
        counts[i] += 1

    data_rows = []
    for cls in range(num_classes):
        # Pick a random center in [0, 10]^d
        center = np.random.uniform(0, 10, num_dimensions)
        # Generate points from a Gaussian distribution
        points = np.random.normal(loc=center, scale=spread_factor, size=(counts[cls], num_dimensions))
        # Append each point with its class label
        for point in points:
            row = list(point) + [cls]
            data_rows.append(row)

    # Convert to numpy array for convenience
    data_array = np.array(data_rows)
    # Ensure the output directory exists
    out_dir = os.path.dirname(output_file)
    os.makedirs(out_dir, exist_ok=True)
    # Save to CSV (no header, comma-separated)
    np.savetxt(output_file, data_array, delimiter=",", fmt=("%.6f," * num_dimensions + "%d"))
    print(f"Synthetic data saved to {output_file}")
    return data_array

def plot_synthetic_data(ax, data, num_dimensions, title="Synthetic Data"):
    """
    Plot the synthetic data on the provided axis.
    Assumes the last column of data is the class label.
    """
    classes = np.unique(data[:, -1])
    cmap = plt.get_cmap("viridis", len(classes))
    
    if num_dimensions == 2:
        for cls in classes:
            mask = data[:, -1] == cls
            ax.scatter(data[mask, 0], data[mask, 1],
                       label=f"Class {int(cls)}", color=cmap(int(cls)))
        ax.set_xlabel("Feature 1")
        ax.set_ylabel("Feature 2")
    elif num_dimensions == 3:
        for cls in classes:
            mask = data[:, -1] == cls
            ax.scatter(data[mask, 0], data[mask, 1], data[mask, 2],
                       label=f"Class {int(cls)}", color=cmap(int(cls)))
        ax.set_xlabel("Feature 1")
        ax.set_ylabel("Feature 2")
        ax.set_zlabel("Feature 3")
    ax.set_title(title)
    ax.legend()

def parse_graph_file(filename, num_dimensions):
    """
    Parse a graph CSV file with the format:
      vertex_id, |, feature0, feature1, ..., featureN, |, cluster_id, |, adjacent_vertex0, adjacent_vertex1, ...
    Returns a dictionary mapping vertex_id -> {'coords': np.array, 'cluster': int, 'adjacent': list of ints}
    """
    graph = {}
    with open(filename, "r") as f:
        for line in f:
            # Remove any extra whitespace/newlines
            line = line.strip()
            if not line:
                continue
            # Split on the '|' separator
            parts = line.split("|")
            if len(parts) < 4:
                continue  # or raise an error if format is not as expected
            # Process each part: remove commas and extra spaces
            v_id = int(parts[0].replace(",", "").strip())
            features = [float(x.strip()) for x in parts[1].split(",") if x.strip() != ""]
            cluster = float(parts[2].replace(",", "").strip())
            adjacent = [
                (int(pair.split('-')[0].strip()), pair.split('-')[1].strip() == "1")
                for pair in parts[3].split(',')
                if pair.strip()  # skip empty strings
            ]
            graph[v_id] = {"coords": np.array(features), "cluster": cluster, "adjacent": adjacent}
    return graph

def plot_graph(ax, graph, num_dimensions, title="Graph"):
    """
    Plot the graph on the provided axis.
    Each vertex is drawn (colored by its cluster) and edges are drawn between vertices as specified.
    Only edges for which both vertices exist in the graph are drawn.
    """
    classes = {v["cluster"] for v in graph.values()}
    cmap = plt.get_cmap("viridis", len(classes))
    
    # Create a mapping vertex_id -> coordinates for easier lookup
    coords = {v_id: data["coords"] for v_id, data in graph.items()}
    if num_dimensions == 2:
        # Plot vertices
        for v_id, data in graph.items():
            ax.scatter(data["coords"][0], data["coords"][1],
                       color=cmap(data["cluster"]), s=50)
        # Draw edges without duplicates, only if both endpoints exist.
        drawn = set()
        for v_id, data in graph.items():
            for adj, isSE in data["adjacent"]:
                if adj not in coords:
                    continue
                if (adj, v_id) in drawn:
                    continue
                p1 = coords[v_id]
                p2 = coords[adj]
                ax.plot([p1[0], p2[0]], [p1[1], p2[1]], "r-" if isSE else "k-", lw=1)
                drawn.add((v_id, adj))
        ax.set_xlabel("Feature 1")
        ax.set_ylabel("Feature 2")
    elif num_dimensions == 3:
        # Plot vertices
        for v_id, data in graph.items():
            ax.scatter(data["coords"][0], data["coords"][1], data["coords"][2],
                       color=cmap(data["cluster"]), s=50)
        # Draw edges without duplicates, only if both endpoints exist.
        drawn = set()
        for v_id, data in graph.items():
            for adj, isSE in data["adjacent"]:
                if adj not in coords:
                    continue
                if (adj, v_id) in drawn:
                    continue
                p1 = coords[v_id]
                p2 = coords[adj]
                ax.plot([p1[0], p2[0]], [p1[1], p2[1]], [p1[2], p2[2]], "r-" if isSE else "k-", lw=1)
                drawn.add((v_id, adj))
        ax.set_xlabel("Feature 1")
        ax.set_ylabel("Feature 2")
        ax.set_zlabel("Feature 3")
    ax.set_title(title)

def run_command(cmd_args, cwd):
    """
    Runs an external command given as a list of arguments.
    Raises an exception if the command fails.
    """
    print("Running command:", " ".join(cmd_args))
    subprocess.run(cmd_args, check=True, cwd=cwd)
    print("Command finished successfully.")

def main():
    parser = argparse.ArgumentParser(description="Generate synthetic data, run Gabriel Graph and Low Degree Filter, and plot results.")
    parser.add_argument("--dimensions", type=int, choices=[2, 3], help="Number of dimensions (2 or 3)")
    parser.add_argument("--num_classes", type=int, help="Number of classes")
    parser.add_argument("--num_vertices", type=int, help="Total number of vertices", default=100, nargs="?")
    parser.add_argument("--spread_factor", type=float, help="Spread factor for the clusters", default=0.5, nargs="?")
    parser.add_argument("--deviation_factor", type=float, help="Deviation factor for the threshold", default=1, nargs="?")
    parser.add_argument("--seed", type=int, help="Random seed for data generation", default=None, nargs="?")
    parser.add_argument("--filter", dest="should_filter", action="store_true", help="Apply Low Degree Filter")
    parser.add_argument("--no-filter", dest="should_filter", action="store_false", help="Skip Low Degree Filter")
    parser.set_defaults(should_filter=True)
    args = parser.parse_args()
    
    # Define paths relative to the workspace root (current directory)
    synthetic_data_dir = os.path.join("data", "synthetic")
    programs_synthetic_data_dir = os.path.join("..", "..", "data", "synthetic")
    synthetic_data_file = os.path.join(synthetic_data_dir, "synthetic.csv")
    programs_synthetic_data_file = os.path.join(programs_synthetic_data_dir, "synthetic.csv")
    
    # 1. Generate synthetic data
    data = generate_synthetic_data(args.dimensions, args.num_classes, args.num_vertices, args.spread_factor, args.seed, synthetic_data_file)

    # Create one figure with 3 subplots side-by-side.
    if args.should_filter:
        if args.dimensions == 2:
            fig, axes = plt.subplots(1, 3, figsize=(18, 6))
        elif args.dimensions == 3:
            fig = plt.figure(figsize=(18, 6))
            ax1 = fig.add_subplot(131, projection="3d")
            ax2 = fig.add_subplot(132, projection="3d")
            ax3 = fig.add_subplot(133, projection="3d")
            axes = [ax1, ax2, ax3]
    else:
        if args.dimensions == 2:
            fig, axes = plt.subplots(1, 2, figsize=(12, 6))
        elif args.dimensions == 3:
            fig = plt.figure(figsize=(12, 6))
            ax1 = fig.add_subplot(121, projection="3d")
            ax2 = fig.add_subplot(122, projection="3d")
            axes = [ax1, ax2]

    # 2. Plot the synthetic data
    plot_synthetic_data(axes[0], data, args.dimensions, title="Synthetic Data")
    
    # 3. Run the Gabriel Graph program over the data
    gabriel_exe = os.path.join(".", "multiVariableGabrielGraph")
    gabriel_output = os.path.join("dev", "GabrielGraph", "output", "synthetic.csv")
    gabriel_cwd = os.path.join("dev", "GabrielGraph")
    run_command([gabriel_exe, programs_synthetic_data_file], gabriel_cwd)
    
    # 4. Parse and plot the constructed graph
    if not os.path.exists(gabriel_output):
        print(f"Error: Expected Gabriel Graph output not found at {gabriel_output}")
    else:
        graph = parse_graph_file(gabriel_output, args.dimensions)
        plot_graph(axes[1], graph, args.dimensions, title="Gabriel Graph")
    
    if args.should_filter:
        # 5. Run the Low Degree Vertices Filter over the graph
        lowdegree_exe = os.path.join(".", "lowDegreeVerticesFilter")
        gabriel_output = os.path.join("..", "GabrielGraph", "output", "synthetic.csv")
        lowdegree_output = os.path.join("dev", "LowDegreeVerticesFilter", "output", "synthetic-filtered.csv")
        lowdegree_cwd = os.path.join("dev", "LowDegreeVerticesFilter")
        run_command([lowdegree_exe, gabriel_output, str(args.deviation_factor)], lowdegree_cwd)
        
        # 6. Parse and plot the filtered graph
        if not os.path.exists(lowdegree_output):
            print(f"Error: Expected Low Degree Filter output not found at {lowdegree_output}")
        else:
            filtered_graph = parse_graph_file(lowdegree_output, args.dimensions)
            plot_graph(axes[2], filtered_graph, args.dimensions, title="Filtered Graph")
    
    plt.show()

if __name__ == "__main__":
    main()
