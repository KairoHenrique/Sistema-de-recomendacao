#include "Preprocessador.hpp"
#include "GerenciadorDeDados.hpp" 
#include <fstream>
#include <unordered_set>
#include <charconv>
#include <iostream>
#include <random>
#include <algorithm>
#include <thread>
#include <future>

// Processa um pedaço do CSV de avaliações.
ContagemParcial Preprocessador::processarChunk(std::string_view chunk) {
    ContagemParcial contagem;
    contagem.filmesCounts.reserve(20000);  
    contagem.usuariosCounts.reserve(40000);

    while (!chunk.empty()) {
        int userId, movieId; 
        float rating;        
        
        // Extrai userId.
        auto p1 = chunk.find(",");
        if (p1 == std::string_view::npos) break;
        std::from_chars(chunk.data(), chunk.data() + p1, userId);
        chunk.remove_prefix(p1 + 1);

        // Extrai movieId.
        auto p2 = chunk.find(",");
        if (p2 == std::string_view::npos) break;
        std::from_chars(chunk.data(), chunk.data() + p2, movieId);
        chunk.remove_prefix(p2 + 1);

        // Extrai rating e avança para a próxima linha.
        auto p3 = chunk.find("\n");
        std::from_chars(chunk.data(), chunk.data() + p3, rating);
        chunk.remove_prefix(p3 != std::string_view::npos ? p3 + 1 : chunk.size());

        // Atualiza contagens e armazena avaliação.
        contagem.filmesCounts[movieId]++;
        contagem.usuariosCounts[userId]++;
        contagem.avaliacoesBrutas.emplace_back(userId, movieId, rating);
    }
    return contagem;
}

// Escreve os dados processados em arquivo binário.
void Preprocessador::escreverInputBin(const std::string& arquivoInput, const std::unordered_map<int, std::vector<std::pair<int, float>>>& dados) {
    std::ofstream out(arquivoInput, std::ios::binary);
    if (!out) {
        std::cerr << "Erro: Nao foi possivel criar o arquivo input binario." << std::endl;
        return;
    }

    size_t numUsuarios = dados.size();
    // Escreve total de usuários.
    out.write(reinterpret_cast<const char*>(&numUsuarios), sizeof(numUsuarios));

    // Para cada usuário, escreve ID e avaliações.
    for (const auto& [userId, avaliacoes] : dados) {
        out.write(reinterpret_cast<const char*>(&userId), sizeof(userId));
        size_t numAvaliacoes = avaliacoes.size();
        out.write(reinterpret_cast<const char*>(&numAvaliacoes), sizeof(numAvaliacoes));
        out.write(reinterpret_cast<const char*>(avaliacoes.data()), numAvaliacoes * sizeof(std::pair<int, float>));
    }
}

// Gera arquivo processado a partir do CSV de avaliações.
void Preprocessador::gerarInput(const std::string& arquivoCSV, const std::string& arquivoSaida) {
    std::string conteudo = GerenciadorDeDados().lerArquivoInteiro(arquivoCSV);
    
    const unsigned int numThreads = std::thread::hardware_concurrency();
    std::vector<std::future<ContagemParcial>> futuros;
    
    size_t chunkSize = conteudo.size() / numThreads;
    size_t inicio = conteudo.find("\n") + 1; // Pula o cabeçalho.

    // Divide em chunks e processa em paralelo.
    for (unsigned int i = 0; i < numThreads; ++i) {
        size_t fim = (i == numThreads - 1) ? conteudo.size() : inicio + chunkSize;
        if (fim < conteudo.size()) {
            fim = conteudo.find("\n", fim);
            if (fim == std::string::npos) fim = conteudo.size();
        }
        std::string_view chunk(conteudo.data() + inicio, fim - inicio);
        futuros.push_back(std::async(std::launch::async, Preprocessador::processarChunk, chunk));
        inicio = fim + 1;
        if (inicio >= conteudo.size()) break;
    }

    std::unordered_map<int, int> filmesCounts;
    std::unordered_map<int, int> usuariosCounts;
    std::vector<std::tuple<int, int, float>> todasAvaliacoesBrutas;
    filmesCounts.reserve(70000);
    usuariosCounts.reserve(170000);

    // Combina resultados das threads.
    for (auto& f : futuros) {
        ContagemParcial parcial = f.get();
        for (const auto& [filmeId, count] : parcial.filmesCounts) filmesCounts[filmeId] += count;
        for (const auto& [userId, count] : parcial.usuariosCounts) usuariosCounts[userId] += count;
        todasAvaliacoesBrutas.insert(todasAvaliacoesBrutas.end(),
                                     std::make_move_iterator(parcial.avaliacoesBrutas.begin()),
                                     std::make_move_iterator(parcial.avaliacoesBrutas.end()));
    }

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

    std::unordered_map<int, std::vector<std::pair<int, float>>> finalData;
    finalData.reserve(validUsers.size());
    
    // Filtra avaliações válidas.
    for (const auto& avaliacao : todasAvaliacoesBrutas) {
        int userId = std::get<0>(avaliacao);
        int movieId = std::get<1>(avaliacao);
        float rating = std::get<2>(avaliacao);

        if (validUsers.count(userId) && validMovies.count(movieId)) {
            finalData[userId].emplace_back(movieId, rating);
        }
    }

    // Escreve arquivo binário final.
    Preprocessador::escreverInputBin(arquivoSaida, finalData);
}

// Gera arquivo binário de usuários para exploração.
void Preprocessador::gerarExplore(GerenciadorDeDados& gerenciador, const std::string& exploreBin, int quantidade) {
    const auto& todosUsuarios = gerenciador.getTodosUsuarios();
    if (todosUsuarios.empty()) {
        std::cerr << "Nenhum usuario carregado para gerar o arquivo de exploracao." << std::endl;
        return;
    }

    std::vector<int> userIds;
    userIds.reserve(todosUsuarios.size());
    for(const auto& [id, _] : todosUsuarios) {
        userIds.push_back(id);
    }

    // Embaralha IDs para seleção aleatória.
    std::mt19937 g(std::random_device{}());
    std::shuffle(userIds.begin(), userIds.end(), g);

    std::ofstream out(exploreBin, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "Erro fatal: nao foi possivel criar " << exploreBin << std::endl;
        exit(1);
    }
    
    // Escreve quantidade de usuários selecionados.
    int limite = std::min(quantidade, static_cast<int>(userIds.size()));
    out.write(reinterpret_cast<const char*>(&limite), sizeof(limite));

    // Escreve IDs selecionados.
    for (int i = 0; i < limite; ++i) {
        out.write(reinterpret_cast<const char*>(&userIds[i]), sizeof(userIds[i]));
    }
}
