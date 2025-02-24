# Default target
all: $(TARGET)

# Debugging target
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(TARGET)

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJS) $(DEPFILES)

# Compile-only target
compile: $(OBJS)

.PHONY: all clean debug compile
