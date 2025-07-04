#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <vector>
#include <utility>
#include <algorithm> // Para std::sort

// Classe Usuario
// Representa um usuário e suas avaliações de filmes.
class Usuario {
private:
    int id; // ID único do usuário.
    std::vector<std::pair<int, float>> avaliacoes; // Vetor de pares: {ID do filme, nota da avaliação}.

public:
    // Construtor da classe Usuario.
    // id: O ID único do usuário.
    Usuario(int id);
    
    // Adiciona uma avaliação de filme para o usuário.
    // filmeId: O ID do filme avaliado.
    // nota: A nota atribuída ao filme.
    void adicionarAvaliacao(int filmeId, float nota);
    
    // Finaliza a adição de avaliações e as ordena por ID do filme.
    // Essencial para o cálculo eficiente da similaridade de cosseno.
    void finalizarEOrdenarAvaliacoes();
    
    // Retorna uma referência constante ao vetor de avaliações do usuário.
    const std::vector<std::pair<int, float>>& getAvaliacoes() const;
    
    // Retorna o ID do usuário.
    int getId() const;
};

#endif


