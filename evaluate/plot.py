import numpy as np
import matplotlib.pyplot as plt
from classifier_pb2 import TrainingDataset, SupportVertices, Experts, VerticesToLabel, LabeledVertices

def plot_dataset(dataset: TrainingDataset, dataset_name: str):
  # Extract features and labels from the dataset
  features = []
  labels = []
  for entry in dataset.entries:
    features.append(entry.features)
    if entry.cluster_id.HasField('cluster_id_int'):
      labels.append(entry.cluster_id.cluster_id_int)
    else:
      labels.append(entry.cluster_id.cluster_id_str)
  
  features = np.array(features)
  
  # Assuming the features are 2D for plotting purposes
  if features.shape[1] != 2:
    raise ValueError("Features must be 2-dimensional for plotting.")
  
  # Create a scatter plot
  _ = plt.scatter(features[:, 0], features[:, 1], c=labels)

def plot_SVs(svs: SupportVertices):
  # Extract features and labels from the svs
  features = []
  labels = []
  for entry in svs.entries:
    features.append(entry.features)
    if entry.cluster_id.HasField('cluster_id_int'):
      labels.append(entry.cluster_id.cluster_id_int)
    else:
      labels.append(entry.cluster_id.cluster_id_str)
  
  features = np.array(features)
  
  # Assuming the features are 2D for plotting purposes
  if features.shape[1] != 2:
    raise ValueError("Features must be 2-dimensional for plotting.")
  
  # Create a scatter plot
  _ = plt.scatter(features[:, 0], features[:, 1], c=labels, marker='x', s=200)
