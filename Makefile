CXX      := g++
CXXFLAGS := -Wall -Wextra -Werror -std=c++17
LDFLAGS  := -lm
SRC_DIR  := src
INC_DIR  := include
BUILD    := build
OBJ_DIR  := $(BUILD)/objects
BIN_DIR  := $(BUILD)
TARGET   := app
SRC      := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS  := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
INCLUDE  := -I$(INC_DIR)
.PHONY: all clean build run debug release massif

all: clean run

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@
$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

build: $(BIN_DIR)/$(TARGET)

run: build
	./$(BIN_DIR)/$(TARGET)

debug: CXXFLAGS += -g -DDEBUG
debug: all

release: CXXFLAGS += -O3 -g
release: all

massif: release
	valgrind --tool=massif --massif-out-file=massif.out ./$(BIN_DIR)/$(TARGET)
	@echo "Arquivo gerado: massif.out (use ms_print para analisar)"

clean:
	@rm -rf $(BUILD)
