include ../../../common_mk/flags.mk

# Target executable
TARGET = hyperplanes

# Source files
SRCS = hyperplanes.cpp ../../Common/stringHelpers.cpp ../../Common/writeFiles.cpp ../../Common/printLoadedData.cpp ../../Common/readFiles.cpp ../../Common/graphTypes.cpp ../../Common/squaredDistance.cpp computeExperts.cpp

# Include directories
INCS = -I../../Common

include ../../../common_mk/files.mk

include ../../../common_mk/rules.mk

include ../../../common_mk/targets.mk

# Run the target executable
run_iris: $(TARGET)
	./$(TARGET) ../../GabrielGraph/output/iris-filtered.csv

run_ilpd: $(TARGET)
	./$(TARGET) ../../GabrielGraph/output/ilpd-filtered.csv

run_synthetic: $(TARGET)
	./$(TARGET) ../../GabrielGraph/output/synthetic-filtered.csv

.PHONY: run_iris run_ilpd run_synthetic