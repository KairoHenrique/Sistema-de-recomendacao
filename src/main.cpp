#include <iostream>
#include <chrono>
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

    // Garante que o diretório 'dados' e 'resultados' existam
    // Isso pode ser feito com comandos de sistema ou verificações mais robustas
    // Por simplicidade, assumimos que eles existem ou serão criados manualmente
    // ou adicionamos a criação aqui.
    // Exemplo de criação de diretório (Linux/macOS):
    // system("mkdir -p dados");
    // system("mkdir -p resultados");

    // Etapas do processo
    preprocessador.gerarInput("dados/ratings.csv", "dados/input.dat");
    preprocessador.gerarExplore("dados/input.dat", "dados/explore.dat", config.N_USUARIOS_EXPLORAR);
    gerenciador.carregarDados("dados/input.dat");
    recomendador.recomendarParaUsuarios("dados/explore.dat", "resultados/output.dat");

    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "Tempo total de execucao: " << duracao.count() << " segundos" << std::endl;

    return 0;
}


