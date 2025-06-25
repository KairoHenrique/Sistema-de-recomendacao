#include "Preprocessador.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <random>
#include "utilitarios.hpp"

void Preprocessador::gerarInput(const std::string& arquivoCSV, const std::string& arquivoSaida) {
    std::ifstream in(arquivoCSV);
    std::ofstream out(arquivoSaida);
    if (!in.is_open() || !out.is_open()) return;

    std::string linha;
    std::getline(in, linha); // Ignorar cabeçalho

    std::unordered_map<int, std::unordered_map<int, float>> usuarioFilmes;
    std::unordered_map<int, std::unordered_set<int>> filmesUsuarios;

    while (std::getline(in, linha)) {
        std::stringstream ss(linha);
        std::string campo;

        std::getline(ss, campo, ',');
        int userId = std::stoi(campo);

        std::getline(ss, campo, ',');
        int movieId = std::stoi(campo);

        std::getline(ss, campo, ',');
        float rating = std::stof(campo);

        usuarioFilmes[userId][movieId] = rating;
        filmesUsuarios[movieId].insert(userId);
    }
    in.close();

    // Filtrar filmes com pelo menos 50 usuários
    std::unordered_set<int> filmesValidos;
    for (const auto& [filmeId, usuarios] : filmesUsuarios) {
        if (usuarios.size() >= 50) {
            filmesValidos.insert(filmeId);
        }
    }

    // Filtrar usuários com pelo menos 50 avaliações de filmes válidos
    std::unordered_map<int, std::unordered_map<int, float>> usuariosValidos;
    for (const auto& [userId, avaliacoes] : usuarioFilmes) {
        int count = 0;
        for (const auto& [filmeId, _] : avaliacoes) {
            if (filmesValidos.count(filmeId)) count++;
        }
        if (count >= 50) {
            usuariosValidos[userId] = avaliacoes;
        }
    }

    // Gerar arquivo de saída
    for (const auto& [userId, avaliacoes] : usuariosValidos) {
        out << userId;
        for (const auto& [filmeId, nota] : avaliacoes) {
            if (filmesValidos.count(filmeId)) {
                out << " " << filmeId << ":" << nota;
            }
        }
        out << "\n";
    }
    out.close();
}

void Preprocessador::gerarExplore(const std::string& inputDat, const std::string& exploreDat, int quantidade) {
    std::ifstream in(inputDat);
    std::ofstream out(exploreDat);
    if (!in.is_open() || !out.is_open()) return;

    std::vector<int> userIds;
    std::string linha;
    while (std::getline(in, linha)) {
        auto partes = dividir(linha, ' ');
        if (!partes.empty()) {
            userIds.push_back(std::stoi(partes[0]));
        }
    }
    in.close();

    if (userIds.empty()) return;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(userIds.begin(), userIds.end(), g);

    std::unordered_set<int> selectedUsers;
    int limite = std::min(quantidade, static_cast<int>(userIds.size()));
    for (int i = 0; i < limite; ++i) {
        selectedUsers.insert(userIds[i]);
    }

    in.open(inputDat);
    while (std::getline(in, linha)) {
        auto partes = dividir(linha, ' ');
        if (!partes.empty()) {
            int userId = std::stoi(partes[0]);
            if (selectedUsers.count(userId)) {
                out << userId << "\n";
            }
        }
    }
    in.close();
    out.close();
}
