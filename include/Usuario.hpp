#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <unordered_map>

class Usuario {
private:
    int id;
    std::unordered_map<int, float> avaliacoes;

public:
    Usuario(int id);
    void adicionarAvaliacao(int filmeId, float nota);
    const std::unordered_map<int, float>& getAvaliacoes() const;
    int getId() const;
};

#endif // USUARIO_HPP

