# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++23 -Iinclude

# === Core (shared) source files — excluding executables
CORE_SRC = $(shell find src -type f \( -name "*.cpp" -o -name "*.cc" \) ! -path "src/executables/*")
CORE_OBJ = $(patsubst src/%, build/%, $(CORE_SRC))
CORE_OBJ := $(CORE_OBJ:.cpp=.o)
CORE_OBJ := $(CORE_OBJ:.cc=.o)

# === Executables: source files in executables/
EXEC_SRC = $(shell find src/executables -maxdepth 1 -type f \( -name "*.cpp" -o -name "*.cc" \))
EXEC_NAMES = $(notdir $(EXEC_SRC))
EXEC_NAMES := $(EXEC_NAMES:.cpp=)
EXEC_NAMES := $(EXEC_NAMES:.cc=)
EXECUTABLES = $(addprefix bin/, $(EXEC_NAMES))

# === Default target
all: $(EXECUTABLES)

# === Rule: compile core files to .o in build/
build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/%.o: src/%.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# === Rule: link each executable with core object files
bin/%: src/executables/%.cpp $(CORE_OBJ)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@

bin/%: src/executables/%.cc $(CORE_OBJ)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@

# === Clean
clean:
	rm -rf build bin

.PHONY: all clean
