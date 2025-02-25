# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++23 -Wall -Wextra -Werror -pedantic -pedantic-errors

# Debugging flags
DEBUGFLAGS = -g

# Dependency flags
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

