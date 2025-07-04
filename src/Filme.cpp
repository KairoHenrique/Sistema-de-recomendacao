#include "Filme.hpp"

// Construtor da classe Filme.
// id: Identificador único do filme.
// nome: Título do filme.
Filme::Filme(int id, const std::string& nome) : id(id), nome(nome) {}

// Retorna o nome do filme.
const std::string& Filme::getNome() const {
    return nome;
}

// Retorna o ID do filme.
int Filme::getId() const {
    return id;
}


