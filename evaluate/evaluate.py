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

  trainers = {
    "chip": "./chips-train",
    "rchip": "./chips-train",
    "nn": "./nn-train"
  }

  classifiers = {
    "chip": "./chip-label",
    "rchip": "./rchip-label",
    "nn": "./nn-label"
  }

  dataset_name = args.dataset
  dataset = "../data" / pathlib.Path(args.dataset) / pathlib.Path(args.dataset)
  trainer = trainers[args.classifier]
  classifier = classifiers[args.classifier]
  tolerance = str(args.tolerance)
  tolabel_path = pathlib.Path("../data") / dataset_name / pathlib.Path("tolabel") 

  pb_dataset = TrainingDataset()
  pb_dataset.ParseFromString(open(dataset, "rb").read())

  plot.plt.figure(figsize=(10, 8))
  plot.plt.title(dataset_name)

  # Plot the dataset
  plot.plot_vertices(pb_dataset, "dataset")
  
  subprocess.run([trainer, dataset, tolerance], cwd=classifiers_dir)

  if args.classifier == "nn":
    support_vertices_path = classifiers_dir / "train" / pathlib.Path("nn-" + dataset_name)
    pb_svs = SupportVertices()
    pb_svs.ParseFromString(open(support_vertices_path, "rb").read())
    plot.plot_vertices(pb_svs, "SVs")
    subprocess.run([classifier, tolabel_path, support_vertices_path], cwd=classifiers_dir)

  if args.classifier == "chip":
    experts_path = classifiers_dir / "train" / pathlib.Path("chips-" + dataset_name)
    pb_experts = Experts()
    pb_experts.ParseFromString(open(experts_path, "rb").read())
    subprocess.run([classifier, tolabel_path, experts_path], cwd=classifiers_dir)

  pb_labeled_vertices = LabeledVertices()
  pb_labeled_vertices.ParseFromString(open(classifiers_dir / "label" / pathlib.Path(args.classifier + "-" + dataset_name), "rb").read())
  plot.plot_vertices(pb_labeled_vertices, "labeled")

  plot.plt.show()
  
if __name__ == "__main__":
  main()
