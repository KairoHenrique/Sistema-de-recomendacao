#include "Preprocessador.hpp"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <random>
// Remover #include <execution> se não for usado em outros lugares
#include "utilitarios.hpp"

void Preprocessador::gerarInput(const std::string& arquivoCSV, const std::string& arquivoSaida) {
    // Implementação dos filtros obrigatórios mantida
    constexpr size_t BUFFER_SIZE = 65536;
    char* buffer = new char[BUFFER_SIZE];
    
    std::ifstream in(arquivoCSV);
    std::ofstream out(arquivoSaida);
    in.rdbuf()->pubsetbuf(buffer, BUFFER_SIZE);
    
    if (!in.is_open() || !out.is_open()) {
        delete[] buffer;
        return;
    }

    std::string linha;
    std::getline(in, linha); // Ignorar cabeçalho

    std::unordered_map<int, std::unordered_map<int, float>> usuarioFilmes;
    std::unordered_map<int, std::vector<int>> filmesUsuarios;
    usuarioFilmes.reserve(300000);
    filmesUsuarios.reserve(100000);

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

        usuarioFilmes[userId][movieId] = rating;
        filmesUsuarios[movieId].push_back(userId);
    }
    in.close();

    // FILTRO 1: Filmes com pelo menos 50 usuários
    std::vector<int> filmesValidos;
    filmesValidos.reserve(filmesUsuarios.size() / 2);
    
    for (const auto& [filmeId, usuarios] : filmesUsuarios) {
        if (usuarios.size() >= 50) {
            filmesValidos.push_back(filmeId);
        }
    }
    std::sort(filmesValidos.begin(), filmesValidos.end());

    // FILTRO 2: Usuários com pelo menos 50 avaliações de filmes válidos
    std::unordered_map<int, std::unordered_map<int, float>> usuariosValidos;
    usuariosValidos.reserve(usuarioFilmes.size() / 3);

    for (const auto& [userId, avaliacoes] : usuarioFilmes) {
        int validCount = 0;
        for (const auto& [filmeId, _] : avaliacoes) {
            if (std::binary_search(filmesValidos.begin(), filmesValidos.end(), filmeId)) {
                validCount++;
            }
        }
        if (validCount >= 50) {
            usuariosValidos[userId] = avaliacoes;
        }
    }

    // Escrita otimizada
    char* write_buffer = new char[BUFFER_SIZE];
    out.rdbuf()->pubsetbuf(write_buffer, BUFFER_SIZE);

    for (const auto& [userId, avaliacoes] : usuariosValidos) {
        out << userId;
        for (const auto& [filmeId, nota] : avaliacoes) {
            if (std::binary_search(filmesValidos.begin(), filmesValidos.end(), filmeId)) {
                out << " " << filmeId << ":" << nota;
            }
        }
        out << "\n";
    }

    out.close();
    delete[] buffer;
    delete[] write_buffer;
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

    // CORREÇÃO: Remover política de execução paralela
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(userIds.begin(), userIds.end(), g);

    // Usar vector ordenado ao invés de set
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
