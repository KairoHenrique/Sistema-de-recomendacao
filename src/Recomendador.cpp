#include "Recomendador.hpp"
#include "CalculadorDeSimilaridade.hpp"
#include "utilitarios.hpp" // Para a função dividir
#include <fstream>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <thread>
#include <mutex>

Recomendador::Recomendador(GerenciadorDeDados& gerenciador, const Configuracao& config)
    : gerenciador(gerenciador), config(config) {}

void Recomendador::recomendarParaUsuario(int usuarioId, std::ofstream& outFile) {
    const Usuario& perfilAtual = gerenciador.getUsuario(usuarioId);
    std::unordered_set<int> filmesVistos;
    for (const auto& [filme, _] : perfilAtual.getAvaliacoes()) {
        filmesVistos.insert(filme);
    }

    std::vector<std::pair<int, float>> similares;
    similares.reserve(gerenciador.getTodosUsuarios().size()); // Pré-alocação
    for (const auto& [outroId, outroUsuario] : gerenciador.getTodosUsuarios()) {
        if (outroId == usuarioId) continue;
        float sim = CalculadorDeSimilaridade::calcularSimilaridadeCosseno(perfilAtual.getAvaliacoes(), outroUsuario.getAvaliacoes());
        similares.push_back({outroId, sim});
    }

    std::sort(similares.begin(), similares.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    std::unordered_map<int, float> somaNotas; // Alterado para unordered_map
    std::unordered_map<int, int> contagem;    // Alterado para unordered_map

    for (int i = 0; i < std::min(config.K_VIZINHOS, (int)similares.size()); ++i) {
        int vizinhoId = similares[i].first;
        const Usuario& vizinhoUsuario = gerenciador.getUsuario(vizinhoId);
        const auto& filmesVizinho = vizinhoUsuario.getAvaliacoes();

        for (const auto& [filme, nota] : filmesVizinho) {
            if (filmesVistos.count(filme) == 0) {
                somaNotas[filme] += nota;
                contagem[filme]++;
            }
        }
    }

    std::vector<std::pair<int, float>> candidatos;
    candidatos.reserve(somaNotas.size()); // Pré-alocação
    for (const auto& [filme, soma] : somaNotas) {
        float media = soma / contagem[filme];
        candidatos.push_back({filme, media});
    }

    std::sort(candidatos.begin(), candidatos.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    std::lock_guard<std::mutex> lock(mtx);
    outFile << usuarioId;
    for (int i = 0; i < std::min(config.N_RECOMENDACOES, (int)candidatos.size()); ++i) {
        int filmeId = candidatos[i].first;
        try {
            outFile << " " << gerenciador.getNomeFilme(filmeId);
        } catch (const std::out_of_range& oor) {
            // Filme não encontrado, ignorar
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


