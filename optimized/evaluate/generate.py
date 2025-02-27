import random
import argparse
import pathlib
from classifier_pb2 import TrainingDataset, TrainingDatasetEntry, VerticesToLabel, VertexToLabelEntry

def generate_synthetic_data(spread, tlspread, vertcount):
  synthetic_dataset = TrainingDataset()
  tolabel_dataset = VerticesToLabel()

  # Generate synthetic dataset
  for _ in range(vertcount):
    entry = TrainingDatasetEntry()
    entry.features.extend([random.uniform(-spread, spread) for _ in range(2)])
    entry.cluster_id.cluster_id_int = [-1, 1][random.randint(0, 1)]
    synthetic_dataset.entries.append(entry)

  # Generate tolabel dataset
  for _ in range(vertcount // 2):
    entry = VertexToLabelEntry()
    entry.features.extend([random.uniform(-tlspread, tlspread) for _ in range(2)])
    entry.vertex_id = random.randint(0, vertcount)
    tolabel_dataset.entries.append(entry)

  return synthetic_dataset, tolabel_dataset

def write_datasets(synthetic_dataset, tolabel_dataset):
  with open('../data/synthetic/synthetic', 'wb') as f:
    f.write(synthetic_dataset.SerializeToString())
  
  with open('../data/synthetic/tolabel', 'wb') as f:
    f.write(tolabel_dataset.SerializeToString())

if __name__ == "__main__":
  parser = argparse.ArgumentParser(description="Generate synthetic datasets for graph-based classifier.")
  parser.add_argument("--spread", type=float, help="Spread for synthetic dataset features")
  parser.add_argument("--tlspread", type=float, help="Spread for tolabel dataset features")
  parser.add_argument("--vertcount", type=int, help="Number of vertices")

  args = parser.parse_args()

  synthetic_dataset, tolabel_dataset = generate_synthetic_data(args.spread, args.tlspread, args.vertcount)
  write_datasets(synthetic_dataset, tolabel_dataset)
