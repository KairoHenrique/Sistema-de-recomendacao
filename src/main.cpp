#include <iostream>
#include <chrono> // Para medir tempo
#include "preprocessamento.hpp"
#include "recomendador.hpp"
#include "config.hpp"

int main() {
    // Início da contagem de tempo
    auto inicio = std::chrono::high_resolution_clock::now();

    // Executa todas as etapas automaticamente – Kairo
    gerarInput("dados/ratings.csv", "dados/input.dat");
    gerarExplore("dados/input.dat", "dados/explore.dat", N_USUARIOS_EXPLORAR);
    carregarDados("dados/input.dat");
    recomendarParaUsuarios("dados/explore.dat", "resultados/output.dat");

    // Fim da contagem de tempo
    auto fim = std::chrono::high_resolution_clock::now();

    // Calcula duração
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "Tempo total de execucao: " << duracao.count() << " segundos" << std::endl;

    return 0;
}
