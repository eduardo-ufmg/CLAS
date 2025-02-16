# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra

# Debugging flags
DEBUGFLAGS = -g

# Target executable
TARGET = multiVariableGabrielGraph

# Source files
SRCS = multiVariableGabrielGraph.cpp ../Common/stringHelpers.cpp ../Common/writeFiles.cpp ../Common/printLoadedData.cpp readGraph.cpp gabrielGraph.cpp

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

.PHONY: all clean run_iris run_ilpd