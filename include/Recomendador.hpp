#ifndef RECOMENDADOR_HPP
#define RECOMENDADOR_HPP
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include "GerenciadorDeDados.hpp"
#include "Configuracao.hpp"

// Responsável por gerar recomendações de filmes para usuários.
class Recomendador {
private:
    GerenciadorDeDados& gerenciador; // Referência ao gerenciador de dados para acesso aos usuários e filmes.
    const Configuracao& config;      // Referência às configurações do sistema.
    std::mutex mtx;                 // Mutex para proteger a escrita no arquivo de saída.
    
public:
    Recomendador(GerenciadorDeDados& gerenciador, const Configuracao& config);

    // Realiza o processo de recomendação para múltiplos usuários em paralelo.
    // arquivoExploracao: Caminho para o arquivo contendo os IDs dos usuários a serem explorados.
    // arquivoSaida: Caminho para o arquivo onde as recomendações serão salvas.
    // numThreads: Número de threads a serem utilizadas para o processamento paralelo.
    void recomendarParaUsuarios(const std::string& arquivoExploracao, const std::string& arquivoSaida, int numThreads);

    // Realiza a recomendação de filmes para um único usuário.
    // usuarioId: O ID do usuário para o qual as recomendações serão geradas.
    // outFile: Um stream de saída onde as recomendações serão escritas.
    void recomendarParaUsuario(int usuarioId, std::ostream& outFile);
};

#endif


