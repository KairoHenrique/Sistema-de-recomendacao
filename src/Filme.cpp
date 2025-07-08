#include "Filme.hpp"

Filme::Filme(int id, const std::string& nome) : id(id), nome(nome) {}

const std::string& Filme::getNome() const {
    return nome;
}

int Filme::getId() const {
    return id;
}


