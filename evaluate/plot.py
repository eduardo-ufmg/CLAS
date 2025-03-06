import numpy as np
import matplotlib.pyplot as plt
from classifier_pb2 import TrainingDataset, SupportVertices, Experts, VerticesToLabel, LabeledVertices

def plot_vertices(ax, vertices, title, type):
    """Plots the vertices in the specified subplot ax."""
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
    
    markers = {'dataset': 'o', 'SVs': ',', 'labeled': 'v'}
    ax.scatter(features[:, 0], features[:, 1], c=labels, edgecolors='k', marker=markers[type])
    ax.set_title(title)

def plot_classification_results(dataset, labeled_chip, labeled_rchip, labeled_nn, dataset_name):
    """Creates a figure with six quadrants displaying dataset and classification results."""
    fig, axs = plt.subplots(2, 3, figsize=(15, 10))
    
    # Plot original dataset in quadrant 0
    plot_vertices(axs[0, 0], dataset, f"Dataset: {dataset_name}", 'dataset')
    
    # Reserve quadrants 1 and 2 for time/memory tables (to be implemented later)
    axs[0, 1].axis('off')
    axs[0, 2].axis('off')
    axs[0, 1].set_title("Training Performance")
    axs[0, 2].set_title("Labeling Performance")
    
    # Plot classified results
    plot_vertices(axs[1, 0], labeled_chip, "Classified by CHIP", 'labeled')
    plot_vertices(axs[1, 1], labeled_rchip, "Classified by RCHIP", 'labeled')
    plot_vertices(axs[1, 2], labeled_nn, "Classified by NN", 'labeled')
    
    plt.tight_layout()
    plt.show()
