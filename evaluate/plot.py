import numpy as np
import matplotlib.pyplot as plt
from classifier_pb2 import TrainingDataset, SupportVertices, Experts, VerticesToLabel, LabeledVertices

def plot_dataset(dataset: TrainingDataset):
  X = np.array([[v.x, v.y] for v in dataset.vertices])
  y = np.array([v.label for v in dataset.vertices])
  plt.scatter(X[:, 0], X[:, 1], c=y)
  plt.show()

def highlight_SVs(dataset: TrainingDataset, SVs: SupportVertices):
  X = np.array([[v.x, v.y] for v in dataset.vertices])
  y = np.array([v.label for v in dataset.vertices])
  plt.scatter(X[:, 0], X[:, 1], c=y)
  SVs = np.array([[v.x, v.y] for v in SVs.vertices])
  plt.scatter(SVs[:, 0], SVs[:, 1], c='red')
  plt.show()

def plot_labeled_vertices(dataset: TrainingDataset, labeled_vertices: LabeledVertices):
  X = np.array([[v.x, v.y] for v in dataset.vertices])
  y = np.array([v.label for v in dataset.vertices])
  plt.scatter(X[:, 0], X[:, 1], c=y)
  labeled_vertices = np.array([[v.x, v.y] for v in labeled_vertices.vertices])
  labels = np.array([v.label for v in labeled_vertices])
  plt.scatter(labeled_vertices[:, 0], labeled_vertices[:, 1], c=labels)
  plt.show()
