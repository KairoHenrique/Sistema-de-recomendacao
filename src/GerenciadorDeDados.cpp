#include "GerenciadorDeDados.hpp"
#include <cmath>
#include <fstream>
#include <vector>
#include <string_view>
#include <charconv>
#include <iostream>
#include <utility>

// Função para ler o conteúdo completo de um arquivo para uma string.
// caminho: Caminho para o arquivo a ser lido.
// Retorna uma string contendo todo o conteúdo do arquivo.
// Em caso de erro na abertura ou leitura, imprime uma mensagem de erro fatal e encerra o programa.
std::string GerenciadorDeDados::lerArquivoInteiro(const std::string& caminho) {
    // Abre o arquivo em modo binário e posiciona o ponteiro no final para obter o tamanho.
    std::ifstream arquivo(caminho, std::ios::binary | std::ios::ate);
    if (!arquivo) {
        std::cerr << "Erro fatal: nao foi possivel abrir o arquivo: " << caminho << std::endl;
        exit(1);
    }
    // Obtém o tamanho do arquivo.
    std::streamsize tamanho = arquivo.tellg();
    // Retorna o ponteiro para o início do arquivo.
    arquivo.seekg(0, std::ios::beg);
    // Cria um buffer (string) com o tamanho do arquivo.
    std::string buffer(tamanho, '\0');
    // Lê o conteúdo do arquivo para o buffer.
    if (!arquivo.read(buffer.data(), tamanho)) {
        std::cerr << "Erro fatal: nao foi possivel ler o arquivo: " << caminho << std::endl;
        exit(1);
    }
    return buffer;
}

// Construtor da classe GerenciadorDeDados.
// Inicializa os vetores e mapas internos com reservas de capacidade para otimização.
GerenciadorDeDados::GerenciadorDeDados() {
    dadosUsuarios.reserve(170000); // Reserva espaço para dados de usuários.
    magnitudes.reserve(170000);   // Reserva espaço para magnitudes de usuários.
    nomesFilmes.reserve(70000);   // Reserva espaço para nomes de filmes.
}

// Carrega os nomes dos filmes a partir de um arquivo CSV.
// caminhoMovies: Caminho para o arquivo CSV contendo IDs e nomes de filmes.
void GerenciadorDeDados::carregarNomesFilmes(const std::string& caminhoMovies) {
    // Lê o conteúdo completo do arquivo de filmes para uma string.
    arenaNomesFilmes = lerArquivoInteiro(caminhoMovies);
    std::string_view sv_filmes(arenaNomesFilmes);

    // Ignora a primeira linha (cabeçalho) do arquivo.
    size_t pos_filmes = sv_filmes.find('\n');
    if (pos_filmes != std::string_view::npos) {
        sv_filmes.remove_prefix(pos_filmes + 1);
    }

    // Processa cada linha do arquivo para extrair ID e nome do filme.
    while (!sv_filmes.empty()) {
        int filmeId; // ID do filme.
        // Encontra o final da linha atual.
        auto fim_linha = sv_filmes.find('\n');
        std::string_view linha = sv_filmes.substr(0, fim_linha);
        // Remove a linha processada da string_view principal.
        sv_filmes.remove_prefix(fim_linha != std::string_view::npos ? fim_linha + 1 : sv_filmes.size());

        // Encontra a primeira vírgula para separar o ID do filme.
        auto virgula1 = linha.find(',');
        if (virgula1 == std::string_view::npos) continue; // Pula linhas mal formatadas.
        
        // Converte a string do ID do filme para inteiro.
        std::from_chars(linha.data(), linha.data() + virgula1, filmeId);
        linha.remove_prefix(virgula1 + 1);

        std::string_view nome; // Nome do filme.
        // Verifica se o nome do filme está entre aspas (para nomes com vírgulas).
        if (!linha.empty() && linha.front() == '"') {
            linha.remove_prefix(1);
            auto aspas_finais = linha.find_last_of('"');
            if (aspas_finais == std::string_view::npos) continue; // Pula linhas mal formatadas.
            nome = linha.substr(0, aspas_finais);
        } else {
            // Se não houver aspas, o nome vai até a próxima vírgula.
            auto virgula2 = linha.find(',');
            nome = linha.substr(0, virgula2);
        }
        // Armazena o nome do filme associado ao seu ID.
        nomesFilmes[filmeId] = nome;
    }
}

