#include "GerenciadorDeDados.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstring>
#include "utilitarios.hpp"

GerenciadorDeDados::GerenciadorDeDados() {
    dadosUsuarios.reserve(100000); // Pré-alocação baseada no dataset filtrado
}

void GerenciadorDeDados::carregarDados(const std::string& caminhoArquivo) {
    // I/O otimizado com buffer
    constexpr size_t BUFFER_SIZE = 65536;
    char* buffer = new char[BUFFER_SIZE];
    
    std::ifstream arquivo(caminhoArquivo);
    arquivo.rdbuf()->pubsetbuf(buffer, BUFFER_SIZE);
    
    if (!arquivo.is_open()) {
        delete[] buffer;
        return;
    }

    std::string linha;
    while (std::getline(arquivo, linha)) {
        const char* ptr = linha.c_str();
        
        // Parse manual ultra-rápido do user ID
        int usuarioId = 0;
        while (*ptr >= '0' && *ptr <= '9') {
            usuarioId = usuarioId * 10 + (*ptr - '0');
            ptr++;
        }

        Usuario usuario(usuarioId);
        
        // Parse das avaliações
        while (*ptr != '\0' && *ptr != '\n') {
            if (*ptr == ' ') {
                ptr++;
                
                // Parse movie ID
                int movieId = 0;
                while (*ptr >= '0' && *ptr <= '9') {
                    movieId = movieId * 10 + (*ptr - '0');
                    ptr++;
                }
                
                if (*ptr == ':') {
                    ptr++;
                    
                    // Parse rating
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
                    
                    usuario.adicionarAvaliacao(movieId, rating);
                }
            } else {
                ptr++;
            }
        }

        // Move semantics para evitar cópia
        dadosUsuarios.emplace(usuarioId, std::move(usuario));
    }

    arquivo.close();
    delete[] buffer;
}

void GerenciadorDeDados::carregarNomesFilmes(const std::string& caminhoCSV) {
    std::ifstream arquivo(caminhoCSV);
    if (!arquivo.is_open()) return;

    std::string linha;
    std::getline(arquivo, linha); // cabeçalho
    
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

        nomesFilmes[filmeId] = std::move(nome);
    }
    arquivo.close();
}

const Usuario& GerenciadorDeDados::getUsuario(int usuarioId) const {
    return dadosUsuarios.at(usuarioId);
}

const std::string& GerenciadorDeDados::getNomeFilme(int filmeId) const {
    return nomesFilmes.at(filmeId);
}

const std::unordered_map<int, Usuario>& GerenciadorDeDados::getTodosUsuarios() const {
    return dadosUsuarios;
}
