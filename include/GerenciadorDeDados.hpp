#ifndef GERENCIADOR_DE_DADOS_HPP
#define GERENCIADOR_DE_DADOS_HPP

#include <string>
#include <unordered_map>
#include "Usuario.hpp"
#include "Filme.hpp"

class GerenciadorDeDados {
private:
    std::unordered_map<int, Usuario> dadosUsuarios;
    std::unordered_map<int, std::string> nomesFilmes;

public:
    GerenciadorDeDados();
    void carregarDados(const std::string& caminhoArquivo);
    void carregarNomesFilmes(const std::string& caminhoCSV);
    const Usuario& getUsuario(int usuarioId) const;
    const std::string& getNomeFilme(int filmeId) const;
    const std::unordered_map<int, Usuario>& getTodosUsuarios() const;
};

#endif // GERENCIADOR_DE_DADOS_HPP

