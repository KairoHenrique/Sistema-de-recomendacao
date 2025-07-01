# Compilador e flags
CXX := g++
CXXFLAGS := -Wall -Wextra -Werror -std=c++20 -O3 -march=native -flto -ffast-math -fno-exceptions -fno-rtti
LDFLAGS := -lm -pthread -flto

# Diretórios
SRC_DIR := src
INC_DIR := include
BUILD := build
OBJ_DIR := $(BUILD)/objects
BIN_DIR := $(BUILD)

# Binário final
TARGET := app

# --- MODIFICAÇÃO PRINCIPAL ---
# Lista explícita dos arquivos fonte que precisam ser compilados.
# Usuario.cpp e CalculadorDeSimilaridade.cpp foram removidos.
SRC_FILES := \
	Configuracao.cpp \
	Filme.cpp \
	GerenciadorDeDados.cpp \
	main.cpp \
	Preprocessador.cpp \
	Recomendador.cpp \
	utilitarios.cpp

# Adiciona o prefixo do diretório 'src' a cada arquivo da lista
SRC := $(addprefix $(SRC_DIR)/, $(SRC_FILES))

# Gera a lista de arquivos objeto (.o) a partir da lista de fontes (.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

# Adiciona o diretório 'include' para busca de cabeçalhos
INCLUDE := -I$(INC_DIR)

# Regras Padrão
.PHONY: all clean build run

# Regra principal: limpa, compila e executa
all: clean build run

# Regra para compilar um arquivo .cpp em um arquivo .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

# Regra para linkar os arquivos .o e criar o executável
$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

# Regra para forçar a compilação
build: $(BIN_DIR)/$(TARGET)

# Regra para executar o programa
run: build
	@mkdir -p dados resultados
	./$(BIN_DIR)/$(TARGET)

# Regra para limpar os arquivos gerados pela compilação
clean:
	@rm -rf $(BUILD)
