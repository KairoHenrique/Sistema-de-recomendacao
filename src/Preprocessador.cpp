#include "Preprocessador.hpp"
#include "utilitarios.hpp"
#include "GerenciadorDeDados.hpp" 
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
#include <future>

// Estrutura para armazenar contagens parciais de filmes e usuários,
// e avaliações brutas durante o processamento paralelo.
struct ContagemParcial {
    std::unordered_map<int, int> filmesCounts;   // Contagem de avaliações por filme.
    std::unordered_map<int, int> usuariosCounts; // Contagem de avaliações por usuário.
    std::vector<std::tuple<int, int, float>> avaliacoesBrutas; // Avaliações lidas do CSV.
};

// Processa um pedaço (chunk) do arquivo CSV de avaliações.
// chunk: Uma string_view contendo uma parte do arquivo CSV.
// Retorna uma estrutura ContagemParcial com os dados processados do chunk.
ContagemParcial processarChunk(std::string_view chunk) {
    ContagemParcial contagem;
    contagem.filmesCounts.reserve(20000);   // Pré-aloca espaço para otimização.
    contagem.usuariosCounts.reserve(40000); // Pré-aloca espaço para otimização.

    // Itera sobre as linhas do chunk.
    while (!chunk.empty()) {
        int userId, movieId;
        float rating;        // Nota da avaliação.
        
        // Extrai o userId.
        auto p1 = chunk.find(",");
        if (p1 == std::string_view::npos) break;
        std::from_chars(chunk.data(), chunk.data() + p1, userId);
        chunk.remove_prefix(p1 + 1);

        // Extrai o movieId.
        auto p2 = chunk.find(",");
        if (p2 == std::string_view::npos) break;
        std::from_chars(chunk.data(), chunk.data() + p2, movieId);
        chunk.remove_prefix(p2 + 1);

        // Extrai o rating e avança para a próxima linha.
        auto p3 = chunk.find("\n");
        std::from_chars(chunk.data(), chunk.data() + p3, rating);
        chunk.remove_prefix(p3 != std::string_view::npos ? p3 + 1 : chunk.size());

        // Incrementa as contagens e armazena a avaliação bruta.
        contagem.filmesCounts[movieId]++;
        contagem.usuariosCounts[userId]++;
        contagem.avaliacoesBrutas.emplace_back(userId, movieId, rating);
    }
    return contagem;
}

// Escreve os dados processados em um arquivo binário de cache.
// arquivoCache: Caminho para o arquivo de cache binário de saída.
// dados: Mapa contendo os IDs dos usuários e suas avaliações.
void escreverCacheBinario(const std::string& arquivoCache, const std::unordered_map<int, std::vector<std::pair<int, float>>>& dados) {
    std::cout << "  - Escrevendo cache binario em " << arquivoCache << "..." << std::endl;
    std::ofstream out(arquivoCache, std::ios::binary); // Abre o arquivo em modo binário.
    if (!out) {
        std::cerr << "Erro: Nao foi possivel criar o arquivo de cache binario." << std::endl;
        return;
    }

    size_t numUsuarios = dados.size();
    // Escreve o número total de usuários no início do arquivo.
    out.write(reinterpret_cast<const char*>(&numUsuarios), sizeof(numUsuarios));

    // Itera sobre cada usuário e suas avaliações.
    for (const auto& [userId, avaliacoes] : dados) {
        // Escreve o ID do usuário.
        out.write(reinterpret_cast<const char*>(&userId), sizeof(userId));
        
        size_t numAvaliacoes = avaliacoes.size();
        // Escreve o número de avaliações para o usuário atual.
        out.write(reinterpret_cast<const char*>(&numAvaliacoes), sizeof(numAvaliacoes));
        
        // Escreve todas as avaliações do usuário de uma vez.
        out.write(reinterpret_cast<const char*>(avaliacoes.data()), numAvaliacoes * sizeof(std::pair<int, float>));
    }
}

