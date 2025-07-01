#ifndef CALCULADOR_DE_SIMILARIDADE_HPP
#define CALCULADOR_DE_SIMILARIDADE_HPP

#include <vector>
#include <utility>
#include <cmath> // Para sqrt

// --- CORREÇÃO DE LINKER ---
// A implementação do método foi movida para o header e marcada como 'inline'
// para resolver o erro de "múltiplas definições".

class CalculadorDeSimilaridade {
public:
    inline static float calcularSimilaridadeCosseno(
        const std::vector<std::pair<int, float>>& u1, float mag1,
        const std::vector<std::pair<int, float>>& u2, float mag2)
    {
        if (mag1 == 0.0f || mag2 == 0.0f) return 0.0f;

        float numerador = 0.0f;
        size_t i = 0, j = 0;

        while (i < u1.size() && j < u2.size()) {
            if (u1[i].first < u2[j].first) {
                i++;
            } else if (u2[j].first < u1[i].first) {
                j++;
            } else {
                numerador += u1[i].second * u2[j].second;
                i++;
                j++;
            }
        }
        
        return numerador / (mag1 * mag2);
    }
};

#endif // CALCULADOR_DE_SIMILARIDADE_HPP
