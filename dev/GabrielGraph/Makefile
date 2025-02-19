# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra

# Debugging flags
DEBUGFLAGS = -g

# Target executable
TARGET = gabrielGraph

# Source files
SRCS = gabrielGraph.cpp ../Common/stringHelpers.cpp ../Common/writeFiles.cpp ../Common/printLoadedData.cpp ../Common/readFiles.cpp ../Common/graphTypes.cpp computeGabrielGraph.cpp

# Include directories
INCS = -I../Common

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $^

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -c $< -o $@

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(TARGET)

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJS)

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

.PHONY: all clean run_iris run_ilpd run_synthetic run_filtered_iris run_filtered_ilpd run_filtered_synthetic