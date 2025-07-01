#ifndef GERENCIADOR_DE_DADOS_HPP
#define GERENCIADOR_DE_DADOS_HPP

#include <unordered_map>
#include <string>
#include <string_view>
#include "Usuario.hpp"

class GerenciadorDeDados {
private:
    std::unordered_map<int, Usuario> dadosUsuarios;
    std::string arenaNomesFilmes;
    std::unordered_map<int, std::string_view> nomesFilmes;
    std::unordered_map<int, float> magnitudes;
    
public:
    GerenciadorDeDados();

    // --- NOVAS FUNÇÕES DE CARREGAMENTO ---
    // Carrega dados a partir do arquivo de texto original (.dat)
    void carregarDadosDeTexto(const std::string& caminhoInput);
    
    // Carrega dados de forma extremamente rápida a partir do cache binário (.bin)
    bool carregarDadosDeCacheBinario(const std::string& caminhoCache);

    // Carrega os nomes dos filmes (necessário em ambos os casos)
    void carregarNomesFilmes(const std::string& caminhoMovies);

    const Usuario& getUsuario(int usuarioId) const;
    std::string_view getNomeFilme(int filmeId) const;
    const std::unordered_map<int, Usuario>& getTodosUsuarios() const;
    float getMagnitude(int usuarioId) const;
};

#endif // GERENCIADOR_DE_DADOS_HPP
