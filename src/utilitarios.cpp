#include "utilitarios.hpp"
#include <fstream>
#include <sstream>

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
