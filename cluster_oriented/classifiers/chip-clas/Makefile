include ../../../common_mk/flags.mk

# Target executable
TARGET := chip-clas

# Source files
SRCS := chip-clas.cpp ../../Common/stringHelpers.cpp ../../Common/writeFiles.cpp ../../Common/printLoadedData.cpp ../../Common/readFiles.cpp ../../Common/graphTypes.cpp ../../Common/squaredDistance.cpp label.cpp

# Include directories
INCS := -I../../Common -I../Common

include ../../../common_mk/files.mk

include ../../../common_mk/rules.mk

include ../../../common_mk/targets.mk

# Run the target executable
run_iris: $(TARGET)
	./$(TARGET) ../../GabrielGraph/output/iris-filtered.csv ../Common/output/iris-filtered.csv ../../../data/iris/to-label.csv

run_ilpd: $(TARGET)
	./$(TARGET) ../../GabrielGraph/output/ilpd-filtered.csv ../Common/output/ilpd-filtered.csv ../../../data/ilpd/to-label.csv

run_synthetic: $(TARGET)
	./$(TARGET) ../../GabrielGraph/output/synthetic-filtered.csv ../Common/output/synthetic-filtered.csv ../../../data/synthetic/to-label.csv

.PHONY: run_iris run_ilpd run_synthetic