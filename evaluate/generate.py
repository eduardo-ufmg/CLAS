import random
import argparse
import pathlib
import sklearn.datasets
import numpy as np
from classifier_pb2 import TrainingDataset, TrainingDatasetEntry, VerticesToLabel, VertexToLabelEntry

def generate_blob(noise, tlnoise, vertcount):
  centers = [(random.uniform(-1, 1), random.uniform(-1, 1)) for _ in range(2)]

  features, labels = sklearn.datasets.make_blobs(n_samples=vertcount,
                                                  centers=centers,
                                                  cluster_std=noise)
  
  tolabel_features, expected_labels = sklearn.datasets.make_blobs(n_samples=vertcount,
                                                                  centers=centers,
                                                                  cluster_std=tlnoise)
  
  return (features, labels), (tolabel_features, expected_labels)

def generate_circle(noise, tlnoise, vertcount):
  factor = random.uniform(0, 1)

  features, labels = sklearn.datasets.make_circles(n_samples=vertcount, noise=noise, factor=factor)

  tolabel_features, expected_labels = sklearn.datasets.make_circles(n_samples=vertcount, noise=tlnoise, factor=factor)

  return (features, labels), (tolabel_features, expected_labels)

def generate_moons(noise, tlnoise, vertcount):
  features, labels = sklearn.datasets.make_moons(n_samples=vertcount, noise=noise)

  tolabel_features, expected_labels = sklearn.datasets.make_moons(n_samples=vertcount, noise=tlnoise)

  return (features, labels), (tolabel_features, expected_labels)

def generate_xor(noise, tlnoise, vertcount):
  centers = [(1, 1), (-1, 1), (-1, -1), (1, -1)]
  
  features, labels = sklearn.datasets.make_blobs(n_samples=vertcount,
                                            centers=centers,
                                            cluster_std=noise)
  
  labels = [label % 2 for label in labels]

  tolabel_features, expected_labels = sklearn.datasets.make_blobs(n_samples=vertcount,
                                                    centers=centers,
                                                    cluster_std=tlnoise)
  
  expected_labels = [label % 2 for label in expected_labels]

  return (features, labels), (tolabel_features, expected_labels)
  

def generate_synthetic_data(type, noise, vertcount):
  dataset = TrainingDataset()
  tolabel_dataset = VerticesToLabel()

  tlnoise = noise * 2

  if type == "blob":
    (features, labels), (tolabel_features, expected_labels) = generate_blob(noise, tlnoise, vertcount)

  elif type == "circle":
    (features, labels), (tolabel_features, expected_labels) = generate_circle(noise, tlnoise, vertcount)

  elif type == "moons":
    (features, labels), (tolabel_features, expected_labels) = generate_moons(noise, tlnoise, vertcount)

  elif type == "xor":
    (features, labels), (tolabel_features, expected_labels) = generate_xor(noise, tlnoise, vertcount)
  else:
    raise ValueError("Invalid synthetic dataset type")
  
  labels = [[-1, 1][label] for label in labels]

  for i in range(vertcount):
    entry = dataset.entries.add()
    entry.features.extend(features[i])
    entry.cluster_id.cluster_id_int = labels[i]

  expected_labels = [[-1, 1][label] for label in expected_labels]

  for i in range(vertcount):
    entry = tolabel_dataset.entries.add()
    entry.vertex_id = -i - 1
    entry.features.extend(tolabel_features[i])
    entry.expected_cluster_id.cluster_id_int = expected_labels[i]

  return dataset, tolabel_dataset


def write_datasets(type, dataset, tolabel_dataset):
  dataset_path = pathlib.Path(f"../data/{type}/{type}")
  tolabel_path = pathlib.Path(f"../data/{type}/tolabel")

  dataset_path.parent.mkdir(parents=True, exist_ok=True)

  with open(dataset_path, "wb") as f:
    f.write(dataset.SerializeToString())

  with open(tolabel_path, "wb") as f:
    f.write(tolabel_dataset.SerializeToString())

if __name__ == "__main__":
  parser = argparse.ArgumentParser(description="Generate synthetic datasets for graph-based classifier.")
  parser.add_argument("--type", type=str, choices=["blob", "circle", "moons", "xor"], help="Type of synthetic dataset to generate")
  parser.add_argument("--noise", type=float, help="Spread for synthetic dataset features")
  parser.add_argument("--vertcount", type=int, help="Number of vertices")
  args = parser.parse_args()

  synthetic_dataset, tolabel_dataset = generate_synthetic_data(args.type, args.noise, args.vertcount)
  write_datasets(args.type, synthetic_dataset, tolabel_dataset)
