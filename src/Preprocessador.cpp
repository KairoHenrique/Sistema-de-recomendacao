#include "Preprocessador.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>
#include <algorithm>
#include <set>
#include "utilitarios.hpp" // Para a função dividir
#include <unordered_map>

void Preprocessador::gerarInput(const std::string& arquivoCSV, const std::string& arquivoSaida) {
    std::ifstream in(arquivoCSV);
    std::ofstream out(arquivoSaida);

    if (!in.is_open() || !out.is_open()) {
        std::cerr << "Erro ao abrir arquivos de entrada/saida para gerarInput" << std::endl;
        return;
    }

    std::string linha;
    std::getline(in, linha); // Ignorar cabeçalho

    std::unordered_map<int, std::unordered_map<int, float>> usuarioFilmes;

    while (std::getline(in, linha)) {
        std::stringstream ss(linha);
        std::string campo;

        std::getline(ss, campo, ','); // userId
        int userId = std::stoi(campo);

        std::getline(ss, campo, ','); // movieId
        int movieId = std::stoi(campo);

        std::getline(ss, campo, ','); // rating
        float rating = std::stof(campo);

        usuarioFilmes[userId][movieId] = rating;
    }

    for (const auto& userPair : usuarioFilmes) {
        out << userPair.first;
        for (const auto& movieRatingPair : userPair.second) {
            out << " " << movieRatingPair.first << ":" << movieRatingPair.second;
        }
        out << "\n";
    }

    in.close();
    out.close();
}

void Preprocessador::gerarExplore(const std::string& inputDat, const std::string& exploreDat, int quantidade) {
    std::ifstream in(inputDat);
    std::ofstream out(exploreDat);

    if (!in.is_open() || !out.is_open()) {
        std::cerr << "Erro ao abrir arquivos de entrada/saida para gerarExplore" << std::endl;
        return;
    }

    std::vector<int> userIds;
    std::string linha;
    while (std::getline(in, linha)) {
        auto partes = dividir(linha, ' ');
        if (!partes.empty()) {
            userIds.push_back(std::stoi(partes[0]));
        }
    }
    in.close();

    if (userIds.empty()) {
        std::cerr << "Nenhum usuario encontrado em " << inputDat << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(userIds.begin(), userIds.end(), g);

    std::set<int> selectedUsers;
    int limite = std::min(quantidade, static_cast<int>(userIds.size()));
    for (int i = 0; i < limite; ++i) {
    selectedUsers.insert(userIds[i]);
}


    in.open(inputDat); // Reabrir input.dat para ler os dados dos usuários selecionados
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

