#include "Recomendador.hpp"
#include "CalculadorDeSimilaridade.hpp"
#include "utilitarios.hpp" // Para a função dividir
#include <fstream>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <map>
#include <iostream>

Recomendador::Recomendador(GerenciadorDeDados& gerenciador, const Configuracao& config)
    : gerenciador(gerenciador), config(config) {}

void Recomendador::recomendarParaUsuarios(const std::string& arquivoExploracao, const std::string& arquivoSaida) {
    std::ifstream in(arquivoExploracao);
    std::ofstream out(arquivoSaida);

    if (!in.is_open() || !out.is_open()) {
        std::cerr << "Erro ao abrir arquivos de entrada/saida" << std::endl;
        return;
    }

    gerenciador.carregarNomesFilmes("dados/movies.csv");

    int usuarioId;
    while (in >> usuarioId) {
        const Usuario& perfilAtual = gerenciador.getUsuario(usuarioId);
        std::unordered_set<int> filmesVistos;
        for (const auto& [filme, _] : perfilAtual.getAvaliacoes()) {
            filmesVistos.insert(filme);
        }

        std::vector<std::pair<int, float>> similares;
        for (const auto& [outroId, outroUsuario] : gerenciador.getTodosUsuarios()) {
            if (outroId == usuarioId) continue;
            float sim = CalculadorDeSimilaridade::calcularSimilaridadeCosseno(perfilAtual.getAvaliacoes(), outroUsuario.getAvaliacoes());
            similares.push_back({outroId, sim});
        }

        std::sort(similares.begin(), similares.end(),
                  [](const auto& a, const auto& b) { return a.second > b.second; });

        std::map<int, float> somaNotas;
        std::map<int, int> contagem;

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
        for (const auto& [filme, soma] : somaNotas) {
            float media = soma / contagem[filme];
            candidatos.push_back({filme, media});
        }

        std::sort(candidatos.begin(), candidatos.end(),
                  [](const auto& a, const auto& b) { return a.second > b.second; });

        out << usuarioId;
        for (int i = 0; i < std::min(config.N_RECOMENDACOES, (int)candidatos.size()); ++i) {
            int filmeId = candidatos[i].first;
            try {
                out << " " << gerenciador.getNomeFilme(filmeId);
            } catch (const std::out_of_range& oor) {
                // Filme não encontrado, ignorar
            }
        }
        out << "\n";
    }

    in.close();
    out.close();
}


