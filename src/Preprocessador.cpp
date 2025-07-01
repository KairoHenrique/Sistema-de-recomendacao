#include "Preprocessador.hpp"
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <string_view>
#include <charconv>
#include <iostream>
#include <random>
#include <algorithm>
#include <thread>
#include <future> // Para std::async e std::future

// Estrutura para manter os resultados de cada thread
struct ContagemParcial {
    std::unordered_map<int, int> filmesCounts;
    std::unordered_map<int, int> usuariosCounts;
};

// Função externa definida em GerenciadorDeDados.cpp
extern std::string lerArquivoInteiro(const std::string& caminho);

// Função que cada thread executará para processar seu pedaço do arquivo
ContagemParcial processarChunk(std::string_view chunk) {
    ContagemParcial contagem;
    contagem.filmesCounts.reserve(20000); // Estimativas
    contagem.usuariosCounts.reserve(40000);

    while (!chunk.empty()) {
        int userId, movieId;
        
        auto p1 = chunk.find(',');
        if (p1 == std::string_view::npos) break;
        std::from_chars(chunk.data(), chunk.data() + p1, userId);
        chunk.remove_prefix(p1 + 1);

        auto p2 = chunk.find(',');
        if (p2 == std::string_view::npos) break;
        std::from_chars(chunk.data(), chunk.data() + p2, movieId);
        
        auto p3 = chunk.find('\n');
        chunk.remove_prefix(p3 != std::string_view::npos ? p3 + 1 : chunk.size());

        contagem.filmesCounts[movieId]++;
        contagem.usuariosCounts[userId]++;
    }
    return contagem;
}


void Preprocessador::gerarInput(const std::string& arquivoCSV, const std::string& arquivoSaida) {
    std::cout << "Iniciando pre-processamento PARALELO (gerarInput)..." << std::endl;
    
    std::string conteudo = lerArquivoInteiro(arquivoCSV);
    
    const unsigned int numThreads = std::thread::hardware_concurrency();
    std::cout << "  - Usando " << numThreads << " threads para o processamento." << std::endl;
    std::vector<std::future<ContagemParcial>> futuros;
    
    size_t chunkSize = conteudo.size() / numThreads;
    size_t inicio = 0;

    // Ignora cabeçalho
    inicio = conteudo.find('\n') + 1;

    // --- Lançamento dos Threads ---
    for (unsigned int i = 0; i < numThreads; ++i) {
        size_t fim = (i == numThreads - 1) ? conteudo.size() : inicio + chunkSize;
        
        // Ajusta 'fim' para a próxima quebra de linha para não cortar uma linha ao meio
        if (fim < conteudo.size()) {
            fim = conteudo.find('\n', fim);
            if (fim == std::string::npos) {
                fim = conteudo.size();
            }
        }

        std::string_view chunk(conteudo.data() + inicio, fim - inicio);
        futuros.push_back(std::async(std::launch::async, processarChunk, chunk));
        
        inicio = fim + 1;
        if (inicio >= conteudo.size()) {
            break;
        }
    }

    // --- Agregação (Reduce) ---
    std::cout << "  - Agregando resultados dos threads..." << std::endl;
    std::unordered_map<int, int> filmesCounts;
    std::unordered_map<int, int> usuariosCounts;
    filmesCounts.reserve(70000);
    usuariosCounts.reserve(170000);

    for (auto& f : futuros) {
        ContagemParcial parcial = f.get();
        for (const auto& [filmeId, count] : parcial.filmesCounts) {
            filmesCounts[filmeId] += count;
        }
        for (const auto& [userId, count] : parcial.usuariosCounts) {
            usuariosCounts[userId] += count;
        }
    }

    // --- O restante do processo é o mesmo, mas agora muito mais rápido ---
    // pois opera em dados já agregados.
    
    std::cout << "  - Criando filtros de filmes e usuarios validos..." << std::endl;
    std::unordered_set<int> validMovies;
    validMovies.reserve(filmesCounts.size());
    for (const auto& [filmeId, count] : filmesCounts) {
        if (count >= 50) validMovies.insert(filmeId);
    }

    std::unordered_set<int> validUsers;
    validUsers.reserve(usuariosCounts.size());
    for (const auto& [userId, count] : usuariosCounts) {
        if (count >= 50) validUsers.insert(userId);
    }
    
    filmesCounts.clear();
    usuariosCounts.clear();

    std::cout << "  - Passagem final: agrupando e escrevendo dados validos..." << std::endl;
    std::unordered_map<int, std::vector<std::pair<int, float>>> finalData;
    finalData.reserve(validUsers.size());
    
    std::string_view sv_final(conteudo);
    sv_final.remove_prefix(sv_final.find('\n') + 1); // Pula cabeçalho

    while (!sv_final.empty()) {
        int userId, movieId;
        float rating;

        auto p1 = sv_final.find(','); if (p1 == std::string_view::npos) break;
        std::from_chars(sv_final.data(), sv_final.data() + p1, userId);
        sv_final.remove_prefix(p1 + 1);

        auto p2 = sv_final.find(','); if (p2 == std::string_view::npos) break;
        std::from_chars(sv_final.data(), sv_final.data() + p2, movieId);
        sv_final.remove_prefix(p2 + 1);

        auto p3 = sv_final.find('\n');
        std::from_chars(sv_final.data(), sv_final.data() + p3, rating);
        sv_final.remove_prefix(p3 != std::string_view::npos ? p3 + 1 : sv_final.size());
        
        if (validUsers.count(userId) && validMovies.count(movieId)) {
            finalData[userId].emplace_back(movieId, rating);
        }
    }

    std::ofstream out(arquivoSaida);
    if (!out.is_open()) {
        std::cerr << "Erro fatal: nao foi possivel criar " << arquivoSaida << std::endl;
        exit(1);
    }
    out.exceptions(std::ofstream::failbit | std::ofstream::badbit);

    for (const auto& [userId, avaliacoes] : finalData) {
        if (avaliacoes.size() >= 50) {
            out << userId;
            for (const auto& [filmeId, nota] : avaliacoes) {
                out << " " << filmeId << ":" << nota;
            }
            out << "\n";
        }
    }
    std::cout << "Pre-processamento PARALELO (gerarInput) finalizado." << std::endl;
}

// A função gerarExplore permanece a mesma, pois é rápida o suficiente.
void Preprocessador::gerarExplore(const std::string& inputDat, const std::string& exploreDat, int quantidade) {
    std::cout << "Gerando arquivo de exploracao..." << std::endl;
    std::ifstream in(inputDat);
    if (!in.is_open()) {
        std::cerr << "Erro fatal: nao foi possivel abrir " << inputDat << std::endl;
        exit(1);
    }

    std::vector<int> userIds;
    userIds.reserve(170000);
    std::string linha;
    while (std::getline(in, linha)) {
        if (linha.empty()) continue;
        int userId;
        auto pos_espaco = linha.find(' ');
        std::from_chars(linha.data(), linha.data() + pos_espaco, userId);
        userIds.push_back(userId);
    }

    if (userIds.empty()) return;

    std::mt19937 g(std::random_device{}());
    std::shuffle(userIds.begin(), userIds.end(), g);

    std::ofstream out(exploreDat);
    if (!out.is_open()) {
        std::cerr << "Erro fatal: nao foi possivel criar " << exploreDat << std::endl;
        exit(1);
    }
    
    int limite = std::min(quantidade, static_cast<int>(userIds.size()));
    for (int i = 0; i < limite; ++i) {
        out << userIds[i] << "\n";
    }
    std::cout << "Arquivo de exploracao gerado." << std::endl;
}
