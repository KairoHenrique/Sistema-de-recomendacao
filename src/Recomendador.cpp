#include "Recomendador.hpp"
#include "CalculadorDeSimilaridade.hpp"
#include <algorithm>
#include <atomic>
#include <vector>
#include <random>
#include <unordered_set>
#include <iostream>
#include <string_view>

// Inicializa o recomendador com o gerenciador de dados e as configurações.
// gerenciador: Referência ao objeto GerenciadorDeDados para acesso aos dados de usuários e filmes.
// config: Referência ao objeto Configuracao com os parâmetros do sistema.
Recomendador::Recomendador(GerenciadorDeDados& gerenciador, const Configuracao& config)
    : gerenciador(gerenciador), config(config) {}

// Realiza a recomendação de filmes para um usuário específico.
// usuarioId: ID do usuário para o qual as recomendações serão geradas.
// outFile: Stream de saída para escrever as recomendações
void Recomendador::recomendarParaUsuario(int usuarioId, std::ostream& outFile) {
    // Obtém o perfil do usuário atual e sua magnitude (norma do vetor de avaliações).
    const Usuario& perfilAtual = gerenciador.getUsuario(usuarioId);
    float mag1 = gerenciador.getMagnitude(usuarioId);

    // Cria um conjunto de filmes já vistos pelo usuário para evitar recomendá-los novamente.
    std::unordered_set<int> filmesVistos;
    filmesVistos.reserve(perfilAtual.getAvaliacoes().size());
    for (const auto& [filmeId, _] : perfilAtual.getAvaliacoes()) {
        filmesVistos.insert(filmeId);
    }

    // Obtém todos os usuários do sistema.
    const auto& todosUsuarios = gerenciador.getTodosUsuarios();
    // Vetor para armazenar IDs de usuários candidatos a vizinhos, thread-local para evitar contenção.
    thread_local std::vector<int> candidatosIds;
    candidatosIds.clear();
    candidatosIds.reserve(todosUsuarios.size());
    // Popula o vetor com IDs de todos os usuários, exceto o usuário atual.
    for (const auto& [id, _] : todosUsuarios) {
        if (id != usuarioId) {
            candidatosIds.push_back(id);
        }
    }

    // Embaralha os IDs dos candidatos para selecionar uma amostra aleatória.
    thread_local std::mt19937 g(std::random_device{}());
    std::shuffle(candidatosIds.begin(), candidatosIds.end(), g);
    
    // Define o tamanho da amostra de usuários a serem considerados como vizinhos.
    int amostra = std::min(5000, static_cast<int>(candidatosIds.size()));
    
    // Vetor para armazenar usuários similares e suas similaridades, thread-local.
    thread_local std::vector<std::pair<int, float>> similares;
    similares.clear();
    similares.reserve(amostra);

    // Calcula a similaridade de cosseno entre o usuário atual e uma amostra de outros usuários.
    for (int i = 0; i < amostra; ++i) {
        int outroId = candidatosIds[i];
        const Usuario& outroUsuario = gerenciador.getUsuario(outroId);
        float mag2 = gerenciador.getMagnitude(outroId);
        
        float sim = CalculadorDeSimilaridade::calcularSimilaridadeCosseno(
            perfilAtual.getAvaliacoes(), mag1,
            outroUsuario.getAvaliacoes(), mag2
        );
        // Se a similaridade for positiva, adiciona à lista de similares.
        if (sim > 0.0f) {
            similares.push_back({outroId, sim});
        }
    }

    // Seleciona os K vizinhos mais similares.
    int k_count = std::min(config.K_VIZINHOS, static_cast<int>(similares.size()));
    std::partial_sort(similares.begin(), similares.begin() + k_count, similares.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    // Mapas para acumular a soma das notas ponderadas e a soma dos pesos (similaridades) por filme.
    thread_local std::unordered_map<int, float> somaNotas;
    thread_local std::unordered_map<int, float> somaPesos;
    somaNotas.clear();
    somaPesos.clear();

    // Para cada um dos K vizinhos mais similares, calcula as notas preditas para filmes não vistos.
    for (int i = 0; i < k_count; ++i) {
        const auto& [vizinhoId, sim] = similares[i];
        const Usuario& vizinhoUsuario = gerenciador.getUsuario(vizinhoId);
        
        for (const auto& [filmeId, nota] : vizinhoUsuario.getAvaliacoes()) {
            // Se o filme não foi visto pelo usuário atual, contribui para a predição.
            if (filmesVistos.find(filmeId) == filmesVistos.end()) {
                somaNotas[filmeId] += sim * nota;
                somaPesos[filmeId] += sim;
            }
        }
    }

    // Vetor para armazenar os filmes candidatos a recomendação e suas notas preditas.
    thread_local std::vector<std::pair<int, float>> candidatos;
    candidatos.clear();
    candidatos.reserve(somaNotas.size());
    // Calcula a nota predita para cada filme.
    for (const auto& [filme, soma] : somaNotas) {
        if (somaPesos.count(filme) && somaPesos.at(filme) > 0.0f) {
            candidatos.push_back({filme, soma / somaPesos.at(filme)});
        }
    }

    // Seleciona os N filmes com as maiores notas preditas para recomendação.
    int n_count = std::min(config.N_RECOMENDACOES, static_cast<int>(candidatos.size()));
    if (n_count > 0) {
        std::partial_sort(candidatos.begin(), candidatos.begin() + n_count, candidatos.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });
    }

    // Protege a escrita no arquivo de saída com um mutex, pois múltiplas threads podem escrever.
    std::lock_guard<std::mutex> lock(mtx);
    outFile << usuarioId; // Escreve o ID do usuário.
    // Escreve os filmes recomendados (ID e nome).
    for (int i = 0; i < n_count; ++i) {
        int filmeId = candidatos[i].first;
        std::string_view nomeFilme = gerenciador.getNomeFilme(filmeId); // Obtém o nome do filme.
        outFile << " " << filmeId << ":";
        if (!nomeFilme.empty()) {
            outFile << nomeFilme;
        } else {
            outFile << "NomeNaoEncontrado"; // Caso o nome do filme não seja encontrado.
        }
    }
    outFile << "\n"; // Nova linha para o próximo usuário.
}

