import argparse
import pathlib
import subprocess
import numpy as np
import matplotlib.pyplot as plt
from typing import List

import generate_data
import parse_files
import plots
from paths import *

NUM_CLASSES = 2
NUM_DIMENSIONS = 2

def run_command(cmd_input_file: List[str], cwd: str) -> None:
    """
    Run a subprocess command in the specified working directory.
    
    Args:
      cmd_input_file: List of command arguments.
      cwd: Working directory in which to execute the command.
    
    Raises:
      subprocess.CalledProcessError if the command fails.
    """
    print("Running command:", " ".join(cmd_input_file))
    try:
        subprocess.run(cmd_input_file, check=True, cwd=cwd)
        print("Command finished successfully.")
    except subprocess.CalledProcessError as e:
        print(f"Command failed: {e}")
        raise

def main():
    parser = argparse.ArgumentParser(
        description="Generate synthetic training and to-classify data, run Gabriel Graph and Low Degree Filter, then run hyperplanes and chip-clas, and plot results."
    )
    parser.add_argument("--num_vertices", type=int, help="Total number of training vertices", default=100)
    parser.add_argument("--spread", type=float, help="Spread factor for the training clusters", default=0.5)
    parser.add_argument("--to_classify_spread", type=float, help="Spread factor for the to-classify clusters", default=0.5)
    parser.add_argument("--seed", type=int, help="Random seed for reproducibility", default=None)
    parser.add_argument("--threshold_factor", type=float, help="Threshold factor for the filter", default=1.0)
    args = parser.parse_args()
    
    # Generate synthetic training data and capture the centers
    training_data, centers = generate_data.generate_synthetic_data(
        NUM_CLASSES, NUM_DIMENSIONS, args.num_vertices, args.spread, synthetic_data_file_from_root, args.seed, return_centers=True
    )
    
    # Generate synthetic to-classify data (half as many vertices) using the same centers
    num_to_classify = args.num_vertices // 2
    to_classify_data = generate_data.generate_to_classify_data(
        NUM_CLASSES, NUM_DIMENSIONS, centers, num_to_classify, args.to_classify_spread, vertices_to_classify_from_root, args.seed
    )
    
    # Create the figure with three subplots
    fig, axes = plt.subplots(1, 4, figsize=(14, 6))
    
    # Left plot: original training dataset
    plots.plot_synthetic_data(axes[0], training_data, title="Training Data")
    
    # Run Gabriel Graph executable on training data
    gabriel_input_file = path_from_gabriel_to_root + synthetic_data_file_from_root
    run_command([gabriel_exe, gabriel_input_file], gabriel_cwd)
    
    # Run Low Degree Filter executable
    filter_input_file = path_from_filter_to_root + gabriel_output_from_root
    run_command([filter_exe, filter_input_file, str(args.threshold_factor)], filter_cwd)
    
    # Recompute Gabriel Graph on the filtered data
    run_command([gabriel_exe, path_from_gabriel_to_root + filter_output_from_root], gabriel_cwd)
    
    # Parse and plot the filtered graph on the midle subplot
    if not pathlib.Path(gabriel_filtered_output_from_root).exists():
        print(f"Error: Expected Low Degree Filter output not found at {gabriel_filtered_output_from_root}")
    
    # Run hyperplanes executable on the filtered Gabriel Graph
    run_command([hyperplanes_exe, path_from_hyperplanes_to_root + gabriel_filtered_output_from_root], hyperplanes_cwd)
    
    # Run chip-clas executable on the to-classify data
    run_command(
        [chip_clas_exe,
         path_from_chip_clas_to_root + gabriel_filtered_output_from_root,
         path_from_chip_clas_to_root + hyperplanes_output_from_root,
         path_from_chip_clas_to_root + vertices_to_classify_from_root],
        chip_clas_cwd
    )
    
    # Parse chip-clas output and merge classification with to-classify data
    classifications = parse_files.parse_clas_file(chip_clas_output_from_root)

    to_classify_classes = np.array([classifications.get(i + (1 - num_to_classify), 0) for i in range(len(to_classify_data))])
    
    plots.plot_classified(axes[1], to_classify_data, to_classify_classes, title="CHIP-clas")

    # Run rchip-clas executable on the to-classify data
    run_command(
        [rchip_clas_exe,
         path_from_rchip_clas_to_root + gabriel_filtered_output_from_root,
         path_from_rchip_clas_to_root + hyperplanes_output_from_root,
         path_from_rchip_clas_to_root + vertices_to_classify_from_root],
        rchip_clas_cwd
    )

    # Parse rchip-clas output and merge classification with to-classify data
    classifications = parse_files.parse_clas_file(rchip_clas_output_from_root)

    to_classify_classes = np.array([classifications.get(i + (1 - num_to_classify), 0) for i in range(len(to_classify_data))])

    plots.plot_classified(axes[2], to_classify_data, to_classify_classes, title="RCHIP-clas")

    # Run nn-clas executable on the to-classify data
    run_command(
        [nn_clas_exe,
         path_from_nn_clas_to_root + gabriel_filtered_output_from_root,
         path_from_nn_clas_to_root + vertices_to_classify_from_root],
        nn_clas_cwd
    )

    # Parse nn-clas output and merge classification with to-classify data
    classifications = parse_files.parse_clas_file(nn_clas_output_from_root)

    to_classify_classes = np.array([classifications.get(i + (1 - num_to_classify), 0) for i in range(len(to_classify_data))])

    plots.plot_classified(axes[3], to_classify_data, to_classify_classes, title="NN-clas")

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    main()
