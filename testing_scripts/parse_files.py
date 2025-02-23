import numpy as np
from typing import Dict, Any

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

def parse_hyperplanes_file(filename: str, num_dimensions: int) -> np.ndarray:
    """
    Parse the hyperplanes output file and extract the midpoint coordinates for each support edge.
    
    Expected format per line:
      id, |, v0, v1, |, diff_coord0, diff_coord1, ..., |, midpoint_coord0, midpoint_coord1, ..., |, bias
    
    Args:
      filename: Path to the hyperplanes output file.
      
    Returns:
      A numpy array of midpoints (each row is a midpoint coordinate).
    """
    midpoints = []
    with open(filename, "r") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            parts = line.split("|")
            if len(parts) < 4:
                continue
            # The midpoint coordinates are in the fourth part (index 3)
            midpoint_str = parts[3].strip()
            # Split by comma and filter out empty strings
            midpoint_vals = [float(x.strip()) for x in midpoint_str.split(",") if x.strip()]
            if len(midpoint_vals) == num_dimensions:
                midpoints.append(midpoint_vals)
    if midpoints:
        midpoints = np.array(midpoints)
    else:
        midpoints = np.empty((0, num_dimensions))
    return midpoints

def parse_clas_file(filename: str) -> Dict[int, int]:
    """
    Parse the chip-clas output file which has lines of the format:
      id, class
    
    Returns:
      A dictionary mapping the vertex id (int) to the classified class (int).
    """
    classifications = {}
    with open(filename, "r") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            try:
                parts = line.split(",")
                v_id = int(parts[0].strip())
                v_class = int(parts[1].strip())
                classifications[v_id] = v_class
            except Exception as e:
                print(f"Error parsing chip-clas line: {line}. Error: {e}")
    return classifications