#include "Usuario.hpp"

Usuario::Usuario(int id) : id(id) {
        avaliacoes.reserve(160);
}
    
// Adiciona uma avaliação de filme para o usuário.
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
    
const std::vector<std::pair<int, float>>& Usuario::getAvaliacoes() const {
    return avaliacoes;
}
    
int Usuario::getId() const {
    return id;
}