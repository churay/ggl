### Compilation/Linking Tools and Flags ###

CXX = clang++
CXXFLAGS = -std=c++11 -Wall -Werror -g -O0
CXXINCLS = -I$(SRC_DIR)
TESTINCLS = -I$(PROJ_DIR) -I$(LIB_DIR)
LDFLAGS = 

### Project Files and Directories ###

PROJ_DIR = .
BIN_DIR = $(PROJ_DIR)/bin
ETC_DIR = $(PROJ_DIR)/etc
SRC_DIR = $(PROJ_DIR)/src
LIB_DIR = $(PROJ_DIR)/lib
TEST_DIR = $(PROJ_DIR)/test

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
HDR_FILES = $(wildcard $(SRC_DIR)/*.h)
TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)

GENSUITE = $(ETC_DIR)/gensuite.py
SUITE_SRC_FILE = $(ETC_DIR)/suite.cpp

### Generated Files or Directories ###

PROJ_EXE = $(BIN_DIR)/ggl
TEST_EXE = $(BIN_DIR)/ggl.test
TEST_LIB = $(LIB_DIR)/catch.hpp

### Project Build Rules and Procedures ###

.PHONY : clean

all : $(PROJ_EXE)

$(PROJ_EXE) : $(SRC_DIR)/main.cpp $(SRC_FILES) $(HDR_FILES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< -o $@

$(TEST_EXE) : $(TEST_FILES) $(SRC_FILES) $(HDR_FILES) $(TEST_LIB)
	$(GENSUITE) -o $(SUITE_SRC_FILE) $(foreach tf,$<,$(basename $(notdir tf)))
	$(CXX) $(CXXFLAGS) $(TESTINCLS) $(SUITE_SRC_FILE) -o $@

%.test : $(TEST_DIR)/%.cpp $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h $(TEST_LIB)
	$(GENSUITE) -o $(SUITE_SRC_FILE) $(basename $(notdir $<))
	$(CXX) $(CXXFLAGS) $(TESTINCLS) $(SUITE_SRC_FILE) -o $(BIN_DIR)/$@

%.o : $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CXX) $(CXXFLAGS) $(CXXINCLS) $< -o $(BIN_DIR)/$@

$(TEST_LIB) :
	mkdir -p $(LIB_DIR)
	wget https://raw.githubusercontent.com/philsquared/Catch/master/single_include/catch.hpp -O $(TEST_LIB)

clean : 
	rm -rf $(BIN_DIR)/* $(ETC_DIR)/*.cpp
