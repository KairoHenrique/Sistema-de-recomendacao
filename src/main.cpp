#include <chrono>
#include <iostream>
#include <thread>
#include "Preprocessador.hpp"
#include "Recomendador.hpp"
#include "Configuracao.hpp"
#include "GerenciadorDeDados.hpp"
#include "utilitarios.hpp"

int main() {
    std::cout << "Iniciando sistema de recomendacao..." << std::endl;
    auto inicio = std::chrono::high_resolution_clock::now();
    
    Configuracao config;
    GerenciadorDeDados gerenciador;
    Preprocessador preprocessador;
    Recomendador recomendador(gerenciador, config);
    
    int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;
    std::cout << "Usando " << numThreads << " threads para recomendacao." << std::endl;

    preprocessador.gerarInput("dados/ratings.csv", "dados/input.dat");
    preprocessador.gerarExplore("dados/input.dat", "dados/explore.dat", config.N_USUARIOS_EXPLORAR);
    gerenciador.carregarDados("dados/input.dat");
    gerenciador.carregarNomesFilmes("dados/movies.csv");
    recomendador.recomendarParaUsuarios("dados/explore.dat", "resultados/output.dat", numThreads);

    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;
    std::cout << "Finalizando sistema de recomendacao." << std::endl;
    std::cout << "Tempo total de execucao: " << duracao.count() << " segundos" << std::endl;
    return 0;
}