// Carrega dados de usuários e avaliações a partir de um arquivo de cache binário.
// caminhoCache: Caminho para o arquivo de cache binário.
// Retorna true se o cache foi carregado com sucesso, false caso contrário.
void GerenciadorDeDados::carregarDadosDeCacheBinario(const std::string& caminhoCache) {
    std::ifstream in(caminhoCache, std::ios::binary); // Abre o arquivo em modo binário.
    if (!in) {
        std::cout << "  - Nao foi possivel abrir o arquivo de cache binario: " << caminhoCache << std::endl;
    }

    std::cout << "  - Lendo cache binario de " << caminhoCache << "..." << std::endl;

    size_t numUsuarios; // Número total de usuários no cache.
    // Lê o número de usuários do arquivo.
    in.read(reinterpret_cast<char*>(&numUsuarios), sizeof(numUsuarios));
    dadosUsuarios.reserve(numUsuarios); // Reserva espaço para os dados dos usuários.

    // Loop para ler os dados de cada usuário.
    for (size_t i = 0; i < numUsuarios; ++i) {
        int userId; // ID do usuário.
        // Lê o ID do usuário.
        in.read(reinterpret_cast<char*>(&userId), sizeof(userId));

        Usuario usuario(userId); // Cria um objeto Usuario.
        float mag_quadrada = 0.0f; // Variável para calcular a magnitude ao quadrado.

        size_t numAvaliacoes; // Número de avaliações para o usuário atual.
        // Lê o número de avaliações.
        in.read(reinterpret_cast<char*>(&numAvaliacoes), sizeof(numAvaliacoes));
        
        // Lê todas as avaliações do usuário de uma vez.
        std::vector<std::pair<int, float>> avaliacoes(numAvaliacoes);
        in.read(reinterpret_cast<char*>(avaliacoes.data()), numAvaliacoes * sizeof(std::pair<int, float>));

        // Adiciona as avaliações ao objeto Usuario e calcula a magnitude.
        for (const auto& [filmeId, nota] : avaliacoes) {
            usuario.adicionarAvaliacao(filmeId, nota);
            mag_quadrada += nota * nota;
        }

        usuario.finalizarEOrdenarAvaliacoes(); // Finaliza e ordena as avaliações do usuário.
        magnitudes[userId] = std::sqrt(mag_quadrada); // Calcula e armazena a magnitude.
        dadosUsuarios.emplace(userId, std::move(usuario)); // Adiciona o usuário ao mapa de dados.
    }
    std::cout << "  - Cache binario carregado com sucesso." << std::endl;
}

// Retorna uma referência constante ao objeto Usuario com o ID especificado.
// usuarioId: ID do usuário a ser recuperado.
// Lança uma exceção se o usuário não for encontrado.
const Usuario& GerenciadorDeDados::getUsuario(int usuarioId) const {
    return dadosUsuarios.at(usuarioId);
}

// Retorna o nome de um filme dado seu ID.
// filmeId: ID do filme.
// Retorna um string_view vazio se o filme não for encontrado.
std::string_view GerenciadorDeDados::getNomeFilme(int filmeId) const {
    auto it = nomesFilmes.find(filmeId);
    if (it != nomesFilmes.end()) {
        return it->second;
    }
    return {}; // Retorna um string_view vazio se o filme não for encontrado.
}

// Retorna uma referência constante ao mapa de todos os usuários.
const std::unordered_map<int, Usuario>& GerenciadorDeDados::getTodosUsuarios() const {
    return dadosUsuarios;
}

// Retorna a magnitude de um usuário dado seu ID.
// usuarioId: ID do usuário.
// Lança uma exceção se o usuário não for encontrado.
float GerenciadorDeDados::getMagnitude(int usuarioId) const {
    return magnitudes.at(usuarioId);
}


