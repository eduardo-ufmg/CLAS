import argparse
import pathlib
import subprocess
import plot
from classifier_pb2 import TrainingDataset, SupportVertices, Experts, VerticesToLabel, LabeledVertices

def main():
  parser = argparse.ArgumentParser("Train a classifier")
  parser.add_argument("--dataset", required=True, help="Path to the dataset")
  parser.add_argument("--classifier", required=True, choices=["chip", "rchip", "nn"], help="Classifier to train")
  parser.add_argument("--tolerance", default=1.0, help="Filter tolerance")
  args = parser.parse_args()

  classifiers_dir = pathlib.Path("../bin")

  classifiers_exe = {
    "chip": "./chip-train",
    "rchip": "./rchip-train",
    "nn": "./nn-train"
  }

  dataset_name = args.dataset
  dataset = "../data" / pathlib.Path(args.dataset) / pathlib.Path(args.dataset)
  classifier = classifiers_exe[args.classifier]
  tolerance = str(args.tolerance)

  pb_dataset = TrainingDataset()
  pb_dataset.ParseFromString(open(dataset, "rb").read())

  plot.plt.figure(figsize=(10, 8))
  plot.plt.title(dataset_name)

  # Plot the dataset
  plot.plot_dataset(pb_dataset, dataset_name)
  
  subprocess.run([classifier, dataset, tolerance], cwd=classifiers_dir)

  if args.classifier == "nn":
    pb_svs = SupportVertices()
    pb_svs.ParseFromString(open(classifiers_dir / "train" / pathlib.Path("nn-" + dataset_name), "rb").read())
    plot.plot_SVs(pb_svs)

  plot.plt.show()
  
if __name__ == "__main__":
  main()
