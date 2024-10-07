# Compiler
CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -I ../../include/
LDFLAGS = -lgtest -lgtest_main -lpthread

# Directories
SRC_DIR = ../../src
TEST_DIR = tests
OBJ_DIR = obj

# Source files and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
TEST_FILES = $(wildcard $(TEST_DIR)/*.cc)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Executable names
EXEC_MAIN = main_program
EXEC_TEST = test_runner

# Target to build the main program executable
all: $(EXEC_MAIN)

# Target to build the test runner executable
test: $(EXEC_TEST)

# Rule to link object files and create the main program executable
$(EXEC_MAIN): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $@

# Rule to link object files and create the test runner executable
$(EXEC_TEST): $(OBJ_FILES) $(TEST_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) $(TEST_FILES) $(LDFLAGS) -o $@

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJ_DIR)/*.o $(EXEC_MAIN) $(EXEC_TEST)

.PHONY: all test clean
