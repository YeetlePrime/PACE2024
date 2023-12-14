CXX = g++
CXXFLAGS = -std=c++17 -fdiagnostics-color=always -Wall -Wpedantic -g
INC_DIRS = -Isrc -Ilib

SRC_DIR = src
LIB_DIR = lib

HEADERS = $(wildcard $(SRC_DIR)/*.hpp)
LIBRARIES = $(wildcard $(LIB_DIR)/*.cpp)

# List all your source files in the src directory
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Generate the object files corresponding to the source files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(SRC_DIR)/%.o)

all: main.exe

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c -o $@ $<

$(LIB_DIR)/%.o: $(LIB_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c -o $@ $<

main.exe: $(OBJECTS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LIBRARIES)

clean:
	rm -f $(SRC_DIR)/*.o $(LIB_DIR)/*.o main.exe

run: all
	./main.exe "ressources/tiny_test_set/complete_4_5.gr"

.PHONY: all clean