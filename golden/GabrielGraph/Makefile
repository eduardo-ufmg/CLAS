# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra

# Target executable
TARGET = multiVariableGabrielGraph

# Source files
SRCS = multiVariableGabrielGraph.cpp gabrielGraph.cpp squaredDistance.cpp

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

run_iris: $(TARGET)
	./$(TARGET) ../../data/iris/iris-features.csv

run_ilpd: $(TARGET)
	./$(TARGET) ../../data/ilpd/ILPD-features.csv

.PHONY: all clean run_iris run_ilpd