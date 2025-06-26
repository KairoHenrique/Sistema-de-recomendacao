#ifndef GERENCIADOR_DE_DADOS_HPP
#define GERENCIADOR_DE_DADOS_HPP

#include <unordered_map>
#include <string>
#include "Usuario.hpp"

class GerenciadorDeDados {
private:
    std::unordered_map<int, Usuario> dadosUsuarios;
    std::unordered_map<int, std::string> nomesFilmes;
    std::unordered_map<int, float> magnitudes;
    
public:
    GerenciadorDeDados();
    void carregarDados(const std::string& caminhoArquivo);
    void carregarNomesFilmes(const std::string& caminhoCSV);
    const Usuario& getUsuario(int usuarioId) const;
    const std::string& getNomeFilme(int filmeId) const;
    const std::unordered_map<int, Usuario>& getTodosUsuarios() const;
    float getMagnitude(int usuarioId) const;
    
    // Adicione este método
    const std::unordered_map<int, std::string>& getNomesFilmesMap() const {
        return nomesFilmes;
    }
};

#endif
