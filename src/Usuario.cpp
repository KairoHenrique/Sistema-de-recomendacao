#include "Usuario.hpp"

Usuario::Usuario(int id) : id(id) {}

void Usuario::adicionarAvaliacao(int filmeId, float nota) {
    avaliacoes[filmeId] = nota;
}

const std::unordered_map<int, float>& Usuario::getAvaliacoes() const {
    return avaliacoes;
}

int Usuario::getId() const {
    return id;
}
