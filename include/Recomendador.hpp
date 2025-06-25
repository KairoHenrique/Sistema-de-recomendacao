#ifndef RECOMENDADOR_HPP
#define RECOMENDADOR_HPP

#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <mutex>
#include <thread>
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
    void recomendarParaUsuario(int usuarioId, std::ostream& outFile); // CORREÇÃO CRÍTICA
};

#endif // RECOMENDADOR_HPP
