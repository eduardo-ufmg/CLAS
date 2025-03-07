import random
import argparse
import pathlib
import sklearn.datasets
import numpy as np
from classifier_pb2 import TrainingDataset, TrainingDatasetEntry, VerticesToLabel, VertexToLabelEntry

def generate_blob(noise, vertcount):
  blob_dataset = TrainingDataset()
  tolabel_dataset = VerticesToLabel()

  tlnoise = noise * 2

  centers = [(random.uniform(-1, 1), random.uniform(-1, 1)) for _ in range(2)]

  blob_features, blob_labels = sklearn.datasets.make_blobs(n_samples=vertcount,
                                                            centers=centers,
                                                            cluster_std=noise)
  
  blob_labels = [[-1, 1][label] for label in blob_labels]

  for i in range(vertcount):
    entry = blob_dataset.entries.add()
    entry.features.extend(blob_features[i])
    entry.cluster_id.cluster_id_int = blob_labels[i]

  tolabel_features, expected_labels = sklearn.datasets.make_blobs(n_samples=vertcount,
                                                                  centers=centers,
                                                                  cluster_std=tlnoise)
  
  expected_labels = [[-1, 1][label] for label in expected_labels]

  for i in range(vertcount):
    entry = tolabel_dataset.entries.add()
    entry.vertex_id = -i - 1
    entry.features.extend(tolabel_features[i])
    entry.expected_cluster_id.cluster_id_int = expected_labels[i]


  return blob_dataset, tolabel_dataset

def generate_synthetic_data(type, spread, vertcount):
  if type == "blob":
    return generate_blob(spread, vertcount)
  elif type == "circle":
    return generate_circle(spread, vertcount)
  elif type == "moons":
    return generate_moons(spread, vertcount)
  elif type == "xor":
    return generate_xor(spread, vertcount)
  else:
    raise ValueError("Invalid synthetic dataset type")

def write_datasets(type, dataset, tolabel_dataset):
  dataset_path = pathlib.Path(f"../data/{type}/{type}")
  tolabel_path = pathlib.Path(f"../data/{type}/tolabel")

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
