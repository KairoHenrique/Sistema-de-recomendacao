#include "recomendador.hpp"
#include "utilitarios.hpp"
#include "config.hpp" // Constantes K e N – Kairo
#include <fstream>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>

extern std::unordered_map<int, std::unordered_map<int, float>> dadosUsuarios;

void recomendarParaUsuarios(const std::string& arquivoExploracao, const std::string& arquivoSaida) {
    std::ifstream in(arquivoExploracao);
    std::ofstream out(arquivoSaida);

    if (!in.is_open() || !out.is_open()) {
        std::cerr << "Erro ao abrir arquivos de entrada/saida – Kairo" << std::endl;
        return;
    }

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

        std::map<int, float> somaNotas; // filme -> soma das notas dos vizinhos
        std::map<int, int> contagem;    // filme -> qtd de vizinhos que avaliaram

        for (int i = 0; i < std::min(K_VIZINHOS, (int)similares.size()); ++i) {
            int vizinhoId = similares[i].first;
            const auto& filmesVizinho = dadosUsuarios[vizinhoId];

            for (const auto& [filme, nota] : filmesVizinho) {
                if (filmesVistos.count(filme) == 0) { // se o usuario atual nao viu – Kairo
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
        for (int i = 0; i < std::min(N_RECOMENDACOES, (int)candidatos.size()); ++i)
            out << " " << candidatos[i].first;
        out << "\n";
    }

    in.close();
    out.close();
}
