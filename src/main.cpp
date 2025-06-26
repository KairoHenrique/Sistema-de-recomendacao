#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include "Preprocessador.hpp"
#include "Recomendador.hpp"
#include "Configuracao.hpp"
#include "GerenciadorDeDados.hpp"
#include "utilitarios.hpp"
#include <sstream>

// Função para pegar memória atual usada pelo processo em MB
double memoriaMB() {
    std::ifstream arquivo("/proc/self/status");
    std::string linha;
    while (std::getline(arquivo, linha)) {
        if (linha.find("VmRSS:") == 0) {
            std::istringstream iss(linha);
            std::string key;
            double memKB;
            std::string unidade;
            iss >> key >> memKB >> unidade;
            return memKB / 1024.0;  // Convertendo para MB
        }
    }
    return 0.0;
}

int main() {
    std::cout << "Iniciando sistema de recomendacao..." << std::endl;
    auto total_inicio = std::chrono::high_resolution_clock::now();

    Configuracao config;
    GerenciadorDeDados gerenciador;
    Preprocessador preprocessador;
    Recomendador recomendador(gerenciador, config);

    int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;
    std::cout << "Usando " << numThreads << " threads para recomendacao." << std::endl;

    // --- Gerar input.dat ---
    auto ini_input = std::chrono::high_resolution_clock::now();
    preprocessador.gerarInput("dados/ratings.csv", "dados/input.dat");
    auto fim_input = std::chrono::high_resolution_clock::now();
    double tempo_input = std::chrono::duration<double>(fim_input - ini_input).count();
    double mem_input = memoriaMB();
    std::cout << "Tempo para gerar input: " << tempo_input << "s | Memória: " << mem_input << " MB" << std::endl;

    // --- Gerar explore.dat ---
    auto ini_explore = std::chrono::high_resolution_clock::now();
    preprocessador.gerarExplore("dados/input.dat", "dados/explore.dat", config.N_USUARIOS_EXPLORAR);
    auto fim_explore = std::chrono::high_resolution_clock::now();
    double tempo_explore = std::chrono::duration<double>(fim_explore - ini_explore).count();
    double mem_explore = memoriaMB();
    std::cout << "Tempo para gerar explore: " << tempo_explore << "s | Memória: " << mem_explore << " MB" << std::endl;

    // --- Carregar dados ---
    auto ini_carregar = std::chrono::high_resolution_clock::now();
    gerenciador.carregarDados("dados/input.dat");
    gerenciador.carregarNomesFilmes("dados/movies.csv");
    auto fim_carregar = std::chrono::high_resolution_clock::now();
    double tempo_carregar = std::chrono::duration<double>(fim_carregar - ini_carregar).count();
    double mem_carregar = memoriaMB();
    std::cout << "Tempo para carregar dados: " << tempo_carregar << "s | Memória: " << mem_carregar << " MB" << std::endl;

    // --- Recomendação ---
    auto ini_recomendar = std::chrono::high_resolution_clock::now();
    recomendador.recomendarParaUsuarios("dados/explore.dat", "resultados/output.dat", numThreads);
    auto fim_recomendar = std::chrono::high_resolution_clock::now();
    double tempo_recomendar = std::chrono::duration<double>(fim_recomendar - ini_recomendar).count();
    double mem_recomendar = memoriaMB();
    std::cout << "Tempo para recomendar: " << tempo_recomendar << "s | Memória: " << mem_recomendar << " MB" << std::endl;

    // --- Tempo total ---
    auto total_fim = std::chrono::high_resolution_clock::now();
    double tempo_total = std::chrono::duration<double>(total_fim - total_inicio).count();
    std::cout << "Finalizando sistema de recomendacao." << std::endl;
    std::cout << "Tempo total de execucao: " << tempo_total << " segundos" << std::endl;
    std::cout << "Uso total de memoria aproximado: " << mem_recomendar << " MB" << std::endl;

    return 0;
}
