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
        
    scatter = ax.scatter(features[:, 0], features[:, 1], c=labels, edgecolors=edge_colors, marker=markers[type])
    ax.set_title(title)
    
    # Create a legend
    legend1 = ax.legend(*scatter.legend_elements(), title="Clusters")
    ax.add_artist(legend1)


def plot_classification_results(dataset, labeled_chip, labeled_rchip, labeled_nn, dataset_name, label_accuracy):
    """Creates a figure with six quadrants displaying dataset and classification results."""
    fig, axs = plt.subplots(2, 3, figsize=(15, 10))
    
    # Plot original dataset in quadrant 0
    plot_vertices(axs[0, 0], dataset, f"Dataset: {dataset_name}", 'dataset')
    
    # Reserve quadrants 1 and 2 for tables
    train_ax = axs[0, 1]
    train_ax.axis('off')
    train_ax.set_title("Training")

    train_table_data = [
        ['Classifier', 'CHIP', 'RCHIP', 'NN'],
        ['Time (ms)', f"{labeled_chip[1]['train_time']:.2f}", f"{labeled_rchip[1]['train_time']:.2f}", f"{labeled_nn[1]['train_time']:.2f}"],
        ['Model Size (B)', f"{labeled_chip[1]['model_size']}", f"{labeled_rchip[1]['model_size']}", f"{labeled_nn[1]['model_size']}"]
    ]

    # Create the table
    train_table = train_ax.table(
        cellText=train_table_data,
        loc='center',
        cellLoc='center',
    )

    train_table.auto_set_font_size(False)
    train_table.set_fontsize(10)
    train_table.scale(1, 1.5)

    label_ax = axs[0, 2]
    label_ax.axis('off')
    label_ax.set_title("Labeling")

    chip_auc = labeled_chip[1]['auc']
    rchip_auc = labeled_rchip[1]['auc']
    nn_auc = labeled_nn[1]['auc']

    # Prepare data for tables
    label_table_data = [
        ['Classifier', 'CHIP', 'RCHIP', 'NN'],
        ['Accuracy (%)', f"{label_accuracy['CHIP']:.2f}%", f"{label_accuracy['RCHIP']:.2f}%", f"{label_accuracy['NN']:.2f}%"],
        ['AUC', f"{chip_auc:.2f}", f"{rchip_auc:.2f}", f"{nn_auc:.2f}"],
        ['Time (ms)', f"{labeled_chip[1]['label_time']:.2f}", f"{labeled_rchip[1]['label_time']:.2f}", f"{labeled_nn[1]['label_time']:.2f}"]
    ]

    # Create the table
    label_table = label_ax.table(
        cellText=label_table_data,
        loc='center',
        cellLoc='center',
    )

    label_table.auto_set_font_size(False)
    label_table.set_fontsize(10)
    label_table.scale(1, 1.5)
    
    # Plot classified results with correctness
    for ax, (labeled_data, metrics) in zip(axs[1, :], [labeled_chip, labeled_rchip, labeled_nn]):
        edge_colors = ['g' if correct else 'r' for correct in metrics['correctness']]
        plot_vertices(ax, labeled_data, ax.get_title(), 'labeled', edge_colors=edge_colors)
    
    # Set titles for classified results
    axs[1, 0].set_title("CHIP")
    axs[1, 1].set_title("RCHIP")
    axs[1, 2].set_title("NN")
    
    plt.tight_layout()
    plt.show()
