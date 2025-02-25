include ../../common_mk/flags.mk

# Target executable
TARGET = filter

# Source files
SRCS = filter.cpp ../Common/stringHelpers.cpp ../Common/writeFiles.cpp ../Common/printLoadedData.cpp ../Common/readFiles.cpp ../Common/graphTypes.cpp ../Common/squaredDistance.cpp computeFilter.cpp

# Include directories
INCS = -I../Common

include ../../common_mk/files.mk

include ../../common_mk/rules.mk

include ../../common_mk/targets.mk

# Run the target executable
run_iris:
	./$(TARGET) ../GabrielGraph/output/iris.csv

run_ilpd:
	./$(TARGET) ../GabrielGraph/output/ilpd.csv

run_synthetic:
	./$(TARGET) ../GabrielGraph/output/synthetic.csv

.PHONY: run_iris run_ilpd run_synthetic