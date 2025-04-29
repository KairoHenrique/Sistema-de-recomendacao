#include "preprocessamento.hpp"
#include "utilitarios.hpp"
#include <unordered_map>
#include <iostream>

std::unordered_map<int, std::unordered_map<int, float>> dadosUsuarios;

void carregarDados(const std::string& caminhoArquivo) {
    std::cout << "[Pre-processamento] Lendo: " << caminhoArquivo << std::endl;
    leituraRapidaCSV(caminhoArquivo, dadosUsuarios);
}
