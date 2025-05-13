#include <iostream>
#include "preprocessamento.hpp"
#include "recomendador.hpp"
#include "config.hpp"

int main() {
    // Executa todas as etapas automaticamente – Kairo
    gerarInput("dados/ratings.csv", "dados/input.dat");
    gerarExplore("dados/input.dat", "dados/explore.dat", N_USUARIOS_EXPLORAR);
    carregarDados("dados/input.dat");
    recomendarParaUsuarios("dados/explore.dat", "resultados/output.dat");
    
    return 0;
}
