#include <iostream>
#include "preprocessamento.hpp"
#include "recomendador.hpp"

int main() {
    std::cout << "Sistema de Recomendacao Iniciado!" << std::endl;

    carregarDados("dados/input.dat");
    recomendarParaUsuarios("dados/explore.dat", "resultados/output.dat");

    std::cout << "Recomendacoes concluidas!" << std::endl;
    return 0;
}
