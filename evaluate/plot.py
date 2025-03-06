import numpy as np
import matplotlib.pyplot as plt
from classifier_pb2 import TrainingDataset, SupportVertices, Experts, VerticesToLabel, LabeledVertices

def plot_vertices(vertices, type):
  # Extract features and labels from the vertices
  features = []
  labels = []
  for entry in vertices.entries:
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
  if type == 'dataset':
    marker = 'o'
  elif type == 'SVs':
    marker = ','
  elif type == 'labeled':
    marker = 'v'
  else:
    raise ValueError("Invalid type. Must be 'SVs', 'labeled', or 'dataset'.")

  _ = plt.scatter(features[:, 0], features[:, 1], c=labels, edgecolors='k', marker=marker)
