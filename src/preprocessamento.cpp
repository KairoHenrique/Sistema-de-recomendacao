#include "preprocessamento.hpp"
#include "utilitarios.hpp"
#include <unordered_map>
#include <iostream>
// Função externa para armazenar os dados dos usuários - Kairo
std::unordered_map<int, std::unordered_map<int, float>> dadosUsuarios;
void carregarDados(const std::string& caminhoArquivo) {
    std::cout << "[Pre-processamento] Lendo: " << caminhoArquivo << std::endl;
    leituraRapidaCSV(caminhoArquivo, dadosUsuarios);
}
