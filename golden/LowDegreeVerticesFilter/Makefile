# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra

# Debugging flags
DEBUGFLAGS = -g

# Target executable
TARGET = lowDegreeVerticesFilter

# Source files
SRCS = lowDegreeVerticesFilter.cpp ../Common/graphTypes.cpp ../Common/stringHelpers.cpp ../Common/writeFiles.cpp readFiles.cpp

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

# Debugging target
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(TARGET)

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJS)

# Run the target executable

run_iris:
	./$(TARGET) ../GabrielGraph/output/iris.csv

run_ilpd:
	./$(TARGET) ../GabrielGraph/output/ilpd.csv

.PHONY: all clean run_iris run_ilpd