# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra

# Target executable
TARGET = MultiVariableGabrielGraph_golden

# Source files
SRCS = MultiVariableGabrielGraph_golden.cpp

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

# Run the program with an input CSV file
run: $(TARGET)
	./$(TARGET) input.csv

.PHONY: all clean run