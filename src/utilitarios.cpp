#include "utilitarios.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

// Função para ler o conteúdo completo de um arquivo para uma string.
// caminho: Caminho para o arquivo a ser lido.
// Retorna uma string contendo todo o conteúdo do arquivo.
// Em caso de erro na abertura ou leitura, imprime uma mensagem de erro fatal e encerra o programa.
std::string lerArquivoInteiro(const std::string& caminho) {
    // Abre o arquivo em modo binário e posiciona o ponteiro no final para obter o tamanho.
    std::ifstream arquivo(caminho, std::ios::binary | std::ios::ate);
    if (!arquivo) {
        std::cerr << "Erro fatal: nao foi possivel abrir o arquivo: " << caminho << std::endl;
        exit(1);
    }
    // Obtém o tamanho do arquivo.
    std::streamsize tamanho = arquivo.tellg();
    // Retorna o ponteiro para o início do arquivo.
    arquivo.seekg(0, std::ios::beg);
    // Cria um buffer (string) com o tamanho do arquivo.
    std::string buffer(tamanho, '\0');
    // Lê o conteúdo do arquivo para o buffer.
    if (!arquivo.read(buffer.data(), tamanho)) {
        std::cerr << "Erro fatal: nao foi possivel ler o arquivo: " << caminho << std::endl;
        exit(1);
    }
    return buffer;
}

// Função para dividir uma string em tokens com base em um delimitador.
// linha: A string a ser dividida.
// Retorna um vetor de strings contendo os tokens.
std::vector<std::string> dividir(const std::string& linha, char delimitador) {
    std::vector<std::string> tokens;
    std::stringstream ss(linha);
    std::string item;
    // Lê a string até o delimitador e adiciona o item ao vetor.
    while (std::getline(ss, item, delimitador)) {
        tokens.push_back(item);
    }
    return tokens;
}

// Função para verificar se um arquivo existe no caminho especificado.
// caminho: O caminho completo do arquivo.
// Retorna true se o arquivo existe e pode ser aberto, false caso contrário.
bool arquivoExiste(const std::string& caminho) {
    std::ifstream file(caminho); // Tenta abrir o arquivo.
    return file.good(); // Retorna true se a abertura foi bem-sucedida.
}


