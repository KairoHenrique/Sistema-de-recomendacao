#include "GerenciadorDeDados.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include "utilitarios.hpp" // Para a função dividir

GerenciadorDeDados::GerenciadorDeDados() {}

void GerenciadorDeDados::carregarDados(const std::string& caminhoArquivo) {
    std::ifstream arquivo(caminhoArquivo);
    std::string linha;

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de dados: " << caminhoArquivo << std::endl;
        return;
    }

    while (std::getline(arquivo, linha)) {
        auto partes = dividir(linha, ' ');
        if (partes.size() < 2) continue;

        int usuarioId = std::stoi(partes[0]);
        Usuario usuario(usuarioId);

        for (size_t i = 1; i < partes.size(); ++i) {
            auto par = dividir(partes[i], ':');
            if (par.size() == 2)
                usuario.adicionarAvaliacao(std::stoi(par[0]), std::stof(par[1]));
        }
        dadosUsuarios.emplace(usuarioId, usuario);
    }
    arquivo.close();
}

void GerenciadorDeDados::carregarNomesFilmes(const std::string& caminhoCSV) {
    std::ifstream arquivo(caminhoCSV);
    std::string linha;

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de filmes: " << caminhoCSV << std::endl;
        return;
    }

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

const Usuario& GerenciadorDeDados::getUsuario(int usuarioId) const {
    return dadosUsuarios.at(usuarioId);
}

const std::string& GerenciadorDeDados::getNomeFilme(int filmeId) const {
    return nomesFilmes.at(filmeId);
}

const std::unordered_map<int, Usuario>& GerenciadorDeDados::getTodosUsuarios() const {
    return dadosUsuarios;
}


