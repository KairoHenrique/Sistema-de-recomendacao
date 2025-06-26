CXX := g++
CXXFLAGS := -Wall -Wextra -Werror -std=c++17 -O3 -march=native -flto -ffast-math
LDFLAGS := -lm -pthread -flto
SRC_DIR := src
INC_DIR := include
BUILD := build
OBJ_DIR := $(BUILD)/objects
BIN_DIR := $(BUILD)
TARGET := app
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
INCLUDE := -I$(INC_DIR)

.PHONY: all clean build run

all: clean build run

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

build: $(BIN_DIR)/$(TARGET)

run: build
	@mkdir -p dados resultados
	./$(BIN_DIR)/$(TARGET)

clean:
	@rm -rf $(BUILD)
