#include "GerenciadorDeDados.hpp"
#include <cmath>
#include <fstream>
#include <vector>
#include <string_view>
#include <charconv>
#include <iostream>
#include <utility>

std::string GerenciadorDeDados::lerArquivoInteiro(const std::string& caminho) {
    // Abre o arquivo em modo binário e move o ponteiro para o final.
    std::ifstream arquivo(caminho, std::ios::binary | std::ios::ate);
    if (!arquivo) {
        std::cerr << "Erro fatal: nao foi possivel abrir o arquivo: " << caminho << std::endl;
        exit(1);
    }
    std::streamsize tamanho = arquivo.tellg();
    arquivo.seekg(0, std::ios::beg); // Volta o ponteiro para o início do arquivo.
    std::string buffer(tamanho, '\0');  // Buffer com o tamanho do arquivo
    // Lê o conteúdo do arquivo para o buffer
    if (!arquivo.read(buffer.data(), tamanho)) {
        std::cerr << "Erro fatal: nao foi possivel ler o arquivo: " << caminho << std::endl;
        exit(1);
    }
    return buffer;
}

// Inicializa os vetores e mapas internos com reservas de capacidade para otimização.
GerenciadorDeDados::GerenciadorDeDados() {
    dadosUsuarios.reserve(170000); 
    magnitudes.reserve(170000);
    nomesFilmes.reserve(70000);
}

void GerenciadorDeDados::carregarNomesFilmes(const std::string& caminhoMovies) {
    arenaNomesFilmes = lerArquivoInteiro(caminhoMovies);
    std::string_view sv_filmes(arenaNomesFilmes);

    // Ignora a primeira linha do arquivo.
    size_t pos_filmes = sv_filmes.find('\n');
    if (pos_filmes != std::string_view::npos) {
        sv_filmes.remove_prefix(pos_filmes + 1);
    }

    // Processa cada linha do arquivo para extrair ID e nome do filme.
    while (!sv_filmes.empty()) {
        int filmeId;
        // Encontra o final da linha atual.
        auto fim_linha = sv_filmes.find('\n');
        std::string_view linha = sv_filmes.substr(0, fim_linha);
        // Remove a linha processada; se for a última, remove tudo (evita overflow)
        sv_filmes.remove_prefix(fim_linha != std::string_view::npos ? fim_linha + 1 : sv_filmes.size());

        // Encontra a primeira vírgula para separar o ID do filme.
        auto virgula1 = linha.find(',');
        if (virgula1 == std::string_view::npos) continue; // Pula linhas mal formatadas.
        
        // Converte a string do ID do filme para inteiro.
        std::from_chars(linha.data(), linha.data() + virgula1, filmeId);
        linha.remove_prefix(virgula1 + 1);

        std::string_view nome;
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

void GerenciadorDeDados::carregarDadosDeCacheBinario(const std::string& caminhoCache) {
    std::ifstream in(caminhoCache, std::ios::binary);
    if (!in) {
        std::cout << "  - Nao foi possivel abrir o arquivo binario: " << caminhoCache << std::endl;
    }

    size_t numUsuarios;
    // Lê e reserva espaço para os usuários
    in.read(reinterpret_cast<char*>(&numUsuarios), sizeof(numUsuarios));
    dadosUsuarios.reserve(numUsuarios);

    // Loop para ler os dados de cada usuário.
    for (size_t i = 0; i < numUsuarios; ++i) {
        int userId;
        in.read(reinterpret_cast<char*>(&userId), sizeof(userId));

        Usuario usuario(userId);
        float mag_quadrada = 0.0f;

        size_t numAvaliacoes;
        in.read(reinterpret_cast<char*>(&numAvaliacoes), sizeof(numAvaliacoes));
        
        // Lê todas as avaliações do usuário de uma vez.
        std::vector<std::pair<int, float>> avaliacoes(numAvaliacoes);
        in.read(reinterpret_cast<char*>(avaliacoes.data()), numAvaliacoes * sizeof(std::pair<int, float>));

        for (const auto& [filmeId, nota] : avaliacoes) {
            usuario.adicionarAvaliacao(filmeId, nota);
            mag_quadrada += nota * nota;
        }

        usuario.finalizarEOrdenarAvaliacoes();
        magnitudes[userId] = std::sqrt(mag_quadrada);
        dadosUsuarios.emplace(userId, std::move(usuario));
    }
}

const Usuario& GerenciadorDeDados::getUsuario(int usuarioId) const {
    return dadosUsuarios.at(usuarioId);
}

std::string_view GerenciadorDeDados::getNomeFilme(int filmeId) const {
    auto it = nomesFilmes.find(filmeId);
    if (it != nomesFilmes.end()) {
        return it->second;
    }
    return {}; // Retorna um string_view vazio se o filme não for encontrado.
}

const std::unordered_map<int, Usuario>& GerenciadorDeDados::getTodosUsuarios() const {
    return dadosUsuarios;
}

float GerenciadorDeDados::getMagnitude(int usuarioId) const {
    return magnitudes.at(usuarioId);
}
