### Compilation/Linking Tools and Flags ###

CXX = clang++
CXXFLAGS = -std=c++11 -Wall -Werror -g -O0
CXXINCLS = 
CXXTESTINCLS = -I$(LIB_DIR)
LDFLAGS = 

### Project Files and Directories ###

PROJ_DIR = .
BIN_DIR = $(PROJ_DIR)/bin
SRC_DIR = $(PROJ_DIR)/src
LIB_DIR = $(PROJ_DIR)/lib
TEST_DIR = $(PROJ_DIR)/test

PROJ_EXE = $(BIN_DIR)/ggl
TEST_LIB = $(LIB_DIR)/catch.hpp

### Project Build Rules and Procedures ###

.PHONY : clean

all : $(PROJ_EXE)

$(PROJ_EXE) : $(SRC_DIR)/main.cpp
	$(CXX) $(LDFLAGS) $< -o $(PROJ_EXE)

%.test : $(TEST_DIR)/%.cpp $(TEST_LIB)
	$(CXX) $(CXXFLAGS) $(CXXINCLS) $(CXXTESTINCLS) $< -o $(BIN_DIR)/$@

%.o : $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CXX) $(CXXFLAGS) $(CXXINCLS) $< -o $(BIN_DIR)/$@

$(TEST_LIB) :
	wget https://raw.githubusercontent.com/philsquared/Catch/master/single_include/catch.hpp -O $(TEST_LIB)

clean : 
	rm -rf $(BIN_DIR)/*
