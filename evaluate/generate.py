import random
import argparse
import pathlib
import sklearn.datasets
import numpy as np
from classifier_pb2 import TrainingDataset, TrainingDatasetEntry, VerticesToLabel, VertexToLabelEntry

def generate_synthetic_data(spread, vertcount):
  synthetic_dataset = TrainingDataset()
  tolabel_dataset = VerticesToLabel()

  tlspread = spread * 2

  centers = [(random.uniform(-1, 1), random.uniform(-1, 1)) for _ in range(2)]

  synthetic_features, synthetic_labels = sklearn.datasets.make_blobs(n_samples=vertcount,
                                                                     centers=centers,
                                                                     cluster_std=spread)

  for i in range(vertcount):
    entry = synthetic_dataset.entries.add()
    entry.features.extend(synthetic_features[i])
    entry.cluster_id.cluster_id_int = synthetic_labels[i]

  tolabel_features, _ = sklearn.datasets.make_blobs(n_samples=vertcount,
                                                    centers=centers,
                                                    cluster_std=tlspread)

  for i in range(vertcount):
    entry = tolabel_dataset.entries.add()
    entry.features.extend(tolabel_features[i])


  return synthetic_dataset, tolabel_dataset

def write_datasets(synthetic_dataset, tolabel_dataset):
  with open('../data/synthetic/synthetic', 'wb') as f:
    f.write(synthetic_dataset.SerializeToString())
  
  with open('../data/synthetic/tolabel', 'wb') as f:
    f.write(tolabel_dataset.SerializeToString())

if __name__ == "__main__":
  parser = argparse.ArgumentParser(description="Generate synthetic datasets for graph-based classifier.")
  parser.add_argument("--spread", type=float, help="Spread for synthetic dataset features")
  parser.add_argument("--vertcount", type=int, help="Number of vertices")

  args = parser.parse_args()

  synthetic_dataset, tolabel_dataset = generate_synthetic_data(args.spread, args.vertcount)
  write_datasets(synthetic_dataset, tolabel_dataset)
