#ifndef RECOMENDADOR_HPP
#define RECOMENDADOR_HPP

#include <string>
#include "GerenciadorDeDados.hpp"
#include "Configuracao.hpp"

class Recomendador {
private:
    GerenciadorDeDados& gerenciador;
    const Configuracao& config;

public:
    Recomendador(GerenciadorDeDados& gerenciador, const Configuracao& config);
    void recomendarParaUsuarios(const std::string& arquivoExploracao, const std::string& arquivoSaida);
};

#endif // RECOMENDADOR_HPP

