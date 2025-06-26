#include "GerenciadorDeDados.hpp"
#include <cmath>
#include <fstream>
#include <memory>
#include <sstream>
#include <utility>
#include "utilitarios.hpp"

GerenciadorDeDados::GerenciadorDeDados() {
    dadosUsuarios.reserve(170000);
    magnitudes.reserve(170000);
}

void GerenciadorDeDados::carregarDados(const std::string& caminhoArquivo) {
    constexpr size_t BUFFER_SIZE = 1048576;
    auto buffer = std::make_unique<char[]>(BUFFER_SIZE);
    
    std::ifstream arquivo(caminhoArquivo);
    arquivo.rdbuf()->pubsetbuf(buffer.get(), BUFFER_SIZE);
    
    if (!arquivo.is_open()) return;

    std::string linha;
    while (std::getline(arquivo, linha)) {
        const char* ptr = linha.c_str();
        int usuarioId = 0;
        while (*ptr >= '0' && *ptr <= '9') {
            usuarioId = usuarioId * 10 + (*ptr - '0');
            ptr++;
        }

        Usuario usuario(usuarioId);
        float mag = 0.0f;
        
        while (*ptr != '\0' && *ptr != '\n') {
            if (*ptr == ' ') {
                ptr++;
                int movieId = 0;
                while (*ptr >= '0' && *ptr <= '9') {
                    movieId = movieId * 10 + (*ptr - '0');
                    ptr++;
                }
                if (*ptr == ':') {
                    ptr++;
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
                    mag += rating * rating;
                }
            } else {
                ptr++;
            }
        }
        
        magnitudes[usuarioId] = std::sqrt(mag);
        dadosUsuarios.emplace(usuarioId, std::move(usuario));
    }
    arquivo.close();
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

float GerenciadorDeDados::getMagnitude(int usuarioId) const {
    return magnitudes.at(usuarioId);
}
