import numpy as np
import matplotlib.pyplot as plt
from typing import Any, Dict, List

def plot_synthetic_data(ax: Any, data: np.ndarray, title: str = "Synthetic Data") -> None:
    """
    Plot synthetic training data on the given matplotlib axis.
    
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

def plot_graph(ax: Any, graph: Dict[int, Dict[str, Any]], title: str = "Graph", draw_edges=True) -> None:
    """
    Plot a graph on the provided axis. Each vertex is plotted as a scatter point, and edges
    are drawn between adjacent vertices if `draw_edges` is `True`
    
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
    if draw_edges:
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

def compute_nearest_neighbor_path(points: np.ndarray) -> List[int]:
    """
    Given an array of points (n x NUM_DIMENSIONS), compute a nearest neighbor tour starting 
    from the first point, then refine it using a two-opt algorithm.
    Returns a list of indices representing the order.
    """
    n = len(points)
    if n == 0:
        return []
    
    # Greedy nearest neighbor route
    visited = set([0])
    route = [0]
    current = 0
    while len(visited) < n:
        dists = np.linalg.norm(points - points[current], axis=1)
        for idx in visited:
            dists[idx] = np.inf
        next_idx = int(np.argmin(dists))
        route.append(next_idx)
        visited.add(next_idx)
        current = next_idx

    # Refine the route using 2-opt improvement
    route = two_opt(route, points)
    return route

def route_distance(route: List[int], points: np.ndarray) -> float:
    """Compute total distance of the tour given a route (list of indices)."""
    distance = 0.0
    for i in range(len(route) - 1):
        distance += np.linalg.norm(points[route[i]] - points[route[i+1]])
    return distance

def two_opt(route: List[int], points: np.ndarray) -> List[int]:
    """
    Apply the two-opt algorithm to improve the tour by swapping segments.
    """
    best = route
    improved = True
    while improved:
        improved = False
        for i in range(1, len(best) - 2):
            for j in range(i + 1, len(best)):
                if j - i == 1:  # skip adjacent indices
                    continue
                new_route = best[:i] + best[i:j][::-1] + best[j:]
                if route_distance(new_route, points) < route_distance(best, points):
                    best = new_route
                    improved = True
        route = best
    return best

# --- Extending the polyline to the plot boundaries ---
def extend_point_to_bounds(point: np.ndarray, direction: np.ndarray, xlim: tuple, ylim: tuple) -> np.ndarray:
    """
    Extend a point along a given direction until it hits the plot boundaries.
    """
    if np.allclose(direction, 0):
        return point.copy()
    
    t_candidates = []
    # Calculate t for vertical boundaries (x-direction)
    if direction[0] != 0:
        if direction[0] > 0:
            t_x = (xlim[1] - point[0]) / direction[0]
        else:
            t_x = (xlim[0] - point[0]) / direction[0]
        t_candidates.append(t_x)
    
    # Calculate t for horizontal boundaries (y-direction)
    if direction[1] != 0:
        if direction[1] > 0:
            t_y = (ylim[1] - point[1]) / direction[1]
        else:
            t_y = (ylim[0] - point[1]) / direction[1]
        t_candidates.append(t_y)
    
    # Choose the smallest positive t
    t = min(t for t in t_candidates if t > 0)
    return point + t * direction

def extend_polyline_to_bounds(polyline: np.ndarray, ax) -> np.ndarray:
    """
    Given a polyline (n x 2), extend its start and end in the directions
    of the first and last segments so that the line touches the plot boundaries.
    """
    xlim = ax.get_xlim()
    ylim = ax.get_ylim()
    
    # Extend start
    start = polyline[0]
    second = polyline[1]
    start_direction = start - second  # reverse direction for extension
    extended_start = extend_point_to_bounds(start, start_direction, xlim, ylim)
    
    # Extend end
    end = polyline[-1]
    penultimate = polyline[-2]
    end_direction = end - penultimate  # forward direction
    extended_end = extend_point_to_bounds(end, end_direction, xlim, ylim)
    
    # Combine the extended endpoints with the original polyline
    extended_polyline = np.vstack([extended_start, polyline, extended_end])
    return extended_polyline

def plot_labeled(ax: Any, to_label_data: np.ndarray, classifications: Dict[int, int], num_to_label: int, title: str) -> None:
    """
    Plot the labeled to-label vertices on the given axis.
    """

    to_label_classes = np.array([classifications.get(i, 0) for i in range(-num_to_label, 0)])

    unique_classes = np.unique(to_label_classes)
    cmap = plt.get_cmap("plasma", len(unique_classes))
    for idx, cl in enumerate(unique_classes):
        mask = to_label_classes == cl
        ax.scatter(
            to_label_data[mask, 0],
            to_label_data[mask, 1],
            marker="D", edgecolor="k", facecolor=cmap(idx),
            s=80, label=cl
        )
    
    ax.legend()
    ax.set_title(title)