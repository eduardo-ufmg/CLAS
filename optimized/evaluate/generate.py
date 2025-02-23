import random
import sys
from classifier_pb2 import TrainingDataset, TrainingDatasetEntry, VerticesToLabel, VertexToLabelEntry

def generate_synthetic_data(spread, tlspread, vertcount):
  synthetic_dataset = TrainingDataset()
  tolabel_dataset = VerticesToLabel()

  # Generate synthetic dataset
  for _ in range(vertcount):
    entry = TrainingDatasetEntry()
    entry.features.extend([random.uniform(-spread, spread) for _ in range(2)])
    entry.class_id_int = random.randint(0, 1)
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
  if len(sys.argv) != 4:
    print("Usage: generate.py <spread> <tlspread> <vertcount>")
    sys.exit(1)

  spread = float(sys.argv[1])
  tlspread = float(sys.argv[2])
  vertcount = int(sys.argv[3])

  synthetic_dataset, tolabel_dataset = generate_synthetic_data(spread, tlspread, vertcount)
  write_datasets(synthetic_dataset, tolabel_dataset)