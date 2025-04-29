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

.PHONY: all build clean debug release run

all: build $(BIN_DIR)/$(TARGET)

# Compilar cada arquivo .cpp em .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

# Linkar os objetos e gerar o binário final
$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

# Diretórios necessários
build:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# Compilação com debug
debug: CXXFLAGS += -g -DDEBUG
debug: all

# Compilação otimizada
release: CXXFLAGS += -O3
release: all

# Limpar build
clean:
	@rm -rf $(BUILD)

# Executar o programa
run: all
	@./$(BIN_DIR)/$(TARGET)
