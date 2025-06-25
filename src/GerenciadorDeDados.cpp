#include "GerenciadorDeDados.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "utilitarios.hpp"

GerenciadorDeDados::GerenciadorDeDados() {
    dadosUsuarios.reserve(100000); // Pré-alocação estimada
    nomesFilmes.reserve(50000);
}

void GerenciadorDeDados::carregarDados(const std::string& caminhoArquivo) {
    std::ifstream arquivo(caminhoArquivo);
    std::string linha;
    
    if (!arquivo.is_open()) {
        return;
    }

    while (std::getline(arquivo, linha)) {
        if (linha.empty()) continue;
        
        // Parsing manual otimizado
        size_t pos = linha.find(' ');
        if (pos == std::string::npos) continue;
        
        int usuarioId = std::stoi(linha.substr(0, pos));
        Usuario usuario(usuarioId);
        
        size_t start = pos + 1;
        while (start < linha.length()) {
            size_t end = linha.find(' ', start);
            if (end == std::string::npos) end = linha.length();
            
            std::string par = linha.substr(start, end - start);
            size_t colonPos = par.find(':');
            
            if (colonPos != std::string::npos) {
                int filmeId = std::stoi(par.substr(0, colonPos));
                float nota = std::stof(par.substr(colonPos + 1));
                usuario.adicionarAvaliacao(filmeId, nota);
            }
            
            start = end + 1;
        }
        
        dadosUsuarios.emplace(usuarioId, std::move(usuario));
    }
    
    arquivo.close();
}

void GerenciadorDeDados::carregarNomesFilmes(const std::string& caminhoCSV) {
    std::ifstream arquivo(caminhoCSV);
    std::string linha;
    
    if (!arquivo.is_open()) {
        return;
    }

    std::getline(arquivo, linha); // Pular cabeçalho
    
    while (std::getline(arquivo, linha)) {
        if (linha.empty()) continue;
        
        size_t pos1 = linha.find(',');
        if (pos1 == std::string::npos) continue;
        
        int filmeId = std::stoi(linha.substr(0, pos1));
        
        size_t pos2 = linha.find(',', pos1 + 1);
        if (pos2 == std::string::npos) pos2 = linha.length();
        
        std::string nome = linha.substr(pos1 + 1, pos2 - pos1 - 1);
        
        // Remover aspas se existirem
        if (!nome.empty() && nome.front() == '"') {
            nome = nome.substr(1);
        }
        if (!nome.empty() && nome.back() == '"') {
            nome = nome.substr(0, nome.length() - 1);
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