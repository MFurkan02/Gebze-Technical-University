# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11

# Target executable name
TARGET = makeFileSystem fileSystemOper

# Source files
SRCS = makeFileSystem.cpp fileSystemOper.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)
	@echo "Build complete: $(TARGET)"

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled: $<"

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Cleaned up build artifacts"

# Phony targets
.PHONY: all clean
