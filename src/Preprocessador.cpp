#include "Preprocessador.hpp"
#include <algorithm>
#include <fstream>
#include <memory>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "utilitarios.hpp"

void Preprocessador::gerarInput(const std::string& arquivoCSV, const std::string& arquivoSaida) {
    constexpr size_t BUFFER_SIZE = 1048576;
    auto in_buffer = std::make_unique<char[]>(BUFFER_SIZE);
    auto out_buffer = std::make_unique<char[]>(BUFFER_SIZE);
    
    std::ifstream in(arquivoCSV);
    in.rdbuf()->pubsetbuf(in_buffer.get(), BUFFER_SIZE);
    
    std::ofstream out(arquivoSaida);
    out.rdbuf()->pubsetbuf(out_buffer.get(), BUFFER_SIZE);
    
    if (!in.is_open() || !out.is_open()) return;

    std::string linha;
    std::getline(in, linha); // Ignorar cabeçalho
    
    std::unordered_map<int, int> filmesCounts;
    std::unordered_map<int, std::unordered_map<int, float>> usuarioFilmes;
    filmesCounts.reserve(70000);
    usuarioFilmes.reserve(170000);

    while (std::getline(in, linha)) {
        const char* ptr = linha.c_str();
        int userId = 0;
        while (*ptr >= '0' && *ptr <= '9') {
            userId = userId * 10 + (*ptr - '0');
            ptr++;
        }
        ptr++; // skip comma
        int movieId = 0;
        while (*ptr >= '0' && *ptr <= '9') {
            movieId = movieId * 10 + (*ptr - '0');
            ptr++;
        }
        ptr++; // skip comma
        float rating = 0.0f;
        float decimal = 1.0f;
        bool after_dot = false;
        while ((*ptr >= '0' && *ptr <= '9') || *ptr == '.') {
            if (*ptr == '.') {
                after_dot = true;
            } else {
                if (after_dot) {
                    decimal *= 0.1f;
                    rating += (*ptr - '0') * decimal;
                } else {
                    rating = rating * 10 + (*ptr - '0');
                }
            }
            ptr++;
        }
        
        filmesCounts[movieId]++;
        usuarioFilmes[userId][movieId] = rating;
    }

    std::unordered_set<int> validMovies;
    for (const auto& [filmeId, count] : filmesCounts) {
        if (count >= 50) {
            validMovies.insert(filmeId);
        }
    }

    for (const auto& [userId, avaliacoes] : usuarioFilmes) {
        int validCount = 0;
        for (const auto& [filmeId, _] : avaliacoes) {
            if (validMovies.find(filmeId) != validMovies.end()) {
                validCount++;
            }
        }
        
        if (validCount >= 50) {
            out << userId;
            for (const auto& [filmeId, nota] : avaliacoes) {
                if (validMovies.find(filmeId) != validMovies.end()) {
                    out << " " << filmeId << ":" << nota;
                }
            }
            out << "\n";
        }
    }
    out.close();
}

void Preprocessador::gerarExplore(const std::string& inputDat, const std::string& exploreDat, int quantidade) {
    std::ifstream in(inputDat);
    std::ofstream out(exploreDat);
    if (!in.is_open() || !out.is_open()) return;

    std::vector<int> userIds;
    userIds.reserve(100000);
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

    std::vector<int> selectedUsers;
    int limite = std::min(quantidade, static_cast<int>(userIds.size()));
    selectedUsers.reserve(limite);
    for (int i = 0; i < limite; ++i) {
        selectedUsers.push_back(userIds[i]);
    }
    std::sort(selectedUsers.begin(), selectedUsers.end());

    for (int userId : selectedUsers) {
        out << userId << "\n";
    }
    out.close();
}
