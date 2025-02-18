import argparse
import pathlib
import subprocess
import numpy as np
import matplotlib.pyplot as plt
from typing import Dict, List, Any

NUM_DIMENSIONS = 2
NUM_CLASSES = 2

def generate_synthetic_data(num_vertices: int, spread_factor: float, output_file: str, random_seed: int = None) -> np.ndarray:
  """
  Generate synthetic 2D data with a specified number of vertices and spread factor.
  Data is saved to a CSV file where each row contains the feature values and a class label.
  
  Args:
    num_vertices: Total number of vertices to generate.
    spread_factor: Standard deviation for the clusters.
    output_file: File path to save the generated data.
    random_seed: Optional seed for reproducibility.
    
  Returns:
    A numpy array of the generated data.
  """
  if random_seed is not None:
    np.random.seed(random_seed)
  
  base = num_vertices // NUM_CLASSES
  counts = [base] * NUM_CLASSES
  for i in range(num_vertices - base * NUM_CLASSES):
    counts[i] += 1

  data_rows = []

  # Use an explicit mapping for class labels.
  class_labels = [-1, 1]
  
  for label, count in zip(class_labels, counts):
    center = np.random.uniform(0, 10, NUM_DIMENSIONS)
    points = np.random.normal(loc=center, scale=spread_factor, size=(count, NUM_DIMENSIONS))
    for point in points:
      data_rows.append(list(point) + [label])
  
  data_array = np.array(data_rows)
  pathlib.Path(output_file).parent.mkdir(parents=True, exist_ok=True)
  fmt = ("%.6f," * NUM_DIMENSIONS + "%d")
  np.savetxt(output_file, data_array, delimiter=",", fmt=fmt)
  print(f"Synthetic data saved to {output_file}")
  return data_array

def plot_synthetic_data(ax: Any, data: np.ndarray, title: str = "Synthetic Data") -> None:
  """
  Plot synthetic data on the given matplotlib axis.
  
  Args:
    ax: Matplotlib axis to plot on.
    data: Data array containing features and class labels.
    title: Plot title.
  """
  unique_classes = np.unique(data[:, -1])
  cmap = plt.get_cmap("viridis", len(unique_classes))
  for label in unique_classes:
    mask = data[:, -1] == label
    # Ensure the colormap index is an integer
    color_index = int(label) if isinstance(label, (int, np.integer, float)) else 0
    ax.scatter(data[mask, 0], data[mask, 1],
           label=f"Class {int(label)}", color=cmap(color_index))
  ax.set_xlabel("Feature 1")
  ax.set_ylabel("Feature 2")
  ax.set_title(title)
  ax.legend()

def parse_graph_file(filename: str) -> Dict[int, Dict[str, Any]]:
  """
  Parse a graph file where each non-empty line has the format:
    vertex_id | features | cluster | adjacent
  The adjacent field should be a comma-separated list of "id-flag" pairs.
  
  Args:
    filename: Path to the graph file.
  
  Returns:
    A dictionary mapping vertex id to its data (coordinates, cluster, adjacent vertices).
  """
  graph: Dict[int, Dict[str, Any]] = {}
  with open(filename, "r") as f:
    for line in f:
      line = line.strip()
      if not line:
        continue
      parts = line.split("|")
      if len(parts) < 4:
        continue
      try:
        v_id = int(parts[0].replace(",", "").strip())
        features = [float(x.strip()) for x in parts[1].split(",") if x.strip()]
        cluster = float(parts[2].replace(",", "").strip())
        adjacent = []
        for pair in parts[3].split(','):
          if pair.strip():
            adj_parts = pair.split('-')
            if len(adj_parts) != 2:
              continue
            adj_id = int(adj_parts[0].strip())
            is_se = adj_parts[1].strip() == "1"
            adjacent.append((adj_id, is_se))
        graph[v_id] = {"coords": np.array(features), "cluster": cluster, "adjacent": adjacent}
      except Exception as e:
        print(f"Error parsing line: {line}. Error: {e}")
        continue
  return graph

