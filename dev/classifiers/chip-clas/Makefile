# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra

# Debugging flags
DEBUGFLAGS = -g

# Target executable
TARGET = chip-clas

# Source files
SRCS = chip-clas.cpp ../../Common/stringHelpers.cpp ../../Common/writeFiles.cpp ../../Common/printLoadedData.cpp ../../Common/readGraph.cpp ../Common/computeExperts.cpp

# Include directories
INCS = -I../../Common -I../Common

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
	./$(TARGET) ../../GabrielGraph/output/iris.csv

run_ilpd: $(TARGET)
	./$(TARGET) ../../GabrielGraph/output/ilpd.csv

run_synthetic: $(TARGET)
	./$(TARGET) ../../GabrielGraph/output/synthetic.csv

.PHONY: all clean run_iris run_ilpd run_synthetic