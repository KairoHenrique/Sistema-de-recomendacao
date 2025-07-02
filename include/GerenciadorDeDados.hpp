#ifndef GERENCIADOR_DE_DADOS_HPP
#define GERENCIADOR_DE_DADOS_HPP

#include <unordered_map>
#include <string>
#include <string_view>
#include "Usuario.hpp"

// Classe GerenciadorDeDados
// Responsável por carregar, armazenar e fornecer acesso aos dados de usuários e filmes.
class GerenciadorDeDados {
private:
    std::unordered_map<int, Usuario> dadosUsuarios; // Mapa de IDs de usuário para objetos Usuario.
    std::string arenaNomesFilmes;                   // Armazena o conteúdo do arquivo de nomes de filmes para string_view.
    std::unordered_map<int, std::string_view> nomesFilmes; // Mapa de IDs de filme para seus nomes (string_view).
    std::unordered_map<int, float> magnitudes;      // Mapa de IDs de usuário para as magnitudes de seus vetores de avaliação.
    
public:
    // Construtor padrão.
    GerenciadorDeDados();

    
    // Carrega dados de usuários e avaliações a partir de um arquivo de cache binário.
    // caminhoCache: Caminho para o arquivo de cache binário (e.g., input.bin).
    // Retorna true se o cache foi carregado com sucesso, false caso contrário.
    bool carregarDadosDeCacheBinario(const std::string& caminhoCache);

    // Carrega os nomes dos filmes a partir de um arquivo CSV.
    // caminhoMovies: Caminho para o arquivo CSV de filmes (e.g., movies.csv).
    void carregarNomesFilmes(const std::string& caminhoMovies);

    // Retorna uma referência constante a um objeto Usuario dado seu ID.
    // usuarioId: O ID do usuário a ser recuperado.
    const Usuario& getUsuario(int usuarioId) const;

    // Retorna o nome de um filme dado seu ID.
    // filmeId: O ID do filme a ser recuperado.
    std::string_view getNomeFilme(int filmeId) const;

    // Retorna uma referência constante ao mapa de todos os usuários.
    const std::unordered_map<int, Usuario>& getTodosUsuarios() const;

    // Retorna a magnitude do vetor de avaliações de um usuário dado seu ID.
    // usuarioId: O ID do usuário.
    float getMagnitude(int usuarioId) const;
};

#endif // GERENCIADOR_DE_DADOS_HPP


