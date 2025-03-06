import argparse
import pathlib
import subprocess
import plot
from classifier_pb2 import TrainingDataset, SupportVertices, Experts, VerticesToLabel, LabeledVertices

def main():
    parser = argparse.ArgumentParser("Train and evaluate multiple classifiers")
    parser.add_argument("--dataset", required=True, help="Path to the dataset")
    parser.add_argument("--tolerance", default=1.0, help="Filter tolerance")
    args = parser.parse_args()
    
    classifiers_dir = pathlib.Path("../bin")
    dataset_name = args.dataset
    dataset_path = pathlib.Path("../data") / dataset_name / dataset_name
    tolerance = str(args.tolerance)
    tolabel_path = pathlib.Path("../data") / dataset_name / "tolabel"
    
    # Load dataset
    pb_dataset = TrainingDataset()
    pb_dataset.ParseFromString(open(dataset_path, "rb").read())
    
    # Define classifiers
    classifiers = {
        "chip": {"trainer": "./chips-train", "labeler": "./chip-label"},
        "rchip": {"trainer": "./chips-train", "labeler": "./rchip-label"},
        "nn": {"trainer": "./nn-train", "labeler": "./nn-label"}
    }
    
    # Storage for labeled results
    labeled_results = {}
    
    for clf_name, paths in classifiers.items():
        trainer = paths["trainer"]
        labeler = paths["labeler"]
        
        # Train classifier
        subprocess.run([trainer, str(dataset_path), tolerance], cwd=classifiers_dir)
        
        # Determine file paths for trained model
        if clf_name == "nn":
            trained_model_path = classifiers_dir / "train" / f"nn-{dataset_name}"
        else:
            trained_model_path = classifiers_dir / "train" / f"chips-{dataset_name}"
        
        # Label dataset
        subprocess.run([labeler, str(tolabel_path), str(trained_model_path)], cwd=classifiers_dir)
        
        # Load labeled results
        labeled_path = classifiers_dir / "label" / f"{clf_name}-{dataset_name}"
        pb_labeled = LabeledVertices()
        pb_labeled.ParseFromString(open(labeled_path, "rb").read())
        labeled_results[clf_name] = pb_labeled
    
    # Plot results
    plot.plot_classification_results(
        pb_dataset, 
        labeled_results["chip"], 
        labeled_results["rchip"], 
        labeled_results["nn"], 
        dataset_name
    )

if __name__ == "__main__":
    main()
