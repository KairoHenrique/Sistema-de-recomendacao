#ifndef FILME_HPP
#define FILME_HPP

#include <string>

// Classe para armazenar o filme, com seu nome e id
class Filme {
private:
    int id;
    std::string nome;
public:
    Filme(int id, const std::string& nome);

    const std::string& getNome() const;

    int getId() const;
};

#endif


