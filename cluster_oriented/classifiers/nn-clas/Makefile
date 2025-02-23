# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++23 -Wall -Wextra

# Debugging flags
DEBUGFLAGS = -g

# Target executable
TARGET = nn-clas

# Source files
SRCS = nn-clas.cpp ../../Common/stringHelpers.cpp ../../Common/writeFiles.cpp ../../Common/printLoadedData.cpp ../../Common/readFiles.cpp ../../Common/graphTypes.cpp ../../Common/squaredDistance.cpp getSEs.cpp classify.cpp

# Include directories
INCS = -I../../Common -I../Common

# Object files
OBJS = $(SRCS:.cpp=.o)

DEPDIR = .deps
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $^

# Include dependency files
$(DEPDIR):
	@mkdir -p $@

# Compile source files into object files
%.o : %.cpp
%.o: %.cpp $(DEPDIR)/%.d | $(DEPDIR)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INCS) -c $< -o $@

DEPFILES := $(SRCS:%.cpp=$(DEPDIR)/%.d)
$(DEPFILES):

include $(wildcard $(DEPFILES))

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(TARGET)

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJS)

# Run the target executable

run_iris: $(TARGET)
	./$(TARGET) ../../GabrielGraph/output/iris-filtered.csv ../Common/output/iris-filtered.csv ../../../data/iris/to-classify.csv

run_ilpd: $(TARGET)
	./$(TARGET) ../../GabrielGraph/output/ilpd-filtered.csv ../Common/output/ilpd-filtered.csv ../../../data/ilpd/to-classify.csv

run_synthetic: $(TARGET)
	./$(TARGET) ../../GabrielGraph/output/synthetic-filtered.csv ../Common/output/synthetic-filtered.csv ../../../data/synthetic/to-classify.csv

.PHONY: all clean run_iris run_ilpd run_synthetic