#include <chrono>
#include <iostream>
#include <thread>
#include <string>
#include <filesystem>
#include <system_error> // Necessário para std::error_code

#include "Preprocessador.hpp"
#include "Recomendador.hpp"
#include "Configuracao.hpp"
#include "GerenciadorDeDados.hpp"

void criarDiretorios() {
    std::error_code ec;
    // Usa a versão de create_directory que não lança exceções.
    // O resultado da operação é colocado em 'ec'.
    std::filesystem::create_directory("dados", ec);
    if (ec && ec.value() != static_cast<int>(std::errc::file_exists)) {
        std::cerr << "Erro ao criar diretorio 'dados': " << ec.message() << std::endl;
    }
    
    ec.clear(); // Limpa o código de erro para a próxima chamada
    
    std::filesystem::create_directory("resultados", ec);
    if (ec && ec.value() != static_cast<int>(std::errc::file_exists)) {
        std::cerr << "Erro ao criar diretorio 'resultados': " << ec.message() << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    criarDiretorios();

    auto inicio_total = std::chrono::high_resolution_clock::now();
    
    Configuracao config;
    GerenciadorDeDados gerenciador;

    auto inicio_pre = std::chrono::high_resolution_clock::now();
    Preprocessador::gerarInput("dados/ratings.csv", "dados/input.dat");
    Preprocessador::gerarExplore("dados/input.dat", "dados/explore.dat", config.N_USUARIOS_EXPLORAR);
    auto fim_pre = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao_pre = fim_pre - inicio_pre;
    std::cout << "Tempo de Pre-processamento: " << duracao_pre.count() << " segundos" << std::endl;

    auto inicio_load = std::chrono::high_resolution_clock::now();
    std::cout << "Carregando dados para a memoria..." << std::endl;
    gerenciador.carregarDados("dados/input.dat", "dados/movies.csv");
    auto fim_load = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao_load = fim_load - inicio_load;
    std::cout << "Tempo de Carregamento: " << duracao_load.count() << " segundos" << std::endl;

    auto inicio_rec = std::chrono::high_resolution_clock::now();
    Recomendador recomendador(gerenciador, config);
    int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;
    std::cout << "Iniciando recomendacoes com " << numThreads << " threads..." << std::endl;
    recomendador.recomendarParaUsuarios("dados/explore.dat", "resultados/output.dat", numThreads);
    auto fim_rec = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao_rec = fim_rec - inicio_rec;
    std::cout << "Tempo de Recomendacao: " << duracao_rec.count() << " segundos" << std::endl;

    auto fim_total = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao_total = fim_total - inicio_total;
    std::cout << "\nSistema de recomendacao finalizado." << std::endl;
    std::cout << "Tempo total de execucao: " << duracao_total.count() << " segundos" << std::endl;
    
    return 0;
}
