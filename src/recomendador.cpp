#include "recomendador.hpp"
#include "utilitarios.hpp"
#include "config.hpp"
#include <fstream>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <map>
#include <iostream>

extern std::unordered_map<int, std::unordered_map<int, float>> dadosUsuarios;

void recomendarParaUsuarios(const std::string& arquivoExploracao, const std::string& arquivoSaida) {
    std::ifstream in(arquivoExploracao);
    std::ofstream out(arquivoSaida);

    if (!in.is_open() || !out.is_open()) {
        std::cerr << "Erro ao abrir arquivos de entrada/saida" << std::endl;
        return;
    }

    std::unordered_map<int, std::string> nomesFilmes;
    carregarNomesFilmes("dados/movies.csv", nomesFilmes); // Le os nomes dos filmes – Kairo

    int usuarioId;
    while (in >> usuarioId) {
        const auto& perfilAtual = dadosUsuarios[usuarioId];
        std::unordered_set<int> filmesVistos;
        for (const auto& [filme, _] : perfilAtual) {
            filmesVistos.insert(filme);
        }

        std::vector<std::pair<int, float>> similares;
        for (const auto& [outroId, perfil] : dadosUsuarios) {
            if (outroId == usuarioId) continue;
            float sim = similaridadeUsuarios(perfilAtual, perfil);
            similares.push_back({outroId, sim});
        }

        std::sort(similares.begin(), similares.end(),
                  [](const auto& a, const auto& b) { return a.second > b.second; });

        std::map<int, float> somaNotas; // filme -> soma das notas dos vizinhos – Kairo
        std::map<int, int> contagem;    // filme -> qtd de vizinhos que avaliaram – Kairo

        for (int i = 0; i < std::min(K_VIZINHOS, (int)similares.size()); ++i) {
            int vizinhoId = similares[i].first;
            const auto& filmesVizinho = dadosUsuarios[vizinhoId];

            for (const auto& [filme, nota] : filmesVizinho) {
                if (filmesVistos.count(filme) == 0) {
                    somaNotas[filme] += nota;
                    contagem[filme]++;
                }
            }
        }

        std::vector<std::pair<int, float>> candidatos;
        for (const auto& [filme, soma] : somaNotas) {
            float media = soma / contagem[filme];
            candidatos.push_back({filme, media});
        }

        std::sort(candidatos.begin(), candidatos.end(),
                  [](const auto& a, const auto& b) { return a.second > b.second; });

        out << usuarioId;
        for (int i = 0; i < std::min(N_RECOMENDACOES, (int)candidatos.size()); ++i) {
            int filmeId = candidatos[i].first;
            if (nomesFilmes.count(filmeId))
                out << " " << nomesFilmes[filmeId]; // Apenas o nome – Kairo
        }
        out << "\n";
    }

    in.close();
    out.close();
}
