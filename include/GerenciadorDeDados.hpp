#ifndef GERENCIADOR_DE_DADOS_HPP
#define GERENCIADOR_DE_DADOS_HPP

#include <unordered_map>
#include <string>
#include <string_view> // Essencial para a otimização de strings
#include "Usuario.hpp"

class GerenciadorDeDados {
private:
    std::unordered_map<int, Usuario> dadosUsuarios;
    
    // --- OTIMIZAÇÃO DE MEMÓRIA: String Arena ---
    std::string arenaNomesFilmes;
    std::unordered_map<int, std::string_view> nomesFilmes;

    std::unordered_map<int, float> magnitudes;
    
public:
    GerenciadorDeDados();
    void carregarDados(const std::string& caminhoInput, const std::string& caminhoMovies);
    const Usuario& getUsuario(int usuarioId) const;
    std::string_view getNomeFilme(int filmeId) const;
    const std::unordered_map<int, Usuario>& getTodosUsuarios() const;
    float getMagnitude(int usuarioId) const;
};

#endif