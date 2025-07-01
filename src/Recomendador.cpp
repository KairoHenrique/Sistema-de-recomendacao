#include "Recomendador.hpp"
#include "CalculadorDeSimilaridade.hpp"
#include <algorithm>
#include <atomic>
#include <vector>
#include <random>
#include <unordered_set>
#include <iostream>
#include <string_view>

Recomendador::Recomendador(GerenciadorDeDados& gerenciador, const Configuracao& config)
    : gerenciador(gerenciador), config(config) {}

void Recomendador::recomendarParaUsuario(int usuarioId, std::ostream& outFile) {
    const Usuario& perfilAtual = gerenciador.getUsuario(usuarioId);
    float mag1 = gerenciador.getMagnitude(usuarioId);

    std::unordered_set<int> filmesVistos;
    filmesVistos.reserve(perfilAtual.getAvaliacoes().size());
    for (const auto& [filmeId, _] : perfilAtual.getAvaliacoes()) {
        filmesVistos.insert(filmeId);
    }

    const auto& todosUsuarios = gerenciador.getTodosUsuarios();
    thread_local std::vector<int> candidatosIds;
    candidatosIds.clear();
    candidatosIds.reserve(todosUsuarios.size());
    for (const auto& [id, _] : todosUsuarios) {
        if (id != usuarioId) {
            candidatosIds.push_back(id);
        }
    }

    thread_local std::mt19937 g(std::random_device{}());
    std::shuffle(candidatosIds.begin(), candidatosIds.end(), g);
    
    int amostra = std::min(5000, static_cast<int>(candidatosIds.size()));
    
    thread_local std::vector<std::pair<int, float>> similares;
    similares.clear();
    similares.reserve(amostra);

    for (int i = 0; i < amostra; ++i) {
        int outroId = candidatosIds[i];
        const Usuario& outroUsuario = gerenciador.getUsuario(outroId);
        float mag2 = gerenciador.getMagnitude(outroId);
        
        float sim = CalculadorDeSimilaridade::calcularSimilaridadeCosseno(
            perfilAtual.getAvaliacoes(), mag1,
            outroUsuario.getAvaliacoes(), mag2
        );
        if (sim > 0.0f) {
            similares.push_back({outroId, sim});
        }
    }

    int k_count = std::min(config.K_VIZINHOS, static_cast<int>(similares.size()));
    std::partial_sort(similares.begin(), similares.begin() + k_count, similares.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    thread_local std::unordered_map<int, float> somaNotas;
    thread_local std::unordered_map<int, float> somaPesos;
    somaNotas.clear();
    somaPesos.clear();

    for (int i = 0; i < k_count; ++i) {
        const auto& [vizinhoId, sim] = similares[i];
        const Usuario& vizinhoUsuario = gerenciador.getUsuario(vizinhoId);
        
        for (const auto& [filmeId, nota] : vizinhoUsuario.getAvaliacoes()) {
            if (filmesVistos.find(filmeId) == filmesVistos.end()) {
                somaNotas[filmeId] += sim * nota;
                somaPesos[filmeId] += sim;
            }
        }
    }

    thread_local std::vector<std::pair<int, float>> candidatos;
    candidatos.clear();
    candidatos.reserve(somaNotas.size());
    for (const auto& [filme, soma] : somaNotas) {
        if (somaPesos.count(filme) && somaPesos.at(filme) > 0.0f) {
            candidatos.push_back({filme, soma / somaPesos.at(filme)});
        }
    }

    int n_count = std::min(config.N_RECOMENDACOES, static_cast<int>(candidatos.size()));
    if (n_count > 0) {
        std::partial_sort(candidatos.begin(), candidatos.begin() + n_count, candidatos.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });
    }

    // --- MODIFICAÇÃO APLICADA AQUI ---
    std::lock_guard<std::mutex> lock(mtx);
    outFile << usuarioId;
    for (int i = 0; i < n_count; ++i) {
        int filmeId = candidatos[i].first;
        // Busca o nome do filme usando o gerenciador de dados
        std::string_view nomeFilme = gerenciador.getNomeFilme(filmeId);
        outFile << " " << filmeId << ":";
        if (!nomeFilme.empty()) {
            outFile << nomeFilme;
        } else {
            outFile << "NomeNaoEncontrado";
        }
    }
    outFile << "\n";
}

void Recomendador::recomendarParaUsuarios(const std::string& arquivoExploracao, const std::string& arquivoSaida, int numThreads) {
    std::ifstream in(arquivoExploracao);
    if (!in.is_open()) {
        std::cerr << "Erro fatal: nao foi possivel abrir arquivo de exploracao: " << arquivoExploracao << std::endl;
        return;
    }

    std::vector<int> usuariosParaExplorar;
    int usuarioId;
    while (in >> usuarioId) {
        usuariosParaExplorar.push_back(usuarioId);
    }
    in.close();
    
    std::ofstream out(arquivoSaida);
    if (!out.is_open()) {
        std::cerr << "Erro fatal: nao foi possivel criar arquivo de saida: " << arquivoSaida << std::endl;
        return;
    }

    std::atomic<size_t> next_index(0);
    std::vector<std::thread> threads;
    
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&]() {
            while (true) {
                size_t j = next_index.fetch_add(1, std::memory_order_relaxed);
                if (j >= usuariosParaExplorar.size()) {
                    break;
                }
                recomendarParaUsuario(usuariosParaExplorar[j], out);
            }
        });
    }

    for (std::thread& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}
