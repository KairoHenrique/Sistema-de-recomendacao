#include "Usuario.hpp"

Usuario::Usuario(int id) : id(id) {
        avaliacoes.reserve(160); // Pré-aloca espaço para otimização.
}
    
// Adiciona uma avaliação de filme para o usuário.
// filmeId: O ID do filme avaliado.
// nota: A nota atribuída ao filme.
void Usuario::adicionarAvaliacao(int filmeId, float nota) {
    avaliacoes.emplace_back(filmeId, nota);
}
    
// Finaliza a adição de avaliações e as ordena por ID do filme.
// Essencial para o cálculo eficiente da similaridade de cosseno.
void Usuario::finalizarEOrdenarAvaliacoes() {
    std::sort(avaliacoes.begin(), avaliacoes.end(), 
            [](const auto& a, const auto& b) {
                return a.first < b.first;
            });
}
    
// Retorna uma referência constante ao vetor de avaliações do usuário.
const std::vector<std::pair<int, float>>& Usuario::getAvaliacoes() const {
    return avaliacoes;
}
    
// Retorna o ID do usuário.
int Usuario::getId() const {
    return id;
}