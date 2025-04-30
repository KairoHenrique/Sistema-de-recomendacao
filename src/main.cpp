#include <iostream>
#include "preprocessamento.hpp"
#include "recomendador.hpp"

int main() {
    int opcao;

    do {
        // Menu de interacao com o sistema – Kairo
        std::cout << "\n===== MENU DO SISTEMA DE RECOMENDACAO =====\n";
        std::cout << "1 - Gerar input.dat (a partir de ratings.csv)\n";
        std::cout << "2 - Gerar explore.dat (usuarios aleatorios)\n";
        std::cout << "3 - Executar recomendacoes (gerar output.dat)\n";
        std::cout << "0 - Sair\n> ";
        std::cin >> opcao;

        switch (opcao) {
            case 1:
                // Gera input.dat aplicando os filtros obrigatorios – Kairo
                gerarInput("dados/ratings.csv", "dados/input.dat");
                break;
            case 2: {
                // Solicita a quantidade e gera explore.dat com usuarios aleatorios – Kairo
                int quantidade;
                std::cout << "Quantos usuarios deseja sortear? ";
                std::cin >> quantidade;
                gerarExplore("dados/input.dat", "dados/explore.dat", quantidade);
                break;
            }
            case 3:
                // Executa o sistema de recomendacao com base no input e explore – Kairo
                carregarDados("dados/input.dat");
                recomendarParaUsuarios("dados/explore.dat", "resultados/output.dat");
                break;
            case 0:
                std::cout << "Saindo\n";
                break;
            default:
                std::cout << "Opcao invalida!\n";
        }
    } while (opcao != 0);

    return 0;
}
