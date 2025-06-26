#ifndef RECOMENDADOR_HPP
#define RECOMENDADOR_HPP

#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
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
    void recomendarParaUsuario(int usuarioId, std::ostream& outFile);
};

#endif // RECOMENDADOR_HPP
