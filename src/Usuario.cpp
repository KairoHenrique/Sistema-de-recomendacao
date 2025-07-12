#include "Usuario.hpp"

Usuario::Usuario(int id) : id(id) {
    avaliacoes.reserve(160);  // Reserva espaço para até 160 avaliações para otimização.
}

// Adiciona uma avaliação (filme e nota) ao usuário.
void Usuario::adicionarAvaliacao(int filmeId, float nota) {
    avaliacoes.emplace_back(filmeId, nota);
}

// Ordena as avaliações por ID do filme para facilitar cálculos posteriores.
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
