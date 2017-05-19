
### Compilation/Linking Tools and Flags ###

CXX = clang++
CXX_FLAGS = -std=c++1y -Wall -g -O0
# CXX = g++
# CXX_FLAGS = -std=c++1y -Wall -Werror -g -O0
CXX_INCLS = -I$(SRC_DIR)
CXX_TINCLS = $(CXX_INCLS) -I$(OPT_DIR)

CXX_LIB_FLAGS = `pkg-config --cflags glfw3`
CXX_LIB_INCLS = `pkg-config --static --libs glfw3`

### Project Directories ###

PROJ_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
BIN_DIR = $(PROJ_DIR)/bin
BUILD_DIR = $(PROJ_DIR)/build
OBJ_DIR = $(PROJ_DIR)/obj
ETC_DIR = $(PROJ_DIR)/etc
SRC_DIR = $(PROJ_DIR)/src
OPT_DIR = $(PROJ_DIR)/opt
EX_DIR = $(SRC_DIR)/ex
TEST_DIR = $(PROJ_DIR)/test

### Project Inclusions ###

include $(shell find $(BUILD_DIR) -name '*.mk')

### Project Files ###

EX_CPP_FILES = $(shell find $(EX_DIR) -name '*.cpp')
EX_EXE_FILES = $(call outputs,$(EX_CPP_FILES),$(BIN_DIR),.ex)

SRC_GLOBAL_FILES = $(SRC_DIR)/consts.hpp
SRC_CPP_FILES = $(filter-out $(EX_CPP_FILES),$(shell find $(SRC_DIR) -name '*.cpp'))
SRC_H_FILES = $(shell find $(SRC_DIR) -name '*.h')
SRC_HPP_FILES = $(shell find $(SRC_DIR) -name '*.hpp')
SRC_ALL_FILES = $(shell find $(SRC_DIR) -type f)
SRC_OBJ_FILES = $(call outputs,$(SRC_H_FILES),$(OBJ_DIR),.o)

TEST_GLOBAL_FILES = $(OPT_DIR)/catch.hpp
TEST_CPP_FILES = $(filter-out $(TEST_DIR)/main.cpp,$(wildcard $(TEST_DIR)/*.cpp))
TEST_OBJ_FILES = $(call outputs,$(TEST_CPP_FILES),$(OBJ_DIR),.to)

### Generated Files or Directories ###

TEST_NAME = ggl.test
TEST_EXE = $(BIN_DIR)/$(TEST_NAME)

### Project Build Rules and Procedures ###

.PHONY : clean
.SECONDEXPANSION :

all : $(PROJ_EXE)

# $(TEST_NAME) : $(TEST_EXE)
$(TEST_EXE) : $(TEST_DIR)/main.cpp $(SRC_ALL_FILES) $(SRC_OBJ_FILES) $(TEST_OBJ_FILES) $(TEST_GLOBAL_FILES) | $(BIN_DIR)
	$(CXX) $(CXX_FLAGS) $(CXX_TINCLS) $(SRC_OBJ_FILES) $(TEST_OBJ_FILES) $< -o $@

$(EX_EXE_FILES) : $$(call inputs,$$@,$(SRC_DIR),.cpp) $(SRC_OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CXX_FLAGS) $(CXX_LIB_FLAGS) $(CXX_INCLS) $(SRC_OBJ_FILES) $< -o $@ $(CXX_LIB_INCLS)

$(TEST_OBJ_FILES) : $$(call inputs,$$@,$(TEST_DIR),.cpp) $$(call inputs,$$@,$(SRC_DIR),.c*) $$(call inputs,$$@,$(SRC_DIR),.h*) $(SRC_GLOBAL_FILES) $(TEST_GLOBAL_FILES) | $(OBJ_DIR)
	$(CXX) $(CXX_FLAGS) $(CXX_TINCLS) $< -c -o $@

$(SRC_OBJ_FILES) : $$(call inputs,$$@,$(SRC_DIR),.cpp) $$(call inputs,$$@,$(SRC_DIR),.h) $(SRC_GLOBAL_FILES) | $(OBJ_DIR)
	$(CXX) $(CXX_FLAGS) $(CXX_INCLS) $< -c -o $@

$(OPT_DIR)/catch.hpp : | $(OPT_DIR)
	wget https://raw.githubusercontent.com/philsquared/Catch/master/single_include/catch.hpp -O $(TEST_LIB)

$(BIN_DIR) $(OBJ_DIR) $(OPT_DIR) :
	mkdir -p $@

clean :
	rm -rf $(BIN_DIR)/* $(OBJ_DIR)
