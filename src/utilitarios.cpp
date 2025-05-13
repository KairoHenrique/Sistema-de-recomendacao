#include "utilitarios.hpp"
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>

std::vector<std::string> dividir(const std::string& linha, char delimitador) {
    // Divide uma linha por delimitador (ex: espaco ou :) – Kairo
    std::vector<std::string> tokens;
    std::stringstream ss(linha);
    std::string item;
    while (std::getline(ss, item, delimitador)) {
        tokens.push_back(item);
    }
    return tokens;
}

bool arquivoExiste(const std::string& caminho) {
    // Verifica existencia de arquivo – Kairo
    std::ifstream file(caminho);
    return file.good();
}

void leituraRapidaCSV(const std::string& caminho,
    std::unordered_map<int, std::unordered_map<int, float>>& dadosUsuarios)
{
    // Le input.dat no formato usuario item:nota – Kairo
    std::ifstream arquivo(caminho);
    std::string linha;

    while (std::getline(arquivo, linha)) {
        auto partes = dividir(linha, ' ');
        if (partes.size() < 2) continue;

        int usuario = std::stoi(partes[0]);
        for (size_t i = 1; i < partes.size(); ++i) {
            auto par = dividir(partes[i], ':');
            if (par.size() == 2)
                dadosUsuarios[usuario][std::stoi(par[0])] = std::stof(par[1]);
        }
    }
    arquivo.close();
}

float similaridadeUsuarios(const std::unordered_map<int, float>& u1,
                           const std::unordered_map<int, float>& u2)
{
    // Calcula a similaridade do cosseno entre dois perfis de usuario – Kairo
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

void carregarNomesFilmes(const std::string& caminhoCSV,
    std::unordered_map<int, std::string>& nomesFilmes)
{
    // Le movies.csv e armazena os nomes dos filmes – Kairo
    std::ifstream arquivo(caminhoCSV);
    std::string linha;
    std::getline(arquivo, linha); // cabecalho

    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);
        std::string campo;

        std::getline(ss, campo, ',');
        int filmeId = std::stoi(campo);

        std::getline(ss, campo, ',');
        std::string nome = campo;

        if (nome.front() == '"') {
            std::string resto;
            std::getline(ss, resto, '"');
            nome += "," + resto;
        }

        nomesFilmes[filmeId] = nome;
    }

    arquivo.close();
}
