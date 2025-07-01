#include "utilitarios.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

// Definição da função utilitária. Agora ela vive aqui.
std::string lerArquivoInteiro(const std::string& caminho) {
    std::ifstream arquivo(caminho, std::ios::binary | std::ios::ate);
    if (!arquivo) {
        std::cerr << "Erro fatal: nao foi possivel abrir o arquivo: " << caminho << std::endl;
        exit(1);
    }
    std::streamsize tamanho = arquivo.tellg();
    arquivo.seekg(0, std::ios::beg);
    std::string buffer(tamanho, '\0');
    if (!arquivo.read(buffer.data(), tamanho)) {
        std::cerr << "Erro fatal: nao foi possivel ler o arquivo: " << caminho << std::endl;
        exit(1);
    }
    return buffer;
}


std::vector<std::string> dividir(const std::string& linha, char delimitador) {
    std::vector<std::string> tokens;
    std::stringstream ss(linha);
    std::string item;
    while (std::getline(ss, item, delimitador)) {
        tokens.push_back(item);
    }
    return tokens;
}

bool arquivoExiste(const std::string& caminho) {
    std::ifstream file(caminho);
    return file.good();
}
