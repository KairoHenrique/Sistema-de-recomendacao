#ifndef FILME_HPP
#define FILME_HPP

#include <string>

// Classe Filme
// Representa um filme com seu ID e nome.
class Filme {
private:
    int id;          // Identificador único do filme.
    std::string nome; // Nome/título do filme.
public:
    // Construtor da classe Filme.
    // id: O identificador único do filme.
    // nome: O nome/título do filme.
    Filme(int id, const std::string& nome);

    // Retorna o nome do filme.
    const std::string& getNome() const;

    // Retorna o ID do filme.
    int getId() const;
};

#endif


