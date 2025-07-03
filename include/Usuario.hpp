#ifndef USUARIO_HPP
#define USUARIO_HPP
#include <vector>
#include <utility>
#include <algorithm>
// Representa um usuário e suas avaliações de filmes.
class Usuario {
private:
    int id; // ID único do usuário.
    std::vector<std::pair<int, float>> avaliacoes; // Vetor de pares: {ID do filme, nota da avaliação}.

public:

    inline Usuario(int id) : id(id) {
        avaliacoes.reserve(160); // Pré-aloca espaço para otimização.
    }
    
    // Adiciona uma avaliação de filme para o usuário.
    // nota: A nota atribuída ao filme.
    inline void adicionarAvaliacao(int filmeId, float nota) {
        avaliacoes.emplace_back(filmeId, nota);
    }
    
    // Finaliza a adição de avaliações e as ordena por ID do filme.
    // Essencial para o cálculo eficiente da similaridade de cosseno.
    inline void finalizarEOrdenarAvaliacoes() {
        std::sort(avaliacoes.begin(), avaliacoes.end(), 
                  [](const auto& a, const auto& b) {
                      return a.first < b.first;
                  });
    }
    
    // Retorna uma referência constante ao vetor de avaliações do usuário.
    const std::vector<std::pair<int, float>>& getAvaliacoes() const {
        return avaliacoes;
    }
    
    // Retorna o ID do usuário.
    inline int getId() const {
        return id;
    }
};

#endif

