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

// Função principal do programa.
int main() {
    // Desabilita a sincronização de C++ streams com C stdio e desvincula cin de cout para otimização de I/O.
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // Registra o tempo de início total da execução do programa.
    auto inicio_total = std::chrono::high_resolution_clock::now();
    
    Configuracao config; // Cria um objeto de configuração com parâmetros padrão.
    GerenciadorDeDados gerenciador; // Cria um objeto para gerenciar o carregamento e acesso aos dados.

    const std::string arquivoCache = "dados/input.bin"; // Define o caminho para o arquivo de cache binário.

    // Tenta carregar os dados de um cache binário.
    if (!gerenciador.carregarDadosDeCacheBinario(arquivoCache)) {
        std::cout << "Cache binario nao encontrado ou invalido." << std::endl;
        
        // Se o cache binário não existir ou for inválido, realiza o pré-processamento.
        auto inicio_pre = std::chrono::high_resolution_clock::now();
        // Gera o arquivo input.bin a partir de ratings.csv.
        Preprocessador::gerarInput("dados/ratings.csv", "dados/input.bin");
        auto fim_pre = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duracao_pre = fim_pre - inicio_pre;
        std::cout << "Tempo de Pre-processamento: " << duracao_pre.count() << " segundos" << std::endl;

        // Carrega os dados do arquivo binário gerado.
        gerenciador.carregarDadosDeCacheBinario("dados/input.bin");
    }

    // Carrega os nomes dos filmes.
    auto inicio_load_nomes = std::chrono::high_resolution_clock::now();
    std::cout << "Carregando nomes dos filmes..." << std::endl;
    gerenciador.carregarNomesFilmes("dados/movies.csv");
    auto fim_load_nomes = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao_load_nomes = fim_load_nomes - inicio_load_nomes;
    std::cout << "Tempo de Carregamento dos Nomes: " << duracao_load_nomes.count() << " segundos" << std::endl;
    
    // Gera o arquivo explore.bin com base nos usuários a serem explorados.
    Preprocessador::gerarExplore(gerenciador, "dados/explore.bin", config.N_USUARIOS_EXPLORAR);

    // Inicia o processo de recomendação.
    auto inicio_rec = std::chrono::high_resolution_clock::now();
    Recomendador recomendador(gerenciador, config); // Cria um objeto Recomendador.
    int numThreads = std::thread::hardware_concurrency(); // Obtém o número de threads disponíveis.
    if (numThreads == 0) numThreads = 4; // Define um valor padrão se não for possível determinar.
    std::cout << "Iniciando recomendacoes com " << numThreads << " threads..." << std::endl;
    // Realiza as recomendações para os usuários e salva no arquivo de saída.
    recomendador.recomendarParaUsuarios("dados/explore.bin", "resultados/output.dat", numThreads);
    auto fim_rec = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao_rec = fim_rec - inicio_rec;
    std::cout << "Tempo de Recomendacao: " << duracao_rec.count() << " segundos" << std::endl;

    // Calcula e exibe o tempo total de execução.
    auto fim_total = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao_total = fim_total - inicio_total;
    std::cout << "\nSistema de recomendacao finalizado." << std::endl;
    std::cout << "Tempo total de execucao: " << duracao_total.count() << " segundos" << std::endl;
    
    return 0;
}


