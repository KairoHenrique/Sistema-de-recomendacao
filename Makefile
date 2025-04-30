# Compilador e flags
CXX      := g++
CXXFLAGS := -Wall -Wextra -Werror -std=c++17
LDFLAGS  := -lm

# Estrutura de pastas
SRC_DIR  := src
INC_DIR  := include
BUILD    := build
OBJ_DIR  := $(BUILD)/objects
BIN_DIR  := $(BUILD)
TARGET   := app

# Arquivos fonte e objetos
SRC      := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS  := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
INCLUDE  := -I$(INC_DIR)

# Alvo principal: executa clean + run automaticamente – Kairo
.PHONY: all clean build run debug release

all: clean run

# Compilacao
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(OBJECTS) -o $@ $(LDFLAGS)

build: $(BIN_DIR)/$(TARGET)

# Executa o programa
run: build
	./$(BIN_DIR)/$(TARGET)

# Compilacao com debug
debug: CXXFLAGS += -g -DDEBUG
debug: all

# Compilacao otimizada
release: CXXFLAGS += -O3
release: all

# Limpeza
clean:
	@rm -rf $(BUILD)
