### Compilation/Linking Tools and Flags ###

CXX = g++
CXXFLAGS = -std=c++1y -Wall -Werror -g -O0
CXXINCLS = -I$(SRC_DIR)
TESTINCLS = -I$(PROJ_DIR) -I$(LIB_DIR)
LDFLAGS =

### Project Directories ###

PROJ_DIR = .
BIN_DIR = $(PROJ_DIR)/bin
ETC_DIR = $(PROJ_DIR)/etc
SRC_DIR = $(PROJ_DIR)/src
LIB_DIR = $(PROJ_DIR)/lib
TEST_DIR = $(PROJ_DIR)/test

### Project Files ###

SRC_CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)
SRC_H_FILES = $(wildcard $(SRC_DIR)/*.h)
SRC_HPP_FILES = $(wildcard $(SRC_DIR)/*.hpp)
SRC_CONFIG_FILES = $(SRC_DIR)/consts.hpp

SRC_FILES = $(SRC_CPP_FILES) $(SRC_H_FILES) $(SRC_HPP_FILES) $(SRC_CONFIG_FILES)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.h,$(BIN_DIR)/%.o,$(SRC_H_FILES))
TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)

GENSUITE = $(ETC_DIR)/gensuite.py
SUITE_SRC_FILE = $(ETC_DIR)/suite.cpp

### Generated Files or Directories ###

PROJ_NAME = ggl
PROJ_EXE = $(BIN_DIR)/$(PROJ_NAME)

TEST_NAME = ggl.test
TEST_EXE = $(BIN_DIR)/$(TEST_NAME)
TEST_LIB = $(LIB_DIR)/catch.hpp

### Project Build Rules and Procedures ###

.PHONY : clean

all : $(PROJ_EXE)

$(PROJ_NAME) : $(PROJ_EXE)
$(PROJ_EXE) : $(SRC_DIR)/main.cpp $(SRC_FILES) $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ_FILES) $< -o $@

$(TEST_NAME) : $(TEST_EXE)
$(TEST_EXE) : $(TEST_FILES) $(SRC_FILES) $(OBJ_FILES) $(TEST_LIB)
	$(GENSUITE) -o $(SUITE_SRC_FILE) $(foreach tf,$<,$(basename $(notdir $(tf))))
	$(CXX) $(CXXFLAGS) $(TESTINCLS) $(OBJ_FILES) $(SUITE_SRC_FILE) -o $@

$(BIN_DIR)/%.test : $(TEST_FILES) $(SRC_FILES) $(OBJ_FILES) $(TEST_LIB)
	$(GENSUITE) -o $(SUITE_SRC_FILE) $(basename $(notdir $<))
	$(CXX) $(CXXFLAGS) $(TESTINCLS) $(OBJ_FILES) $(SUITE_SRC_FILE) -o $@

$(BIN_DIR)/%.o : $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h $(SRC_CONFIG_FILES)
	$(CXX) $(CXXFLAGS) $(CXXINCLS) $< -c -o $@

$(TEST_LIB) :
	mkdir -p $(LIB_DIR)
	wget https://raw.githubusercontent.com/philsquared/Catch/master/single_include/catch.hpp -O $(TEST_LIB)

clean : 
	rm -rf $(BIN_DIR)/* $(ETC_DIR)/*.cpp
