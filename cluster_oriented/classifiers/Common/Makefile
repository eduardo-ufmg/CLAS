# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++23 -Wall -Wextra

# Debugging flags
DEBUGFLAGS = -g

# Target executable
TARGET = hyperplanes

# Source files
SRCS = hyperplanes.cpp ../../Common/stringHelpers.cpp ../../Common/writeFiles.cpp ../../Common/printLoadedData.cpp ../../Common/readFiles.cpp ../../Common/graphTypes.cpp ../../Common/squaredDistance.cpp computeExperts.cpp

# Include directories
INCS = -I../../Common

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
	./$(TARGET) ../../GabrielGraph/output/iris-filtered.csv

run_ilpd: $(TARGET)
	./$(TARGET) ../../GabrielGraph/output/ilpd-filtered.csv

run_synthetic: $(TARGET)
	./$(TARGET) ../../GabrielGraph/output/synthetic-filtered.csv

.PHONY: all clean run_iris run_ilpd run_synthetic