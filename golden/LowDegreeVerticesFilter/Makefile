# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra

# Debugging flags
DEBUGFLAGS = -g

# Target executable
TARGET = lowDegreeVerticesFilter

# Source files
SRCS = lowDegreeVerticesFilter.cpp graphTypes.cpp stringHelpers.cpp readFiles.cpp buildGraph.cpp filter.cpp writeFiles.cpp

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

# Debugging target
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(TARGET)

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJS)

# Run the target executable

run_iris:
	./$(TARGET) ../../data/iris/iris-features.csv ../../data/iris/iris-targets.csv ../GabrielGraph/output/iris-edges.csv

run_ilpd:
	./$(TARGET) ../../data/ilpd/ILPD-features.csv ../../data/ilpd/ILPD-targets.csv ../GabrielGraph/output/ILPD-edges.csv

.PHONY: all clean run_iris run_ilpd