### Compilation/Linking Tools and Flags ###

CXX = g++
CXXFLAGS = -std=c++1y -Wall -Werror -g -O0
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

PROJ_NAME = ggl
PROJ_EXE = $(BIN_DIR)/$(PROJ_NAME)

TEST_NAME = ggl.test
TEST_EXE = $(BIN_DIR)/$(TEST_NAME)
TEST_LIB = $(LIB_DIR)/catch.hpp

### Project Build Rules and Procedures ###

.PHONY : clean

all : $(PROJ_EXE)

$(PROJ_NAME) : $(PROJ_EXE)
$(PROJ_EXE) : $(SRC_DIR)/main.cpp $(SRC_FILES) $(HDR_FILES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< -o $@

$(TEST_NAME) : $(TEST_EXE)
$(TEST_EXE) : $(TEST_FILES) $(SRC_FILES) $(HDR_FILES) $(TEST_LIB)
	$(GENSUITE) -o $(SUITE_SRC_FILE) $(foreach tf,$<,$(basename $(notdir $(tf))))
	$(CXX) $(CXXFLAGS) $(TESTINCLS) $(SUITE_SRC_FILE) -o $@

$(BIN_DIR)/%.test : $(TEST_DIR)/%.cpp $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h $(TEST_LIB)
	$(GENSUITE) -o $(SUITE_SRC_FILE) $(basename $(notdir $<))
	$(CXX) $(CXXFLAGS) $(TESTINCLS) $(SUITE_SRC_FILE) -o $@

$(BIN_DIR)/%.o : $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CXX) $(CXXFLAGS) $(CXXINCLS) $< -o $(BIN_DIR)/$@

$(TEST_LIB) :
	mkdir -p $(LIB_DIR)
	wget https://raw.githubusercontent.com/philsquared/Catch/master/single_include/catch.hpp -O $(TEST_LIB)

clean : 
	rm -rf $(BIN_DIR)/* $(ETC_DIR)/*.cpp
