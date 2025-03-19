import sklearn.datasets
import numpy as np
from classifier_pb2 import TrainingDataset, VerticesToLabel
from correct_labels import correct_labels

def generate_multidim_blob(noise, idtype, vertcount, dim):
  tlnoise = noise * 2

  centers = [np.random.uniform(-1, 1, dim) for _ in range(2)]

  features, labels = sklearn.datasets.make_blobs(n_samples=vertcount,
                                                  centers=centers,
                                                  cluster_std=noise)
  
  test_features, expected_labels = sklearn.datasets.make_blobs(n_samples=vertcount,
                                                                  centers=centers,
                                                                  cluster_std=tlnoise)
  
  dataset = TrainingDataset()
  test_dataset = VerticesToLabel()

  labels = correct_labels(labels, idtype)
  expected_labels = correct_labels(expected_labels, idtype)

  for i in range(vertcount):
    entry = dataset.entries.add()
    entry.features.extend(features[i])
    if idtype == "int":
      entry.cluster_id.cluster_id_int = labels[i]
    elif idtype == "str":
      entry.cluster_id.cluster_id_str = labels[i]

  for i in range(vertcount):
    entry = test_dataset.entries.add()
    entry.vertex_id = -i - 1
    entry.features.extend(test_features[i])
    if idtype == "int":
      entry.expected_cluster_id.cluster_id_int = expected_labels[i]
    elif idtype == "str":
      entry.expected_cluster_id.cluster_id_str = expected_labels[i]

  return dataset, test_dataset