//Algoritmo de recomendação de filmes
#include "recomendador.hpp"
#include "utilitarios.hpp"
#include <fstream>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <algorithm>


// Função externa para armazenar os dados dos usuários - Kairo
extern std::unordered_map<int, std::unordered_map<int, float>> dadosUsuarios;

// Função para recomendar filmes para usuários - Kairo
void recomendarParaUsuarios(const std::string& arquivoExploracao, const std::string& arquivoSaida) {
    std::ifstream in(arquivoExploracao);
    std::ofstream out(arquivoSaida);

    if (!in.is_open() || !out.is_open()) {
        std::cerr << "Erro ao abrir arquivos de entrada ou saida." << std::endl;
        return;
    }

    int usuarioId;
    while (in >> usuarioId) {
        std::vector<std::pair<int, float>> similares;

        for (const auto& [outroId, perfil] : dadosUsuarios) {
            if (outroId == usuarioId) continue;
            float sim = similaridadeUsuarios(dadosUsuarios[usuarioId], perfil);
            similares.push_back({outroId, sim});
        }

        std::sort(similares.begin(), similares.end(),
                  [](const auto& a, const auto& b) { return a.second > b.second; });

        // Por enquanto so mostra os 5 mais similares ao usuario - Kairo
        out << usuarioId;
        for (int i = 0; i < std::min(5, (int)similares.size()); ++i) {
            out << " " << similares[i].first;
        }
        out << "\n";
    }

    in.close();
    out.close();
}
