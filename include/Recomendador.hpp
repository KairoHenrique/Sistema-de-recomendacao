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

// Responsável por gerar recomendações de filmes.
class Recomendador {
private:
    GerenciadorDeDados& gerenciador;
    const Configuracao& config;
    std::mutex mtx;

public:
    Recomendador(GerenciadorDeDados& gerenciador, const Configuracao& config);

    void recomendarParaUsuarios(const std::string& arquivoExploracao, const std::string& arquivoSaida, int numThreads);
    void recomendarParaUsuario(int usuarioId, std::ostream& outFile);
};

#endif