// Gera o arquivo de entrada processado a partir do CSV de avaliações.
// arquivoCSV: Caminho para o arquivo CSV de avaliações
// arquivoSaida: Caminho para o arquivo de saída
void Preprocessador::gerarInput(const std::string& arquivoCSV, const std::string& arquivoSaida) {
    std::cout << "Iniciando pre-processamento (gerarInput)" << std::endl;
    
    std::string conteudo = lerArquivoInteiro(arquivoCSV); // Lê o conteúdo completo do CSV.
    
    const unsigned int numThreads = std::thread::hardware_concurrency(); // Obtém o número de threads.
    std::cout << "  - Usando " << numThreads << " threads para o processamento." << std::endl;
    std::vector<std::future<ContagemParcial>> futuros; // Vetor para armazenar os resultados das threads.
    
    size_t chunkSize = conteudo.size() / numThreads; // Calcula o tamanho de cada chunk.
    size_t inicio = 0;

    inicio = conteudo.find("\n") + 1; // Pula a linha de cabeçalho do CSV.

    // Divide o conteúdo em chunks e lança threads para processar cada um.
    for (unsigned int i = 0; i < numThreads; ++i) {
        size_t fim = (i == numThreads - 1) ? conteudo.size() : inicio + chunkSize;
        // Ajusta o fim do chunk para não cortar uma linha no meio.
        if (fim < conteudo.size()) {
            fim = conteudo.find("\n", fim);
            if (fim == std::string::npos) fim = conteudo.size();
        }
        std::string_view chunk(conteudo.data() + inicio, fim - inicio);
        futuros.push_back(std::async(std::launch::async, processarChunk, chunk)); // Inicia a thread assíncrona.
        inicio = fim + 1;
        if (inicio >= conteudo.size()) break;
    }

    std::cout << "  - Agregando resultados dos threads..." << std::endl;
    std::unordered_map<int, int> filmesCounts;   // Contagem total de filmes.
    std::unordered_map<int, int> usuariosCounts; // Contagem total de usuários.
    std::vector<std::tuple<int, int, float>> todasAvaliacoesBrutas; // Todas as avaliações brutas.
    filmesCounts.reserve(70000);   // Pré-aloca espaço.
    usuariosCounts.reserve(170000); // Pré-aloca espaço.

    // Agrega os resultados de todas as threads.
    for (auto& f : futuros) {
        ContagemParcial parcial = f.get(); // Obtém o resultado da thread.
        // Combina as contagens de filmes e usuários.
        for (const auto& [filmeId, count] : parcial.filmesCounts) filmesCounts[filmeId] += count;
        for (const auto& [userId, count] : parcial.usuariosCounts) usuariosCounts[userId] += count;
        // Move as avaliações brutas para o vetor principal.
        todasAvaliacoesBrutas.insert(todasAvaliacoesBrutas.end(),
                                     std::make_move_iterator(parcial.avaliacoesBrutas.begin()),
                                     std::make_move_iterator(parcial.avaliacoesBrutas.end()));
    }

    std::cout << "Criando filtros de filmes e usuarios validos" << std::endl;
    std::unordered_set<int> validMovies; // Conjunto de filmes válidos (com >= 50 avaliações).
    validMovies.reserve(filmesCounts.size());
    for (const auto& [filmeId, count] : filmesCounts) {
        if (count >= 50) validMovies.insert(filmeId);
    }

    std::unordered_set<int> validUsers; // Conjunto de usuários válidos (com >= 50 avaliações).
    validUsers.reserve(usuariosCounts.size());
    for (const auto& [userId, count] : usuariosCounts) {
        if (count >= 50) validUsers.insert(userId);
    }
    
    filmesCounts.clear();  
    usuariosCounts.clear(); 

    std::unordered_map<int, std::vector<std::pair<int, float>>> finalData; // Dados finais agrupados por usuário.
    finalData.reserve(validUsers.size());
    
    // Filtra e agrupa as avaliações válidas.
    for (const auto& avaliacao : todasAvaliacoesBrutas) {
        int userId = std::get<0>(avaliacao);
        int movieId = std::get<1>(avaliacao);
        float rating = std::get<2>(avaliacao);

        if (validUsers.count(userId) && validMovies.count(movieId)) {
            finalData[userId].emplace_back(movieId, rating);
        }
    }

    // Abre o arquivo de saída para escrita.
    std::ofstream out(arquivoSaida, std::ios::binary); 
    if (!out.is_open()) {
        std::cerr << "Erro fatal: nao foi possivel criar " << arquivoSaida << std::endl;
        exit(1);
    }
    out.exceptions(std::ofstream::failbit | std::ofstream::badbit); // Habilita exceções para erros de escrita.

    // Escreve o número de usuários no início do arquivo binário.
    size_t numUsuariosFinal = finalData.size();
    out.write(reinterpret_cast<const char*>(&numUsuariosFinal), sizeof(numUsuariosFinal));

    // Escreve os dados de cada usuário no arquivo binário.
    for (auto& [userId, avaliacoes] : finalData) {
        if (avaliacoes.size() >= 50) { // Garante que o usuário tem pelo menos 50 avaliações válidas.
            out.write(reinterpret_cast<const char*>(&userId), sizeof(userId));
            size_t numAvaliacoes = avaliacoes.size();
            out.write(reinterpret_cast<const char*>(&numAvaliacoes), sizeof(numAvaliacoes));
            out.write(reinterpret_cast<const char*>(avaliacoes.data()), numAvaliacoes * sizeof(std::pair<int, float>));
        }
    }
    std::cout << "Pre-processamento (gerarInput) finalizado." << std::endl;
}

