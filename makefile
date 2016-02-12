### Compilation/Linking Tools and Flags ###

CXX = clang++
CXXFLAGS = -std=c++11 -Wall -Werror -g -O0
LDFLAGS = 

### Project Files and Directories ###

PROJ_DIR = .
BIN_DIR = $(PROJ_DIR)/bin
SRC_DIR = $(PROJ_DIR)/src
TEST_DIR = $(PROJ_DIR)/test

PROJ_EXE = $(BIN_DIR)/ggl

### Project Build Rules and Procedures ###

.PHONY : clean

all : $(PROJ_EXE)

$(PROJ_EXE) : $(SRC_DIR)/main.cpp
	$(CXX) $(LDFLAGS) $< -o $(PROJ_EXE)

%.o : $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CXX) $(CXXFLAGS) $< -o $(BIN_DIR)/$@

clean : 
	rm -rf $(BIN_DIR)/*
