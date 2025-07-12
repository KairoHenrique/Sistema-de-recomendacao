#include <chrono>
#include <iostream>
#include <thread>
#include <string>

#include "Preprocessador.hpp"
#include "Recomendador.hpp"
#include "Configuracao.hpp"
#include "GerenciadorDeDados.hpp"

int main() {
    // Otimiza I/O desabilitando sincronização e desvinculando cin de cout.
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // Marca o início da execução.
    auto inicio_total = std::chrono::high_resolution_clock::now();
    
    Configuracao config;
    GerenciadorDeDados gerenciador;

    // Gera arquivo binário.
    Preprocessador::gerarInput("dados/ratings.csv", "dados/input.bin");

    gerenciador.carregarDadosDeCacheBinario("dados/input.bin");

    gerenciador.carregarNomesFilmes("dados/movies.csv");
    
    Preprocessador::gerarExplore(gerenciador, "dados/explore.bin", config.N_USUARIOS_EXPLORAR);

    // Inicia recomendação com número de threads disponíveis (default 4).
    Recomendador recomendador(gerenciador, config);
    int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;

    // Executa recomendações para usuários da exploração.
    recomendador.recomendarParaUsuarios("dados/explore.bin", "resultados/output.dat", numThreads);

    // Calcula e exibe tempo total de execução.
    auto fim_total = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao_total = fim_total - inicio_total;
    std::cout << "Tempo total de execucao: " << duracao_total.count() << " segundos" << std::endl;
    
    return 0;
}
