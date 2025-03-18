import numpy as np
import matplotlib.pyplot as plt

def str_to_int(s):
  d = 0
  for c in s:
    d += ord(c)
  return d

def plot_vertices(ax, vertices, title, type, edge_colors=None):
    """Plots the vertices in the specified subplot ax with edge colors based on correctness."""
    features = []
    labels = []
    for entry in vertices.entries:
        features.append(entry.features)
        if entry.cluster_id.HasField('cluster_id_int'):
            labels.append(entry.cluster_id.cluster_id_int)
        else:
            label_str = entry.cluster_id.cluster_id_str
            label_int = str_to_int(label_str)

            labels.append( (label_str, label_int) )
    
    features = np.array(features)

    labels_to_use_in_colors = [label if isinstance(label, int) else label[1] for label in labels]

    dim = features.shape[1]
    if dim not in [2, 3]:
        raise ValueError("Features must be 2 or 3-dimensional for plotting.")
    
    markers = {'dataset': 'o', 'labeled': 'o'}

    if edge_colors is None:
        edge_colors = 'k'

    if dim == 2:
        scatter = ax.scatter(features[:, 0], features[:, 1], c=labels_to_use_in_colors, edgecolors=edge_colors, marker=markers[type])
    elif dim == 3:
        scatter = ax.scatter(features[:, 0], features[:, 1], features[:, 2], c=labels_to_use_in_colors, edgecolors=edge_colors, marker=markers[type])

    ax.set_title(title)
    
    # Create a legend

    handles, legend_labels = scatter.legend_elements()

    unique_labels = set(labels)
    
    for i, legend_label in enumerate(legend_labels):
        for label in unique_labels:
            if not isinstance(label, int):
                legend_label_int = int(legend_label.strip('$\\mathdefault{}'))
                if label[1] == legend_label_int:
                    legend_labels[i] = label[0]
                    break

    legend1 = ax.legend(handles, legend_labels, title="Clusters")
    ax.add_artist(legend1)

def plot_test_grid(ax, test_grid, title, type):
    """Plots the test grid in the specified subplot ax."""
    features = []
    labels = []
    for entry in test_grid.entries:
        features.append(entry.features)
        if entry.cluster_id.HasField('cluster_id_int'):
            labels.append(entry.cluster_id.cluster_id_int)
        else:
            label_str = entry.cluster_id.cluster_id_str
            label_int = str_to_int(label_str)
            labels.append((label_str, label_int))

    features = np.array(features)
    labels_to_use_in_colors = [label if isinstance(label, int) else label[1] for label in labels]

    dim = features.shape[1]
    if dim != 2:
        raise ValueError("Test grid features must be 2-dimensional for plotting.")

    scatter = ax.scatter(features[:, 0], features[:, 1], c=labels_to_use_in_colors, cmap='viridis', marker='s', s=10, alpha=0.6)
    ax.set_title(title)

def plot_classification_results(dataset, labeled_chip, labeled_rchip, labeled_nn, dataset_name):
    """Creates a figure with six quadrants displaying dataset and classification results."""    
    dim = len(dataset.entries[0].features)

    if dim == 2:
        fig, axs = plt.subplots(2, 3, figsize=(15, 10))
    elif dim == 3:
        fig = plt.figure(figsize=(15, 10))
        axs = fig.subplots(2, 3, subplot_kw={'projection': '3d'})
    else:
        fig, axs = plt.subplots(1, 2, figsize=(15, 10))

    # Plot original dataset in quadrant 0 if 2D or 3D
    if dim in [2, 3]:
        plot_vertices(axs[0, 0], dataset, f"Dataset: {dataset_name}", 'dataset')

    if dim in [2, 3]:
        train_ax = axs[0, 1]
        label_ax = axs[0, 2]
    else:
        train_ax = axs[0]
        label_ax = axs[1]

    train_ax.axis('off')
    train_ax.set_title("Training")

    train_table_data = [
        ['Classifier', 'Time (ms)', 'Model Size (B)'],
        ['CHIP', f"{labeled_chip[1]['train_time']:.2f}", f"{labeled_chip[1]['model_size']}"],
        ['RCHIP', f"{labeled_rchip[1]['train_time']:.2f}", f"{labeled_rchip[1]['model_size']}"],
        ['NN', f"{labeled_nn[1]['train_time']:.2f}", f"{labeled_nn[1]['model_size']}"]
    ]

    # Create the table
    _ = train_ax.table(
        cellText=train_table_data,
        loc='center',
        cellLoc='center',
    )

    label_ax.axis('off')
    label_ax.set_title("Labeling")

    # Prepare data for tables
    label_table_data = [
        ['Classifier', 'Time (ms)'],
        ['CHIP', f"{labeled_chip[1]['label_time']:.2f}"],
        ['RCHIP', f"{labeled_rchip[1]['label_time']:.2f}"],
        ['NN', f"{labeled_nn[1]['label_time']:.2f}"]
    ]

    # Create the table
    _ = label_ax.table(
        cellText=label_table_data,
        loc='center',
        cellLoc='center',
    )
    
    # Plot classified results with correctness
    if dim in [2, 3]:
        for ax, (labeled_data, _) in zip(axs[1, :], [labeled_chip, labeled_rchip, labeled_nn]):
            plot_test_grid(ax, labeled_data, f"Test Grid: {dataset_name}", 'labeled')
        
        # Set titles for classified results
        axs[1, 0].set_title("CHIP")
        axs[1, 1].set_title("RCHIP")
        axs[1, 2].set_title("NN")
    
    plt.tight_layout()
    plt.show()
