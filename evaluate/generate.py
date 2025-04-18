import argparse
import pathlib
import numpy as np
from synthetic_2d import generate_2d_synthetic_data
from synthetic_3d import generate_3d_synthetic_data
from synthetic_nd import generate_multidim_blob

def write_datasets(type, dataset, test_dataset):
  dataset_path = pathlib.Path(f"../data/{type}/{type}")
  test_path = pathlib.Path(f"../data/{type}/test")

  dataset_path.parent.mkdir(parents=True, exist_ok=True)

  with open(dataset_path, "wb") as f:
    f.write(dataset.SerializeToString())

  with open(test_path, "wb") as f:
    f.write(test_dataset.SerializeToString())

if __name__ == "__main__":
  parser = argparse.ArgumentParser(description="Generate synthetic datasets for graph-based classifier.")
  parser.add_argument("--dim", type=int, default=2, help="Dimension of synthetic dataset")
  parser.add_argument("--type", type=str, choices=["blob", "circle", "moons", "xor", "spiral"], required=True, help="Type of synthetic dataset to generate")
  parser.add_argument("--idtype", type=str, choices=["int", "str"], default="int", help="Type of cluster ID to use")
  parser.add_argument("--noise", type=float, default=0.01, help="Spread for synthetic dataset features")
  parser.add_argument("--samples", type=int, default=100, help="Number of vertices")
  parser.add_argument("--grid_res", type=int, default=100, help="Resolution of grid for 2D and 3D datasets")
  args = parser.parse_args()

  if args.dim == 2:
    synthetic_dataset, test_dataset = generate_2d_synthetic_data(args.type, args.idtype, args.noise, args.samples, args.grid_res)
  elif args.dim == 3:
    synthetic_dataset, test_dataset = generate_3d_synthetic_data(args.type, args.idtype, args.noise, args.samples, args.grid_res)
  else:
    synthetic_dataset, test_dataset = generate_multidim_blob(args.noise, args.idtype, args.samples, args.dim)

  write_datasets(args.type, synthetic_dataset, test_dataset)
