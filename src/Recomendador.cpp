#include "Recomendador.hpp"
#include "CalculadorDeSimilaridade.hpp"
#include "utilitarios.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <thread>

Recomendador::Recomendador(GerenciadorDeDados& gerenciador, const Configuracao& config)
    : gerenciador(gerenciador), config(config) {}

void Recomendador::recomendarParaUsuario(int usuarioId, std::ostream& outFile) {
    const Usuario& perfilAtual = gerenciador.getUsuario(usuarioId);
    std::unordered_set<int> filmesVistos;
    for (const auto& [filme, _] : perfilAtual.getAvaliacoes()) {
        filmesVistos.insert(filme);
    }

    std::vector<std::pair<int, float>> similares;
    similares.reserve(gerenciador.getTodosUsuarios().size());

    for (const auto& [outroId, outroUsuario] : gerenciador.getTodosUsuarios()) {
        if (outroId == usuarioId) continue;
        float sim = CalculadorDeSimilaridade::calcularSimilaridadeCosseno(
            perfilAtual.getAvaliacoes(),
            outroUsuario.getAvaliacoes()
        );
        similares.push_back({outroId, sim});
    }

    std::sort(similares.begin(), similares.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    std::unordered_map<int, float> somaNotas;
    std::unordered_map<int, int> contagem;

    for (int i = 0; i < std::min(config.K_VIZINHOS, (int)similares.size()); ++i) {
        int vizinhoId = similares[i].first;
        const Usuario& vizinhoUsuario = gerenciador.getUsuario(vizinhoId);
        for (const auto& [filme, nota] : vizinhoUsuario.getAvaliacoes()) {
            if (filmesVistos.count(filme) == 0) {
                somaNotas[filme] += nota;
                contagem[filme]++;
            }
        }
    }

    std::vector<std::pair<int, float>> candidatos;
    candidatos.reserve(somaNotas.size());
    for (const auto& [filme, soma] : somaNotas) {
        float media = soma / contagem[filme];
        candidatos.push_back({filme, media});
    }

    std::sort(candidatos.begin(), candidatos.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    {
        std::lock_guard<std::mutex> lock(mtx);
        outFile << usuarioId;
        for (int i = 0; i < std::min(config.N_RECOMENDACOES, (int)candidatos.size()); ++i) {
            outFile << " " << candidatos[i].first; // Usa ID do filme, não o nome
        }
        outFile << "\n";
    }
}

void Recomendador::recomendarParaUsuarios(const std::string& arquivoExploracao, const std::string& arquivoSaida, int numThreads) {
    std::ifstream in(arquivoExploracao);
    std::ofstream out(arquivoSaida);
    if (!in.is_open() || !out.is_open()) return;

    std::vector<int> usuariosParaExplorar;
    int usuarioId;
    while (in >> usuarioId) {
        usuariosParaExplorar.push_back(usuarioId);
    }
    in.close();

    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&, i]() {
            for (size_t j = i; j < usuariosParaExplorar.size(); j += numThreads) {
                recomendarParaUsuario(usuariosParaExplorar[j], out);
            }
        });
    }

    for (std::thread& t : threads) {
        t.join();
    }
    out.close();
}
