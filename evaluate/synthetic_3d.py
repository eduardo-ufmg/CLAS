import sklearn.datasets
import numpy as np
from classifier_pb2 import TrainingDataset, VerticesToLabel
from correct_labels import correct_labels

def generate_3d_blob(noise, samples):
  centers = [(np.random.uniform(-1, 1), np.random.uniform(-1, 1), np.random.uniform(-1, 1)) for _ in range(2)]

  features, labels = sklearn.datasets.make_blobs(n_samples=samples,
                                                  centers=centers,
                                                  cluster_std=noise)
  
  return features, labels

def generate_hemisphere(radius, points, noise):
  features = []

  for i in range(points):
    theta = np.random.uniform(0, 2 * np.pi)
    phi = np.random.uniform(0, np.pi / 2)

    x = radius * np.sin(phi) * np.cos(theta)
    y = radius * np.sin(phi) * np.sin(theta)
    z = radius * np.cos(phi)

    x += np.random.uniform(-noise, noise)
    y += np.random.uniform(-noise, noise)
    z += np.random.uniform(-noise, noise)

    features.append([x, y, z])

  return features

def generate_hemispheres(noise, samples):
  half = samples // 2
  outer_radius = np.random.uniform(1, 2)
  inner_radius = np.random.uniform(0, 1)

  features = generate_hemisphere(outer_radius, half, noise)
  labels = [1] * half

  features += generate_hemisphere(inner_radius, half, noise)
  labels += [0] * half

  return features, labels

def generate_3d_moons(noise, samples):
  radius = np.random.uniform(1, 2)

  features, labels = sklearn.datasets.make_moons(n_samples=samples, noise=noise)

  phi = np.random.uniform(0, 2 * np.pi, features.shape[0])

  features3d = np.zeros((features.shape[0], 3))
  features3d[:, 0] = (radius + features[:, 0]) * np.cos(phi)
  features3d[:, 1] = (radius + features[:, 0]) * np.sin(phi)
  features3d[:, 2] = features[:, 1]

  return features3d, labels

def generate_3d_xor(noise, samples):
  centers = [(1, 1, 1), (-1, 1, 1), (-1, -1, 1), (1, -1, 1), (1, 1, -1), (-1, 1, -1), (-1, -1, -1), (1, -1, -1)]
  
  features, labels = sklearn.datasets.make_blobs(n_samples=samples,
                                            centers=centers,
                                            cluster_std=noise)
  
  labels = [label % 2 for label in labels]

  return features, labels

def generate_3d_spiral(noise, samples):
  features = []
  labels = []

  for i in range(samples):
    r = i / samples * 5
    theta = i / samples * 4 * np.pi
    phi = i / samples * 2 * np.pi

    x = r * np.cos(theta) * np.sin(phi)
    y = r * np.sin(theta) * np.sin(phi)
    z = r * np.cos(phi)

    x += np.random.uniform(-noise, noise)
    y += np.random.uniform(-noise, noise)
    z += np.random.uniform(-noise, noise)

    features.append([x, y, z])
    labels.append(0)

  return features, labels

def generate_3d_synthetic_data(type, idtype, noise, samples, grid_res):
  dataset = TrainingDataset()
  test_grid = VerticesToLabel()

  if type == "blob":
    features, labels = generate_3d_blob(noise, samples)

  elif type == "circle":
    features, labels = generate_hemispheres(noise, samples)

  elif type == "moons":
    features, labels = generate_3d_moons(noise, samples)

  elif type == "xor":
    features, labels = generate_3d_xor(noise, samples)

  elif type == "spiral":
    features, labels = generate_3d_spiral(noise, samples)

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
  grid_z_max = max(features, key=lambda x: x[2])[2]
  grid_z_min = min(features, key=lambda x: x[2])[2]

  grid_x = np.linspace(grid_x_min, grid_x_max, grid_res)
  grid_y = np.linspace(grid_y_min, grid_y_max, grid_res)
  grid_z = np.linspace(grid_z_min, grid_z_max, grid_res)

  grid_x, grid_y, grid_z = np.meshgrid(grid_x, grid_y, grid_z)

  for x, y, z in zip(grid_x.flatten(), grid_y.flatten(), grid_z.flatten()):
    entry = test_grid.entries.add()
    entry.vertex_id = -1
    entry.features.extend([x, y, z])
    if idtype == "int":
      entry.expected_cluster_id.cluster_id_int = 0
    elif idtype == "str":
      entry.expected_cluster_id.cluster_id_str = '\0'

  return dataset, test_grid
