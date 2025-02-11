# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra

# Target executable
TARGET = LowDegreeVerticesFilter

# Source files
SRCS = LowDegreeVerticesFilter.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJS)

# Run the target executable

run_iris:
	./$(TARGET) ../../data/iris/iris-features.csv ../../data/iris/iris-targets.csv ../GabrielGraph/output/iris-features_edges.csv

run_ilpd:
	./$(TARGET) ../../data/ilpd/ILPD-features.csv ../../data/ilpd/ILPD-targets.csv ../GabrielGraph/output/ILPD-features_edges.csv

.PHONY: all clean run_iris run_ilpd