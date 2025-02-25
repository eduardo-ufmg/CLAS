include ../../common_mk/flags.mk

# Target executable
TARGET := gabrielGraph

# Source files
SRCS := gabrielGraph.cpp ../Common/stringHelpers.cpp ../Common/writeFiles.cpp ../Common/printLoadedData.cpp ../Common/readFiles.cpp ../Common/graphTypes.cpp ../Common/squaredDistance.cpp computeGabrielGraph.cpp

# Include directories
INCS := -I../Common

include ../../common_mk/files.mk

include ../../common_mk/rules.mk

include ../../common_mk/targets.mk

# Run the target executable
run_iris: $(TARGET)
	./$(TARGET) ../../data/iris/iris.csv

run_ilpd: $(TARGET)
	./$(TARGET) ../../data/ilpd/ilpd.csv

run_synthetic: $(TARGET)
	./$(TARGET) ../../data/synthetic/synthetic.csv

run_filtered_iris: $(TARGET)
	./$(TARGET) ../Filter/output/iris-filtered.csv

run_filtered_ilpd: $(TARGET)
	./$(TARGET) ../Filter/output/ilpd-filtered.csv

run_filtered_synthetic: $(TARGET)
	./$(TARGET) ../Filter/output/synthetic-filtered.csv

.PHONY: run_iris run_ilpd run_synthetic run_filtered_iris run_filtered_ilpd run_filtered_synthetic