def plot_graph(ax: Any, graph: Dict[int, Dict[str, Any]], title: str = "Graph") -> None:
  """
  Plot a graph on the provided axis. Each vertex is plotted as a scatter point, and edges
  are drawn between adjacent vertices.
  
  Args:
    ax: Matplotlib axis to plot on.
    graph: Dictionary representing the graph.
    title: Plot title.
  """
  clusters = {node["cluster"] for node in graph.values()}
  cmap = plt.get_cmap("viridis", len(clusters))
  coords = {v_id: data["coords"] for v_id, data in graph.items()}

  for v_id, data in graph.items():
    cluster_int = int(data["cluster"]) if isinstance(data["cluster"], (int, np.integer, float)) else 0
    ax.scatter(data["coords"][0], data["coords"][1],
           color=cmap(cluster_int), s=50)
  
  drawn = set()
  for v_id, data in graph.items():
    for adj, is_se in data.get("adjacent", []):
      if adj not in coords:
        continue
      if (adj, v_id) in drawn:
        continue
      p1 = coords[v_id]
      p2 = coords[adj]
      line_color = "r-" if is_se else "k-"
      ax.plot([p1[0], p2[0]], [p1[1], p2[1]], line_color, lw=1)
      drawn.add((v_id, adj))
  ax.set_xlabel("Feature 1")
  ax.set_ylabel("Feature 2")
  ax.set_title(title)

def run_command(cmd_input_file: List[str], cwd: str) -> None:
  """
  Run a subprocess command in the specified working directory.
  
  Args:
    cmd_input_file: List of command arguments.
    cwd: Working directory in which to execute the command.
  
  Raises:
    subprocess.CalledProcessError if the command fails.
  """
  print("Running command:", " ".join(cmd_input_file))
  try:
    subprocess.run(cmd_input_file, check=True, cwd=cwd)
    print("Command finished successfully.")
  except subprocess.CalledProcessError as e:
    print(f"Command failed: {e}")
    raise

def main():
  parser = argparse.ArgumentParser(
    description="Generate synthetic data, run Gabriel Graph and Low Degree Filter, and plot results."
  )
  parser.add_argument("--num_vertices", type=int, help="Total number of vertices", default=100)
  parser.add_argument("--spread", type=float, help="Spread factor for the clusters", default=0.5)
  parser.add_argument("--seed", type=int, help="Random seed for reproducibility", default=None)
  parser.add_argument("--threshold_factor", type=float, help="Threshold factor for the filter", default=1.0)
  args = parser.parse_args()
  
  synthetic_data_file_from_root = "data/synthetic/synthetic.csv"
  gabriel_graph_exe_from_root = "dev/GabrielGraph/gabrielGraph"
  gabriel_output_from_root = "dev/GabrielGraph/output/synthetic.csv"
  filter_exe_from_root = "dev/Filter/filter"
  filter_output_from_root = "dev/Filter/output/synthetic-filtered.csv"
  gabriel_filtered_output_from_root = "dev/GabrielGraph/output/synthetic-filtered.csv"
  path_from_gabriel_to_root = "../../"
  path_from_filter_to_root = "../../"
  gabriel_cwd = "dev/GabrielGraph"
  filter_cwd = "dev/Filter"
  gabriel_exe = "./gabrielGraph"
  filter_exe = "./filter"
  gabriel_input_file = path_from_gabriel_to_root + synthetic_data_file_from_root
  filter_input_file = path_from_filter_to_root + gabriel_output_from_root
  gabriel_output = gabriel_output_from_root
  filter_output = filter_output_from_root
  gabriel_over_filtered_input_file = path_from_gabriel_to_root + filter_output_from_root
  gabriel_filtered_output = gabriel_filtered_output_from_root
  
  # Generate synthetic data
  data = generate_synthetic_data(args.num_vertices, args.spread, synthetic_data_file_from_root, args.seed)

  fig, axes = plt.subplots(1, 2, figsize=(12, 6))
  plot_synthetic_data(axes[0], data, title="Synthetic Data")
  
  # Run Gabriel Graph executable
  run_command([gabriel_exe, gabriel_input_file], gabriel_cwd)
  
  # Run Low Degree Filter executable
  run_command([filter_exe, filter_input_file, str(args.threshold_factor)], filter_cwd)

  # Compute new Gabriel Graph on the filtered data
  run_command([gabriel_exe, gabriel_over_filtered_input_file], gabriel_cwd)
  
  if not pathlib.Path(gabriel_filtered_output).exists():
    print(f"Error: Expected Low Degree Filter output not found at {gabriel_filtered_output}")
  else:
    filtered_graph = parse_graph_file(gabriel_filtered_output)
    # Update the second subplot with the filtered graph.
    plot_graph(axes[1], filtered_graph, title="Filtered Graph")
  
  plt.tight_layout()
  plt.show()

if __name__ == "__main__":
  main()
