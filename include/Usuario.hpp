#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <vector>
#include <utility>
#include <algorithm>

class Usuario {
private:
    int id;
    std::vector<std::pair<int, float>> avaliacoes;

public:
    Usuario(int id);
    void adicionarAvaliacao(int filmeId, float nota);
    void finalizarEOrdenarAvaliacoes();
    const std::vector<std::pair<int, float>>& getAvaliacoes() const;
    int getId() const;
};

#endif
