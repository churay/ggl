### Compilation/Linking Tools and Flags ###

CXX = clang++
CXX_FLAGS = -std=c++1y -stdlib=libc++ -Wall -g -O0
# CXX = g++
# CXX_FLAGS = -std=c++1y -Wall -Werror -g -O0
CXX_INCLS = -I$(SRC_DIR)
CXX_TINCLS = -I$(PROJ_DIR) -I$(LIB_DIR)

CXX_LIB_FLAGS = `pkg-config --cflags glfw3`
CXX_LIB_INCLS = `pkg-config --static --libs glfw3`

### Project Directories ###

PROJ_DIR = .
BIN_DIR = $(PROJ_DIR)/bin
OBJ_DIR = $(PROJ_DIR)/obj
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
SRC_OBJ_FILES = $(patsubst $(SRC_DIR)/%.h,$(OBJ_DIR)/%.o,$(SRC_H_FILES))
TEST_FILES = $(filter-out $(TEST_DIR)/main.cpp,$(wildcard $(TEST_DIR)/*.cpp))
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/%.to,$(TEST_FILES))

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
$(PROJ_EXE) : $(SRC_DIR)/main.cpp $(SRC_FILES) $(SRC_OBJ_FILES)
	$(CXX) $(CXX_FLAGS) $(CXX_LIB_FLAGS) $(CXX_INCLS) $(SRC_OBJ_FILES) $< -o $@ $(CXX_LIB_INCLS)

$(TEST_NAME) : $(TEST_EXE)
$(TEST_EXE) : $(TEST_DIR)/main.cpp $(SRC_FILES) $(SRC_OBJ_FILES) $(TEST_OBJ_FILES) | $(TEST_LIB)
	$(CXX) $(CXX_FLAGS) $(CXX_TINCLS) $(SRC_OBJ_FILES) $(TEST_OBJ_FILES) $< -o $@

$(BIN_DIR)/%.ex : $(ETC_DIR)/%.cpp $(SRC_FILES) $(SRC_OBJ_FILES)
	$(CXX) $(CXX_FLAGS) $(CXX_LIB_FLAGS) $(CXX_INCLS) $(SRC_OBJ_FILES) $< -o $@ $(CXX_LIB_INCLS)

$(OBJ_DIR)/%.to : $(TEST_DIR)/%.cpp $(SRC_DIR)/%.cpp $(wildcard $(SRC_DIR)/%.h*) $(SRC_CONFIG_FILES) | $(TEST_LIB) $(OBJ_DIR)
	$(CXX) $(CXX_FLAGS) $(CXX_TINCLS) $< -c -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h $(SRC_CONFIG_FILES) | $(OBJ_DIR)
	$(CXX) $(CXX_FLAGS) $(CXX_INCLS) $< -c -o $@

$(TEST_LIB) :
	mkdir -p $(LIB_DIR)
	wget https://raw.githubusercontent.com/philsquared/Catch/master/single_include/catch.hpp -O $(TEST_LIB)

$(OBJ_DIR) :
	mkdir -p $(OBJ_DIR)

clean : 
	rm -rf $(BIN_DIR)/* $(OBJ_DIR)
