#include <chrono>
#include <iostream>
#include <thread>
#include <sys/resource.h>     // Para medir uso de memória
#include <functional>         // NECESSÁRIO para std::function
#include "Preprocessador.hpp"
#include "Recomendador.hpp"
#include "Configuracao.hpp"
#include "GerenciadorDeDados.hpp"
#include "utilitarios.hpp"


double memoriaMB() {
    struct rusage uso;
    getrusage(RUSAGE_SELF, &uso);
    return uso.ru_maxrss / 1024.0; // Em MB (Linux: ru_maxrss é em KB)
}

void medirAcao(const std::string& nome, const std::function<void()>& acao) {
    std::cout << "\nIniciando: " << nome << std::endl;
    auto inicio = std::chrono::high_resolution_clock::now();
    double memoriaAntes = memoriaMB();

    acao(); // Executa a ação

    auto fim = std::chrono::high_resolution_clock::now();
    double memoriaDepois = memoriaMB();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "Finalizado: " << nome << std::endl;
    std::cout << "Tempo: " << duracao.count() << " segundos" << std::endl;
    std::cout << "Memória: " << (memoriaDepois - memoriaAntes) << " MB usada\n";
}

int main() {
    std::cout << "Iniciando sistema de recomendacao..." << std::endl;
    auto inicioGeral = std::chrono::high_resolution_clock::now();

    Configuracao config;
    GerenciadorDeDados gerenciador;
    Preprocessador preprocessador;
    Recomendador recomendador(gerenciador, config);

    int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;
    std::cout << "Usando " << numThreads << " threads para recomendacao." << std::endl;

    medirAcao("Gerar input.dat", [&]() {
        preprocessador.gerarInput("dados/ratings.csv", "dados/input.dat");
    });

    medirAcao("Gerar explore.dat", [&]() {
        preprocessador.gerarExplore("dados/input.dat", "dados/explore.dat", config.N_USUARIOS_EXPLORAR);
    });

    medirAcao("Carregar input.dat", [&]() {
        gerenciador.carregarDados("dados/input.dat");
    });

    medirAcao("Carregar movies.csv", [&]() {
        gerenciador.carregarNomesFilmes("dados/movies.csv");
    });

    medirAcao("Recomendar para usuarios", [&]() {
        recomendador.recomendarParaUsuarios("dados/explore.dat", "resultados/output.dat", numThreads);
    });

    auto fimGeral = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracaoGeral = fimGeral - inicioGeral;
    std::cout << "\nFinalizando sistema de recomendacao." << std::endl;
    std::cout << "Tempo total de execucao: " << duracaoGeral.count() << " segundos" << std::endl;

    return 0;
}