// Realiza a recomendação de filmes para múltiplos usuários em paralelo.
// arquivoExploracao: Caminho para o arquivo contendo os IDs dos usuários a serem explorados.
// arquivoSaida: Caminho para o arquivo onde as recomendações serão salvas.
// numThreads: Número de threads a serem usadas para o processamento paralelo.
void Recomendador::recomendarParaUsuarios(const std::string& arquivoExploracao, const std::string& arquivoSaida, int numThreads) {
    std::ifstream in(arquivoExploracao, std::ios::binary); // Abre o arquivo de exploração para leitura em modo binário.
    if (!in.is_open()) {
        std::cerr << "Erro fatal: nao foi possivel abrir arquivo de exploracao: " << arquivoExploracao << std::endl;
        return;
    }

    std::vector<int> usuariosParaExplorar; // Vetor para armazenar os IDs dos usuários a serem processados.
    int numUsuariosParaExplorar; // Número de usuários a serem explorados.
    // Lê o número de usuários do arquivo binário.
    in.read(reinterpret_cast<char*>(&numUsuariosParaExplorar), sizeof(numUsuariosParaExplorar));
    usuariosParaExplorar.resize(numUsuariosParaExplorar); // Redimensiona o vetor para o número correto de usuários.
    // Lê todos os IDs dos usuários do arquivo binário de uma vez.
    in.read(reinterpret_cast<char*>(usuariosParaExplorar.data()), numUsuariosParaExplorar * sizeof(int));
    in.close();
    
    std::ofstream out(arquivoSaida); // Abre o arquivo de saída para escrita.
    if (!out.is_open()) {
        std::cerr << "Erro fatal: nao foi possivel criar arquivo de saida: " << arquivoSaida << std::endl;
        return;
    }

    std::atomic<size_t> next_index(0); // Índice atômico para controlar qual usuário será processado em seguida.
    std::vector<std::thread> threads; // Vetor para armazenar as threads de execução.
    
    // Cria e lança as threads.
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&]() {
            while (true) {
                // Obtém o próximo índice de usuário a ser processado de forma atômica.
                size_t j = next_index.fetch_add(1, std::memory_order_relaxed);
                if (j >= usuariosParaExplorar.size()) {
                    break; // Sai do loop se todos os usuários foram processados.
                }
                // Chama a função de recomendação para o usuário atual.
                recomendarParaUsuario(usuariosParaExplorar[j], out);
            }
        });
    }

    // Espera todas as threads terminarem a execução.
    for (std::thread& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}


