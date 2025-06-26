#include "Recomendador.hpp"
#include "CalculadorDeSimilaridade.hpp"
#include "utilitarios.hpp"
#include <algorithm>
#include <atomic>
#include <cmath>
#include <fstream>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <unordered_set>
#include <vector>

Recomendador::Recomendador(GerenciadorDeDados& gerenciador, const Configuracao& config)
    : gerenciador(gerenciador), config(config) {}

void Recomendador::recomendarParaUsuario(int usuarioId, std::ostream& outFile) {
    const Usuario& perfilAtual = gerenciador.getUsuario(usuarioId);
    float mag1 = gerenciador.getMagnitude(usuarioId);
    std::unordered_set<int> filmesVistos;
    filmesVistos.reserve(perfilAtual.getAvaliacoes().size());
    
    for (const auto& [filme, _] : perfilAtual.getAvaliacoes()) {
        filmesVistos.insert(filme);
    }

    const auto& todosUsuarios = gerenciador.getTodosUsuarios();
    std::vector<int> candidatosIds;
    candidatosIds.reserve(todosUsuarios.size());
    
    for (const auto& [id, _] : todosUsuarios) {
        if (id != usuarioId) candidatosIds.push_back(id);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(candidatosIds.begin(), candidatosIds.end(), g);
    
    int amostra = std::min(5000, static_cast<int>(candidatosIds.size()));
    std::vector<std::pair<int, float>> similares;
    similares.reserve(amostra);

    for (int i = 0; i < amostra; ++i) {
        int outroId = candidatosIds[i];
        const Usuario& outroUsuario = gerenciador.getUsuario(outroId);
        float mag2 = gerenciador.getMagnitude(outroId);
        
        float sim = CalculadorDeSimilaridade::calcularSimilaridadeCosseno(
            perfilAtual.getAvaliacoes(), mag1,
            outroUsuario.getAvaliacoes(), mag2
        );
        similares.push_back({outroId, sim});
    }

    int k_count = std::min(config.K_VIZINHOS, static_cast<int>(similares.size()));
    std::partial_sort(similares.begin(), similares.begin() + k_count, similares.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    std::unordered_map<int, float> somaNotas;
    std::unordered_map<int, float> somaPesos;
    somaNotas.reserve(1000);
    somaPesos.reserve(1000);

    for (int i = 0; i < k_count; ++i) {
        int vizinhoId = similares[i].first;
        float sim = similares[i].second;
        const Usuario& vizinhoUsuario = gerenciador.getUsuario(vizinhoId);
        
        for (const auto& [filme, nota] : vizinhoUsuario.getAvaliacoes()) {
            if (filmesVistos.count(filme) == 0) {
                somaNotas[filme] += sim * nota;
                somaPesos[filme] += sim;
            }
        }
    }

    thread_local std::vector<std::pair<int, float>> candidatos;
    candidatos.clear();
    candidatos.reserve(somaNotas.size());
    
    for (const auto& [filme, soma] : somaNotas) {
        if (somaPesos[filme] != 0) {
            float media = soma / somaPesos[filme];
            candidatos.push_back({filme, media});
        }
    }

    int n_count = std::min(config.N_RECOMENDACOES, static_cast<int>(candidatos.size()));
    if (n_count > 0) {
        std::partial_sort(candidatos.begin(), candidatos.begin() + n_count, candidatos.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });
    }

    std::lock_guard<std::mutex> lock(mtx);
    outFile << usuarioId;

    for (int i = 0; i < n_count; ++i) {
        int filmeId = candidatos[i].first;
        const auto& nomesMap = gerenciador.getNomesFilmesMap();
        auto it = nomesMap.find(filmeId);
        if (it != nomesMap.end()) {
            outFile << " " << filmeId << ":" << it->second;
        } else {
            outFile << " " << filmeId << ":Filme_" << filmeId;
        }
    }
    outFile << "\n";
}

void Recomendador::recomendarParaUsuarios(const std::string& arquivoExploracao, const std::string& arquivoSaida, int numThreads) {
    std::ifstream in(arquivoExploracao);
    std::ofstream out(arquivoSaida);
    if (!in.is_open() || !out.is_open()) {
        std::cerr << "Erro ao abrir arquivos de entrada/saida" << std::endl;
        return;
    }

    gerenciador.carregarNomesFilmes("dados/movies.csv");
    std::vector<int> usuariosParaExplorar;
    int usuarioId;
    while (in >> usuarioId) {
        usuariosParaExplorar.push_back(usuarioId);
    }
    in.close();

    std::atomic<size_t> next_index(0);
    std::vector<std::thread> threads;
    
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&]() {
            size_t j = next_index.fetch_add(1, std::memory_order_relaxed);
            while (j < usuariosParaExplorar.size()) {
                recomendarParaUsuario(usuariosParaExplorar[j], out);
                j = next_index.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    for (std::thread& t : threads) {
        t.join();
    }
    out.close();
}
