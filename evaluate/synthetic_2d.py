import sklearn.datasets
import numpy as np
from classifier_pb2 import TrainingDataset, VerticesToLabel
from correct_labels import correct_labels

def generate_blob(noise, samples):
  centers = [(np.random.uniform(-1, 1), np.random.uniform(-1, 1)) for _ in range(2)]

  features, labels = sklearn.datasets.make_blobs(n_samples=samples,
                                                  centers=centers,
                                                  cluster_std=noise)
  
  return (features, labels)

def generate_circle(noise, samples):
  factor = np.random.uniform(0, 1)

  features, labels = sklearn.datasets.make_circles(n_samples=samples, noise=noise, factor=factor)

  return (features, labels)

def generate_moons(noise, samples):
  features, labels = sklearn.datasets.make_moons(n_samples=samples, noise=noise)

  return (features, labels)

def generate_xor(noise, samples):
  centers = [(1, 1), (-1, 1), (-1, -1), (1, -1)]
  
  features, labels = sklearn.datasets.make_blobs(n_samples=samples,
                                            centers=centers,
                                            cluster_std=noise)
  
  labels = [label % 2 for label in labels]

  return (features, labels)

def generate_spirals(noise, samples):
  n = samples // 2

  theta1 = np.linspace(0, 4 * np.pi, n)
  r1 = theta1
  x1 = r1 * np.cos(theta1) + np.random.normal(0, noise, n)
  y1 = r1 * np.sin(theta1) + np.random.normal(0, noise, n)

  theta2 = np.linspace(0, 4 * np.pi, n)
  r2 = theta2
  x2 = r2 * np.cos(theta2 + np.pi) + np.random.normal(0, noise, n)
  y2 = r2 * np.sin(theta2 + np.pi) + np.random.normal(0, noise, n)

  features = np.vstack([np.column_stack((x1, y1)), np.column_stack((x2, y2))])
  labels = np.hstack([np.zeros(n, dtype=int), np.ones(n, dtype=int)])

  return (features, labels)

def generate_2d_synthetic_data(type, idtype, noise, samples, grid_res):
  dataset = TrainingDataset()
  test_grid = VerticesToLabel()

  if type == "blob":
    (features, labels) = generate_blob(noise, samples)

  elif type == "circle":
    (features, labels) = generate_circle(noise, samples)

  elif type == "moons":
    (features, labels) = generate_moons(noise, samples)

  elif type == "xor":
    (features, labels) = generate_xor(noise, samples)

  elif type == "spiral":
    (features, labels) = generate_spirals(noise, samples)
    
  else:
    raise ValueError("Invalid synthetic dataset type")
  
  labels = correct_labels(labels, idtype)

  for i in range(samples):
    entry = dataset.entries.add()
    entry.features.extend(features[i])
    if idtype == "int":
      entry.cluster_id.cluster_id_int = labels[i]
    elif idtype == "str":
      entry.cluster_id.cluster_id_str = labels[i]

  grid_x_max = max(features, key=lambda x: x[0])[0]
  grid_x_min = min(features, key=lambda x: x[0])[0]
  grid_y_max = max(features, key=lambda x: x[1])[1]
  grid_y_min = min(features, key=lambda x: x[1])[1]

  grid_x, grid_y = np.meshgrid(np.linspace(grid_x_min, grid_x_max, grid_res),
                               np.linspace(grid_y_min, grid_y_max, grid_res))

  for x, y in zip(grid_x.ravel(), grid_y.ravel()):
    entry = test_grid.entries.add()
    entry.vertex_id = -1
    entry.features.extend([x, y])
    if idtype == "int":
      entry.expected_cluster_id.cluster_id_int = 0
    elif idtype == "str":
      entry.expected_cluster_id.cluster_id_str = '\0'

  return dataset, test_grid