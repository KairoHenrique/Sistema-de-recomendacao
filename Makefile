MAKEFLAGS += -j$(nproc)

CXX := g++
CXXFLAGS := -Wall -Wextra -Werror -std=c++20 -O3 -march=native -flto -ffast-math -fno-exceptions -fno-rtti
LDFLAGS := -lm -pthread -flto

SRC_DIR := src
INC_DIR := include
BUILD := build
OBJ_DIR := $(BUILD)/objects
BIN_DIR := $(BUILD)

TARGET := app

SRC_FILES := \
	Configuracao.cpp \
	Filme.cpp \
	GerenciadorDeDados.cpp \
	main.cpp \
	Preprocessador.cpp \
	Recomendador.cpp \
	Usuario.cpp \
	CalculadorDeSimilaridade.cpp

SRC := $(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

INCLUDE := -I$(INC_DIR)

.PHONY: all clean build run

all: build

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

build: $(BIN_DIR)/$(TARGET)

run:
	@mkdir -p dados resultados
	./$(BIN_DIR)/$(TARGET)

clean:
	@rm -rf $(BUILD)


