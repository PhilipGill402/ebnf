# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -g -Iinclude -DDEBUG -std=c++14

# Output binaries
TARGET = ebnf
TEST_TARGET = lexer 

# Sources
SRC := $(wildcard src/*.cpp)
APP_SRC := $(filter-out src/main.cpp,$(SRC))    # sources WITHOUT main
TEST_MAIN := tests/test.cpp

# Objects (built into build/)
APP_OBJ := $(patsubst src/%.cpp,build/%.o,$(APP_SRC))
MAIN_OBJ := build/main.o
TEST_MAIN_OBJ := build/test.o

# Default rule
all: $(TARGET)

# Build main app (includes src/main.cpp)
$(TARGET): $(APP_OBJ) $(MAIN_OBJ)
	$(CXX) $^ -o $@

# Build tests (excludes src/main.cpp, includes tests/test.cpp which has main)
test: $(TEST_TARGET)

$(TEST_TARGET): $(APP_OBJ) $(TEST_MAIN_OBJ)
	$(CXX) $^ -o $@

# Compile rules
build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/test.o: $(TEST_MAIN) | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure build directory exists
build:
	mkdir -p build

# Clean build artifacts
clean:
	rm -rf build $(TARGET) $(TEST_TARGET)

.PHONY: all test clean

