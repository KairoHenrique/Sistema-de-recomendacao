#include <iostream>
#include <chrono>
#include <thread>
#include "Preprocessador.hpp"
#include "Recomendador.hpp"
#include "Configuracao.hpp"
#include "GerenciadorDeDados.hpp"
#include "utilitarios.hpp"

int main() {
    auto inicio = std::chrono::high_resolution_clock::now();

    // Instancia as classes
    Configuracao config;
    GerenciadorDeDados gerenciador;
    Preprocessador preprocessador;
    Recomendador recomendador(gerenciador, config);

    // Define o número de threads a serem usadas
    // Pode ser ajustado com base no número de núcleos do processador ou configurado externamente
    int numThreads = std::thread::hardware_concurrency(); 
    if (numThreads == 0) { // Fallback se hardware_concurrency não puder determinar
        numThreads = 4; // Valor padrão razoável
    }
    std::cout << "Usando " << numThreads << " threads para recomendacao." << std::endl;

    // Etapas do processo
    preprocessador.gerarInput("dados/ratings.csv", "dados/input.dat");
    preprocessador.gerarExplore("dados/input.dat", "dados/explore.dat", config.N_USUARIOS_EXPLORAR);
    gerenciador.carregarDados("dados/input.dat");
    recomendador.recomendarParaUsuarios("dados/explore.dat", "resultados/output.dat", numThreads);

    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "Tempo total de execucao: " << duracao.count() << " segundos" << std::endl;

    return 0;
}


