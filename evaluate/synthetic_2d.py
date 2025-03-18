import random
import sklearn.datasets
import numpy as np
from classifier_pb2 import TrainingDataset, VerticesToLabel
from correct_labels import correct_labels

def generate_blob(noise, tlnoise, vertcount):
  centers = [(np.random.uniform(-1, 1), np.random.uniform(-1, 1)) for _ in range(2)]

  features, labels = sklearn.datasets.make_blobs(n_samples=vertcount,
                                                  centers=centers,
                                                  cluster_std=noise)
  
  return (features, labels)

def generate_circle(noise, tlnoise, vertcount):
  factor = np.random.uniform(0, 1)

  features, labels = sklearn.datasets.make_circles(n_samples=vertcount, noise=noise, factor=factor)

  return (features, labels)

def generate_moons(noise, tlnoise, vertcount):
  features, labels = sklearn.datasets.make_moons(n_samples=vertcount, noise=noise)

  return (features, labels)

def generate_xor(noise, tlnoise, vertcount):
  centers = [(1, 1), (-1, 1), (-1, -1), (1, -1)]
  
  features, labels = sklearn.datasets.make_blobs(n_samples=vertcount,
                                            centers=centers,
                                            cluster_std=noise)
  
  labels = [label % 2 for label in labels]

  return (features, labels)
  

def generate_2d_synthetic_data(type, idtype, noise, vertcount, grid_res):
  dataset = TrainingDataset()
  test_grid = VerticesToLabel()

  tlnoise = noise * 2

  if type == "blob":
    (features, labels) = generate_blob(noise, tlnoise, vertcount)

  elif type == "circle":
    (features, labels) = generate_circle(noise, tlnoise, vertcount)

  elif type == "moons":
    (features, labels) = generate_moons(noise, tlnoise, vertcount)

  elif type == "xor":
    (features, labels) = generate_xor(noise, tlnoise, vertcount)
  else:
    raise ValueError("Invalid synthetic dataset type")
  
  labels = correct_labels(labels, idtype)

  for i in range(vertcount):
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

  grid_x = np.linspace(grid_x_min, grid_x_max, grid_res)
  grid_y = np.linspace(grid_y_min, grid_y_max, grid_res)

  for x in grid_x:
    for y in grid_y:
      entry = test_grid.entries.add()
      entry.vertex_id = -1
      entry.features.extend([x, y])
      if idtype == "int":
        entry.expected_cluster_id.cluster_id_int = 0
      elif idtype == "str":
        entry.expected_cluster_id.cluster_id_str = '\0'

  return dataset, test_grid