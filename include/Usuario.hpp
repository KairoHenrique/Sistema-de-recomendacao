#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <vector>
#include <utility>
#include <algorithm> // Para std::sort

// --- CORREÇÃO DE LINKER ---
// As implementações dos métodos foram movidas para o header e marcadas como 'inline'
// para resolver o erro de "múltiplas definições".

class Usuario {
private:
    int id;
    std::vector<std::pair<int, float>> avaliacoes; // Par: {filmeId, nota}

public:
    inline Usuario(int id) : id(id) {
        avaliacoes.reserve(160);
    }
    
    inline void adicionarAvaliacao(int filmeId, float nota) {
        avaliacoes.emplace_back(filmeId, nota);
    }
    
    inline void finalizarEOrdenarAvaliacoes() {
        std::sort(avaliacoes.begin(), avaliacoes.end(), 
                  [](const auto& a, const auto& b) {
                      return a.first < b.first;
                  });
    }
    
    inline const std::vector<std::pair<int, float>>& getAvaliacoes() const {
        return avaliacoes;
    }
    
    inline int getId() const {
        return id;
    }
};

#endif // USUARIO_HPP
