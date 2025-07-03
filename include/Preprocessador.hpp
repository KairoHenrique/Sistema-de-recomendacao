#ifndef PREPROCESSADOR_HPP
#define PREPROCESSADOR_HPP
#include <string>

class GerenciadorDeDados;
// Contém métodos estáticos para pré-processar os dados de entrada.
class Preprocessador {
public:
    // Gera o arquivo de entrada processado (input.bin) a partir do CSV de avaliações.
    // arquivoCSV: Caminho para o arquivo CSV de avaliações
    // arquivoSaida: Caminho para o arquivo de saída
    static void gerarInput(const std::string& arquivoCSV, const std::string& arquivoSaida);

    // Gera o arquivo de usuários para exploração (explore.bin).
    // gerenciador: Referência ao objeto GerenciadorDeDados, que contém os usuários carregados.
    // exploreDat: Caminho para o arquivo de saída explore.dat.
    static void gerarExplore(GerenciadorDeDados& gerenciador, const std::string& exploreDat, int quantidade);
};

#endif


