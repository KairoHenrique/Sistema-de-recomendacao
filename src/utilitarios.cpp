#include "utilitarios.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

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

void leituraRapidaCSV(const std::string& caminho,
    std::unordered_map<int, std::unordered_map<int, float>>& dadosUsuarios)
{
    std::ifstream arquivo(caminho);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << caminho << std::endl;
        return;
    }

    std::string linha;
    std::getline(arquivo, linha); // Pular cabecalho

    while (std::getline(arquivo, linha)) {
        auto partes = dividir(linha, ',');
        if (partes.size() < 3) continue;

        int usuario = std::stoi(partes[0]);
        int item = std::stoi(partes[1]);
        float nota = std::stof(partes[2]);

        dadosUsuarios[usuario][item] = nota;
    }

    arquivo.close();
}

float similaridadeUsuarios(const std::unordered_map<int, float>& u1,
                           const std::unordered_map<int, float>& u2)
{
    float numerador = 0.0f, mag1 = 0.0f, mag2 = 0.0f;

    for (const auto& [item, nota1] : u1) {
        if (u2.count(item)) {
            float nota2 = u2.at(item);
            numerador += nota1 * nota2;
        }
        mag1 += nota1 * nota1;
    }

    for (const auto& [_, nota2] : u2) {
        mag2 += nota2 * nota2;
    }

    if (mag1 == 0 || mag2 == 0) return 0.0f;
    return numerador / (std::sqrt(mag1) * std::sqrt(mag2));
}
