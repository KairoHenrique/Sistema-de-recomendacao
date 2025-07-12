#include "CalculadorDeSimilaridade.hpp"

// Calcula a similaridade de cosseno entre dois usuários.
float CalculadorDeSimilaridade::calcularSimilaridadeCosseno(
    const std::vector<std::pair<int, float>>& u1, float mag1,
    const std::vector<std::pair<int, float>>& u2, float mag2)
{
    if (mag1 == 0.0f || mag2 == 0.0f) return 0.0f;

    float numerador = 0.0f;
    size_t i = 0, j = 0;

    // Percorre os vetores ordenados para encontrar filmes avaliados em comum.
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
