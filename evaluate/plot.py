import numpy as np
import matplotlib.pyplot as plt
from classifier_pb2 import TrainingDataset, SupportVertices, Experts, VerticesToLabel, LabeledVertices

def plot_vertices(ax, vertices, title, type, edge_colors=None):
    """Plots the vertices in the specified subplot ax with edge colors based on correctness."""
    features = []
    labels = []
    for entry in vertices.entries:
        features.append(entry.features)
        if entry.cluster_id.HasField('cluster_id_int'):
            labels.append(entry.cluster_id.cluster_id_int)
        else:
            labels.append(entry.cluster_id.cluster_id_str)
    
    features = np.array(features)
    if features.shape[1] != 2:
        raise ValueError("Features must be 2-dimensional for plotting.")
    
    markers = {'dataset': 'o', 'labeled': 'o'}
    if edge_colors is None:
        edge_colors = 'k'
    ax.scatter(features[:, 0], features[:, 1], c=labels, edgecolors=edge_colors, marker=markers[type])
    ax.set_title(title)

def plot_classification_results(dataset, labeled_chip, labeled_rchip, labeled_nn, dataset_name):
    """Creates a figure with six quadrants displaying dataset and classification results."""
    fig, axs = plt.subplots(2, 3, figsize=(15, 10))
    
    # Plot original dataset in quadrant 0
    plot_vertices(axs[0, 0], dataset, f"Dataset: {dataset_name}", 'dataset')
    
    # Reserve quadrants 1 and 2 for tables
    axs[0, 1].axis('off')
    axs[0, 2].axis('off')
    axs[0, 1].set_title("Training")
    axs[0, 2].set_title("Labeling")
    
    # Plot classified results with correctness
    for ax, (labeled_data, correctness) in zip(axs[1, :], [labeled_chip, labeled_rchip, labeled_nn]):
        edge_colors = ['g' if correct else 'r' for correct in correctness]
        plot_vertices(ax, labeled_data, ax.get_title(), 'labeled', edge_colors=edge_colors)
    
    # Set titles for classified results
    axs[1, 0].set_title("CHIP")
    axs[1, 1].set_title("RCHIP")
    axs[1, 2].set_title("NN")
    
    plt.tight_layout()
    plt.show()
