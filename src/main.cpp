#include <chrono>
#include <iostream>
#include <thread>
#include <string>

#include "Preprocessador.hpp"
#include "Recomendador.hpp"
#include "Configuracao.hpp"
#include "GerenciadorDeDados.hpp"

int main() {
    // Desabilita a sincronização de C++ streams com C stdio e desvincula cin de cout para otimização de I/O.
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // Registra o tempo de início total da execução do programa.
    auto inicio_total = std::chrono::high_resolution_clock::now();
    
    Configuracao config;
    GerenciadorDeDados gerenciador;

    // Gera o arquivo input.bin.
    Preprocessador::gerarInput("dados/ratings.csv", "dados/input.bin");

    gerenciador.carregarDadosDeCacheBinario("dados/input.bin");

    // Carrega os nomes dos filmes.
    gerenciador.carregarNomesFilmes("dados/movies.csv");
    
    // Gera o arquivo explore.bin com base nos usuários a serem explorados.
    Preprocessador::gerarExplore(gerenciador, "dados/explore.bin", config.N_USUARIOS_EXPLORAR);

    // Inicia o processo de recomendação.
    Recomendador recomendador(gerenciador, config);
    int numThreads = std::thread::hardware_concurrency(); // Obtém o número de threads disponíveis.
    if (numThreads == 0) numThreads = 4;

    // Realiza as recomendações para os usuários e salva no arquivo de saída.
    recomendador.recomendarParaUsuarios("dados/explore.bin", "resultados/output.dat", numThreads);

    auto fim_total = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao_total = fim_total - inicio_total;

    std::cout << "Tempo total de execucao: " << duracao_total.count() << " segundos" << std::endl;
    
    return 0;
}
