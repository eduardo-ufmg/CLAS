import argparse
import pathlib
import subprocess
import plot
import metrics
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

    # Load to-label vertices
    pb_tolabel = VerticesToLabel()
    pb_tolabel.ParseFromString(open(tolabel_path, "rb").read())
    expected_dict = {entry.vertex_id: entry.expected_cluster_id for entry in pb_tolabel.entries}
    
    # Define classifiers
    classifiers = {
        "chip": {"trainer": "./chips-train", "labeler": "./chip-label"},
        "rchip": {"trainer": "./chips-train", "labeler": "./rchip-label"},
        "nn": {"trainer": "./nn-train", "labeler": "./nn-label"}
    }
    
    # Storage for labeled results and correctness
    labeled_results = {}
    
    for clf_name, paths in classifiers.items():
        trainer = paths["trainer"]
        labeler = paths["labeler"]
        
        # Train classifier
        train_time = metrics.run_and_measure_time([trainer, str(dataset_path), tolerance], classifiers_dir)

        # Determine file paths for trained model
        if clf_name == "nn":
            trained_model_path = classifiers_dir / "train" / f"nn-{dataset_name}"
        else:
            trained_model_path = classifiers_dir / "train" / f"chips-{dataset_name}"

        # Model size
        model_size = trained_model_path.stat().st_size
        
        # Label dataset
        label_time = metrics.run_and_measure_time([labeler, str(tolabel_path), str(trained_model_path)], cwd=classifiers_dir)
        
        # Load labeled results
        labeled_path = classifiers_dir / "label" / f"{clf_name}-{dataset_name}"
        pb_labeled = LabeledVertices()
        pb_labeled.ParseFromString(open(labeled_path, "rb").read())
        
        # Check correctness
        correctness = metrics.vertexwise_correctness(pb_labeled, expected_dict)

        # Mean AUC
        auc = metrics.mean_auc(pb_labeled, expected_dict)

        run_metrics = {
            "train_time": train_time,
            "model_size": model_size,
            "label_time": label_time,
            "correctness": correctness,
            "auc": auc
        }
        
        labeled_results[clf_name] = (pb_labeled, run_metrics)

    # Calculate accuracies
    accuracies = {
        'CHIP': (sum(labeled_results["chip"][1]["correctness"]) / len(labeled_results["chip"][1]["correctness"])) * 100,
        'RCHIP': (sum(labeled_results["rchip"][1]["correctness"]) / len(labeled_results["rchip"][1]["correctness"])) * 100,
        'NN': (sum(labeled_results["nn"][1]["correctness"]) / len(labeled_results["nn"][1]["correctness"])) * 100
    }

    accuracies = {k: round(v, 2) for k, v in accuracies.items()}

    # Plot results
    plot.plot_classification_results(
        pb_dataset, 
        labeled_results["chip"], 
        labeled_results["rchip"], 
        labeled_results["nn"], 
        dataset_name,
        accuracies,
    )

if __name__ == "__main__":
    main()
    