#include "preprocessamento.hpp"
#include "utilitarios.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <random>
#include <algorithm>

// Armazena os dados lidos de input.dat – Kairo
std::unordered_map<int, std::unordered_map<int, float>> dadosUsuarios;

void carregarDados(const std::string& caminhoArquivo) {
    // Le input.dat em formato usuario item:nota – Kairo
    leituraRapidaCSV(caminhoArquivo, dadosUsuarios);
}

void gerarInput(const std::string& arquivoCSV, const std::string& arquivoSaida) {
    // Le ratings.csv, aplica filtros >= 50 avaliacoes por usuario/item – Kairo
    // Remove duplicatas e gera input.dat no formato solicitado – Kairo

    std::ifstream in(arquivoCSV);
    std::unordered_map<int, std::unordered_map<int, float>> avaliacoes;
    std::unordered_map<int, int> contagemUsuarios;
    std::unordered_map<int, int> contagemFilmes;

    std::string linha;
    std::getline(in, linha); // cabecalho

    while (std::getline(in, linha)) {
        std::stringstream ss(linha);
        std::string campo;
        std::getline(ss, campo, ',');
        int usuario = std::stoi(campo);
        std::getline(ss, campo, ',');
        int filme = std::stoi(campo);
        std::getline(ss, campo, ',');
        float nota = std::stof(campo);
        avaliacoes[usuario][filme] = nota;
    }
    in.close();

    for (const auto& [usuario, filmes] : avaliacoes) {
        contagemUsuarios[usuario] = filmes.size();
        for (const auto& [filme, _] : filmes)
            contagemFilmes[filme]++;
    }

    std::ofstream out(arquivoSaida);
    for (const auto& [usuario, filmes] : avaliacoes) {
        if (contagemUsuarios[usuario] < 50) continue;

        std::ostringstream linha;
        linha << usuario;
 
        int filmesValidos = 0;
        for (const auto& [filme, nota] : filmes) {
            if (contagemFilmes[filme] >= 50) {
                linha << " " << filme << ":" << nota;
                filmesValidos++;
            }
        }

        if (filmesValidos > 0)
            out << linha.str() << "\n";
    }
    out.close();
}
// Seleciona N usuarios aleatorios de input.dat e grava em explore.dat – Kairo
void gerarExplore(const std::string& inputDat, const std::string& exploreDat, int quantidade) {
    std::ifstream in(inputDat);
    std::vector<int> usuarios;
    std::string linha;

    while (std::getline(in, linha)) {
        std::stringstream ss(linha);
        int id;
        ss >> id;
        usuarios.push_back(id);
    }
    in.close();

    // Garante que não vai tentar pegar mais usuários do que existe
    if (quantidade > static_cast<int>(usuarios.size())) 
        quantidade = usuarios.size();

    // Embaralha os usuários – Kairo
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(usuarios.begin(), usuarios.end(), rng);

    std::ofstream out(exploreDat);
    for (int i = 0; i < quantidade; ++i)
        out << usuarios[i] << "\n";
    out.close();
}

