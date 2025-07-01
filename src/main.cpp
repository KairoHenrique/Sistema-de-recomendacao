#include <chrono>
#include <iostream>
#include <thread>
#include <string>
#include <filesystem>
#include <system_error>
#include <sys/resource.h> // Para medir memória

#include "Preprocessador.hpp"
#include "Recomendador.hpp"
#include "Configuracao.hpp"
#include "GerenciadorDeDados.hpp"
#include "utilitarios.hpp" // Para arquivoExiste

void criarDiretorios() {
    std::error_code ec;
    std::filesystem::create_directory("dados", ec);
    if (ec && ec.value() != static_cast<int>(std::errc::file_exists)) {
        std::cerr << "Erro ao criar diretorio 'dados': " << ec.message() << std::endl;
    }
    
    ec.clear();
    
    std::filesystem::create_directory("resultados", ec);
    if (ec && ec.value() != static_cast<int>(std::errc::file_exists)) {
        std::cerr << "Erro ao criar diretorio 'resultados': " << ec.message() << std::endl;
    }
}

double obterUsoMaximoMemoriaMB() {
    struct rusage uso;
    getrusage(RUSAGE_SELF, &uso);
    return uso.ru_maxrss / 1024.0; // ru_maxrss vem em KB no Linux
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    criarDiretorios();

    auto inicio_total = std::chrono::high_resolution_clock::now();
    
    Configuracao config;
    GerenciadorDeDados gerenciador;

    const std::string arquivoCache = "dados/input.bin";

    if (!gerenciador.carregarDadosDeCacheBinario(arquivoCache)) {
        std::cout << "Cache binario nao encontrado ou invalido." << std::endl;
        
        auto inicio_pre = std::chrono::high_resolution_clock::now();
        Preprocessador::gerarInput("dados/ratings.csv", "dados/input.dat");
        auto fim_pre = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duracao_pre = fim_pre - inicio_pre;
        std::cout << "Tempo de Pre-processamento: " << duracao_pre.count() << " segundos" << std::endl;

        gerenciador.carregarDadosDeTexto("dados/input.dat");
    }

    auto inicio_load_nomes = std::chrono::high_resolution_clock::now();
    std::cout << "Carregando nomes dos filmes..." << std::endl;
    gerenciador.carregarNomesFilmes("dados/movies.csv");
    auto fim_load_nomes = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao_load_nomes = fim_load_nomes - inicio_load_nomes;
    std::cout << "Tempo de Carregamento dos Nomes: " << duracao_load_nomes.count() << " segundos" << std::endl;
    
    // CORREÇÃO: passando gerenciador por referência
    Preprocessador::gerarExplore(gerenciador, "dados/explore.dat", config.N_USUARIOS_EXPLORAR);

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

    double memoriaMB = obterUsoMaximoMemoriaMB();
    std::cout << "Memoria maxima utilizada: " << memoriaMB << " MB" << std::endl;
    
    return 0;
}
