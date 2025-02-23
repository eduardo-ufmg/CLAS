# Define file paths (relative to project root)
synthetic_data_file_from_root = "data/synthetic/synthetic.csv"
vertices_to_classify_from_root = "data/synthetic/to-classify.csv"

# Gabriel Graph parameters
gabriel_graph_exe_from_root = "cluster_oriented/GabrielGraph/gabrielGraph"
gabriel_output_from_root = "cluster_oriented/GabrielGraph/output/synthetic.csv"
gabriel_filtered_output_from_root = "cluster_oriented/GabrielGraph/output/synthetic-filtered.csv"
path_from_gabriel_to_root = "../../"
gabriel_cwd = "cluster_oriented/GabrielGraph"
gabriel_exe = "./gabrielGraph"

# Low Degree Filter parameters
filter_exe_from_root = "cluster_oriented/Filter/filter"
filter_output_from_root = "cluster_oriented/Filter/output/synthetic-filtered.csv"
path_from_filter_to_root = "../../"
filter_cwd = "cluster_oriented/Filter"
filter_exe = "./filter"

# Hyperplanes parameters
hyperplanes_from_root = "cluster_oriented/classifiers/Common/hyperplanes"
hyperplanes_output_from_root = "cluster_oriented/classifiers/Common/output/synthetic-filtered.csv"
path_from_hyperplanes_to_root = "../../../"
hyperplanes_cwd = "cluster_oriented/classifiers/Common"
hyperplanes_exe = "./hyperplanes"

# CHIP-clas parameters
chip_clas_from_root = "cluster_oriented/classifiers/chip-clas/chip-clas"
chip_clas_output_from_root = "cluster_oriented/classifiers/chip-clas/output/synthetic-classified.csv"
path_from_chip_clas_to_root = "../../../"
chip_clas_cwd = "cluster_oriented/classifiers/chip-clas"
chip_clas_exe = "./chip-clas"

# RCHIP-clas parameters
rchip_clas_from_root = "cluster_oriented/classifiers/rchip-clas/rchip-clas"
rchip_clas_output_from_root = "cluster_oriented/classifiers/rchip-clas/output/synthetic-classified.csv"
path_from_rchip_clas_to_root = "../../../"
rchip_clas_cwd = "cluster_oriented/classifiers/rchip-clas"
rchip_clas_exe = "./rchip-clas"

# NN-clas parameters
nn_clas_from_root = "cluster_oriented/classifiers/nn-clas/nn-clas"
nn_clas_output_from_root = "cluster_oriented/classifiers/nn-clas/output/synthetic-classified.csv"
path_from_nn_clas_to_root = "../../../"
nn_clas_cwd = "cluster_oriented/classifiers/nn-clas"
nn_clas_exe = "./nn-clas"