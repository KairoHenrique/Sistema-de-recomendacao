#ifndef PREPROCESSADOR_HPP
#define PREPROCESSADOR_HPP

#include <string>

// Forward declaration para evitar inclusão circular e dependências desnecessárias.
class GerenciadorDeDados;

// Classe Preprocessador
// Contém métodos estáticos para pré-processar os dados de entrada.
class Preprocessador {
public:
    // Gera o arquivo de entrada processado (input.dat) a partir do CSV de avaliações.
    // arquivoCSV: Caminho para o arquivo CSV de avaliações (e.g., ratings.csv).
    // arquivoSaida: Caminho para o arquivo de saída (e.g., input.dat).
    static void gerarInput(const std::string& arquivoCSV, const std::string& arquivoSaida);

    // Gera o arquivo de usuários para exploração (explore.dat).
    // gerenciador: Referência ao objeto GerenciadorDeDados, que contém os usuários carregados.
    // exploreDat: Caminho para o arquivo de saída explore.dat.
    // quantidade: Número de usuários a serem incluídos no arquivo explore.dat.
    static void gerarExplore(GerenciadorDeDados& gerenciador, const std::string& exploreDat, int quantidade);
};

#endif