// Gera o arquivo de usuários para exploração (explore.bin) em formato binário.
// gerenciador: Referência ao GerenciadorDeDados para acessar os usuários.
// exploreDat: Caminho para o arquivo de saída explore.bin. (mudar nome da funcao)
// quantidade: Número de usuários a serem incluídos no arquivo explore.bin.
void Preprocessador::gerarExplore(GerenciadorDeDados& gerenciador, const std::string& exploreDat, int quantidade) {
    std::cout << "Gerando arquivo de exploracao a partir dos dados carregados" << std::endl;
    
    const auto& todosUsuarios = gerenciador.getTodosUsuarios(); // Obtém todos os usuários carregados.
    if (todosUsuarios.empty()) {
        std::cerr << "Nenhum usuario carregado para gerar o arquivo de exploracao." << std::endl;
        return;
    }

    std::vector<int> userIds; // Vetor para armazenar os IDs dos usuários.
    userIds.reserve(todosUsuarios.size());
    for(const auto& [id, _] : todosUsuarios) {
        userIds.push_back(id);
    }

    // Embaralha os IDs dos usuários para seleção aleatória.
    std::mt19937 g(std::random_device{}());
    std::shuffle(userIds.begin(), userIds.end(), g);

    // Abre o arquivo explore.bin para escrita em modo binário.
    std::ofstream out(exploreDat, std::ios::binary); 
    if (!out.is_open()) {
        std::cerr << "Erro fatal: nao foi possivel criar " << exploreDat << std::endl;
        exit(1);
    }
    
    // Escreve o número de usuários selecionados no início do arquivo binário.
    int limite = std::min(quantidade, static_cast<int>(userIds.size()));
    out.write(reinterpret_cast<const char*>(&limite), sizeof(limite));

    // Escreve os IDs dos usuários selecionados no arquivo binário.
    for (int i = 0; i < limite; ++i) {
        out.write(reinterpret_cast<const char*>(&userIds[i]), sizeof(userIds[i]));
    }
    std::cout << "Arquivo de explore gerado." << std::endl;
}


