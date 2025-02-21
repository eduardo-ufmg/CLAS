# Define file paths (relative to project root)
synthetic_data_file_from_root = "data/synthetic/synthetic.csv"
vertices_to_classify_from_root = "data/synthetic/to-classify.csv"

# Gabriel Graph parameters
gabriel_graph_exe_from_root = "cpp/GabrielGraph/gabrielGraph"
gabriel_output_from_root = "cpp/GabrielGraph/output/synthetic.csv"
gabriel_filtered_output_from_root = "cpp/GabrielGraph/output/synthetic-filtered.csv"
path_from_gabriel_to_root = "../../"
gabriel_cwd = "cpp/GabrielGraph"
gabriel_exe = "./gabrielGraph"

# Low Degree Filter parameters
filter_exe_from_root = "cpp/Filter/filter"
filter_output_from_root = "cpp/Filter/output/synthetic-filtered.csv"
path_from_filter_to_root = "../../"
filter_cwd = "cpp/Filter"
filter_exe = "./filter"

# Hyperplanes parameters
hyperplanes_from_root = "cpp/classifiers/Common/hyperplanes"
hyperplanes_output_from_root = "cpp/classifiers/Common/output/synthetic-filtered.csv"
path_from_hyperplanes_to_root = "../../../"
hyperplanes_cwd = "cpp/classifiers/Common"
hyperplanes_exe = "./hyperplanes"

# CHIP-clas parameters
chip_clas_from_root = "cpp/classifiers/chip-clas/chip-clas"
chip_clas_output_from_root = "cpp/classifiers/chip-clas/output/synthetic-classified.csv"
path_from_chip_clas_to_root = "../../../"
chip_clas_cwd = "cpp/classifiers/chip-clas"
chip_clas_exe = "./chip-clas"

# RCHIP-clas parameters
rchip_clas_from_root = "cpp/classifiers/rchip-clas/rchip-clas"
rchip_clas_output_from_root = "cpp/classifiers/rchip-clas/output/synthetic-classified.csv"
path_from_rchip_clas_to_root = "../../../"
rchip_clas_cwd = "cpp/classifiers/rchip-clas"
rchip_clas_exe = "./rchip-clas"