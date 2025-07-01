#include "GerenciadorDeDados.hpp"
#include <cmath>
#include <fstream>
#include <vector>
#include <string_view>
#include <charconv>
#include <iostream>
#include <utility>

// Função externa para ler um arquivo inteiro para a memória.
// A definição real está em outro arquivo, mas precisamos da declaração.
extern std::string lerArquivoInteiro(const std::string& caminho);

GerenciadorDeDados::GerenciadorDeDados() {
    dadosUsuarios.reserve(170000);
    magnitudes.reserve(170000);
    nomesFilmes.reserve(70000);
}

void GerenciadorDeDados::carregarNomesFilmes(const std::string& caminhoMovies) {
    arenaNomesFilmes = lerArquivoInteiro(caminhoMovies);
    std::string_view sv_filmes(arenaNomesFilmes);

    size_t pos_filmes = sv_filmes.find('\n');
    if (pos_filmes != std::string_view::npos) {
        sv_filmes.remove_prefix(pos_filmes + 1);
    }

    while (!sv_filmes.empty()) {
        int filmeId;
        auto fim_linha = sv_filmes.find('\n');
        std::string_view linha = sv_filmes.substr(0, fim_linha);
        sv_filmes.remove_prefix(fim_linha != std::string_view::npos ? fim_linha + 1 : sv_filmes.size());

        auto virgula1 = linha.find(',');
        if (virgula1 == std::string_view::npos) continue;
        
        std::from_chars(linha.data(), linha.data() + virgula1, filmeId);
        linha.remove_prefix(virgula1 + 1);

        std::string_view nome;
        if (!linha.empty() && linha.front() == '"') {
            linha.remove_prefix(1);
            auto aspas_finais = linha.find_last_of('"');
            if (aspas_finais == std::string_view::npos) continue;
            nome = linha.substr(0, aspas_finais);
        } else {
            auto virgula2 = linha.find(',');
            nome = linha.substr(0, virgula2);
        }
        nomesFilmes[filmeId] = nome;
    }
}

bool GerenciadorDeDados::carregarDadosDeCacheBinario(const std::string& caminhoCache) {
    std::ifstream in(caminhoCache, std::ios::binary);
    if (!in) {
        return false; // Cache não encontrado ou não pôde ser aberto
    }

    std::cout << "  - Lendo cache binario de " << caminhoCache << "..." << std::endl;

    size_t numUsuarios;
    in.read(reinterpret_cast<char*>(&numUsuarios), sizeof(numUsuarios));
    dadosUsuarios.reserve(numUsuarios);

    for (size_t i = 0; i < numUsuarios; ++i) {
        int userId;
        in.read(reinterpret_cast<char*>(&userId), sizeof(userId));

        Usuario usuario(userId);
        float mag_quadrada = 0.0f;

        size_t numAvaliacoes;
        in.read(reinterpret_cast<char*>(&numAvaliacoes), sizeof(numAvaliacoes));
        
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
    
    return true;
}

void GerenciadorDeDados::carregarDadosDeTexto(const std::string& caminhoInput) {
    std::cout << "  - Lendo dados do arquivo de texto " << caminhoInput << "..." << std::endl;
    std::string conteudoInput = lerArquivoInteiro(caminhoInput);
    std::string_view sv_input(conteudoInput);

    while (!sv_input.empty()) {
        size_t fim_linha = sv_input.find('\n');
        std::string_view linha = sv_input.substr(0, fim_linha);
        sv_input.remove_prefix(fim_linha != std::string_view::npos ? fim_linha + 1 : sv_input.size());
        if (linha.empty()) continue;

        int usuarioId; // Variável declarada como 'usuarioId'
        auto pos_espaco = linha.find(' ');
        std::from_chars(linha.data(), linha.data() + pos_espaco, usuarioId);
        linha.remove_prefix(pos_espaco != std::string_view::npos ? pos_espaco + 1 : linha.size());
        
        Usuario usuario(usuarioId);
        float mag_quadrada = 0.0f;

        while (!linha.empty()) {
            int filmeId;
            float nota;

            auto pos_dois_pontos = linha.find(':');
            std::from_chars(linha.data(), linha.data() + pos_dois_pontos, filmeId);
            linha.remove_prefix(pos_dois_pontos + 1);

            auto pos_proximo_espaco = linha.find(' ');
            std::from_chars(linha.data(), linha.data() + pos_proximo_espaco, nota);
            linha.remove_prefix(pos_proximo_espaco != std::string_view::npos ? pos_proximo_espaco + 1 : linha.size());

            usuario.adicionarAvaliacao(filmeId, nota);
            mag_quadrada += nota * nota;
        }
        
        usuario.finalizarEOrdenarAvaliacoes();
        
        // --- CORREÇÃO APLICADA AQUI ---
        // O nome da variável foi corrigido de 'userId' para 'usuarioId'.
        magnitudes[usuarioId] = std::sqrt(mag_quadrada);
        dadosUsuarios.emplace(usuarioId, std::move(usuario));
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
    return {};
}

const std::unordered_map<int, Usuario>& GerenciadorDeDados::getTodosUsuarios() const {
    return dadosUsuarios;
}

float GerenciadorDeDados::getMagnitude(int usuarioId) const {
    return magnitudes.at(usuarioId);
}
