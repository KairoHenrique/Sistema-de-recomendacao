#include "recomendador.hpp"
#include "utilitarios.hpp"
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>

// Usa a variavel global com dados dos usuarios – Kairo
extern std::unordered_map<int, std::unordered_map<int, float>> dadosUsuarios;

void recomendarParaUsuarios(const std::string& arquivoExploracao, const std::string& arquivoSaida) {
    // Para cada usuario do explore.dat, calcula os mais similares usando similaridade do cosseno – Kairo
    // Escreve os IDs dos mais similares no output.dat – Kairo

    std::ifstream in(arquivoExploracao);
    std::ofstream out(arquivoSaida);

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

        out << usuarioId;
        for (int i = 0; i < std::min(5, (int)similares.size()); ++i)
            out << " " << similares[i].first;
        out << "\n";
    }
    in.close();
    out.close();
}
