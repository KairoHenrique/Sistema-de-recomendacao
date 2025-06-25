#ifndef RECOMENDADOR_HPP
#define RECOMENDADOR_HPP

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include "GerenciadorDeDados.hpp"
#include "Configuracao.hpp"

class Recomendador {
private:
    GerenciadorDeDados& gerenciador;
    const Configuracao& config;
    std::mutex mtx;

public:
    Recomendador(GerenciadorDeDados& gerenciador, const Configuracao& config);
    void recomendarParaUsuarios(const std::string& arquivoExploracao, const std::string& arquivoSaida, int numThreads);
    void recomendarParaUsuario(int usuarioId, std::ofstream& outFile);
};

#endif // RECOMENDADOR_HPP


