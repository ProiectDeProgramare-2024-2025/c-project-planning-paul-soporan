# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++23 -Iinclude

# Find all source files recursively
SRC = $(shell find src -type f \( -name "*.cpp" -o -name "*.cc" \))

# Replace src/ with build/ and change extensions to .o
OBJ = $(patsubst src/%, build/%, $(SRC))
OBJ := $(OBJ:.cpp=.o)
OBJ := $(OBJ:.cc=.o)

# Output binary
TARGET = dental_office_appointment_system

# Default target
all: $(TARGET)

# Link all object files
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile .cpp and .cc to .o inside build/
build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/%.o: src/%.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -rf build $(TARGET)

.PHONY: all clean
