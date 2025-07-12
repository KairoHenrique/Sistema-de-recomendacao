#ifndef GERENCIADOR_DE_DADOS_HPP
#define GERENCIADOR_DE_DADOS_HPP

#include <unordered_map>
#include <string>
#include <string_view>
#include "Usuario.hpp"


// Responsável por carregar, armazenar e fornecer acesso aos dados de usuários e filmes.
class GerenciadorDeDados {
private:
    std::unordered_map<int, Usuario> dadosUsuarios; 
    std::string arenaNomesFilmes;
    std::unordered_map<int, std::string_view> nomesFilmes; 
    std::unordered_map<int, float> magnitudes;      
    
public:
    // Lê todo o conteúdo de um arquivo e retorna como string.
    std::string lerArquivoInteiro(const std::string& caminho);

    GerenciadorDeDados();

    void carregarDadosDeCacheBinario(const std::string& caminhoCache);

    void carregarNomesFilmes(const std::string& caminhoMovies);

    const Usuario& getUsuario(int usuarioId) const;

    std::string_view getNomeFilme(int filmeId) const;

    const std::unordered_map<int, Usuario>& getTodosUsuarios() const;

    float getMagnitude(int usuarioId) const;
};

#endif


