import argparse
import pathlib
import plot
from classifier_pb2 import TrainingDataset

def main():
  parser = argparse.ArgumentParser(description="Plot a dataset from a binary file.")
  parser.add_argument("--dataset", required=True, help="Path to the dataset binary file")
  args = parser.parse_args()

  dataset_path = pathlib.Path(args.dataset)

  # Load dataset
  pb_dataset = TrainingDataset()
  pb_dataset.ParseFromString(open(dataset_path, "rb").read())

  # Determine dimensionality
  dim = len(pb_dataset.entries[0].features)

  # Plot dataset
  if dim == 2:
    plot.plot_vertices(plot.plt.gca(), pb_dataset, "2D Dataset", 'dataset')
  elif dim == 3:
    fig = plot.plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    plot.plot_vertices(ax, pb_dataset, "3D Dataset", 'dataset')
  else:
    raise ValueError("Features must be 2 or 3-dimensional for plotting.")

  plot.plt.show()

if __name__ == "__main__":
  main()
  