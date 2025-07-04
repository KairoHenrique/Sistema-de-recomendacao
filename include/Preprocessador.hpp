#ifndef PREPROCESSADOR_HPP
#define PREPROCESSADOR_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <string_view>

// Forward declaration para evitar inclusão circular e dependências desnecessárias.
class GerenciadorDeDados;

// Estrutura para armazenar contagens parciais de filmes e usuários,
// e avaliações brutas durante o processamento paralelo.
struct ContagemParcial {
    std::unordered_map<int, int> filmesCounts;   // Contagem de avaliações por filme.
    std::unordered_map<int, int> usuariosCounts; // Contagem de avaliações por usuário.
    std::vector<std::tuple<int, int, float>> avaliacoesBrutas; // Avaliações lidas do CSV.
};

// Classe Preprocessador
// Contém métodos estáticos para pré-processar os dados de entrada.
class Preprocessador {
private:
    // Processa um pedaço (chunk) do arquivo CSV de avaliações.
    static ContagemParcial processarChunk(std::string_view chunk);

    // Escreve os dados processados em um arquivo binário de cache.
    static void escreverCacheBinario(const std::string& arquivoCache, const std::unordered_map<int, std::vector<std::pair<int, float>>>& dados);

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


