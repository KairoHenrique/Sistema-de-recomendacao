#include "CalculadorDeSimilaridade.hpp"
#include <cmath>

float CalculadorDeSimilaridade::calcularSimilaridadeCosseno(const std::unordered_map<int, float>& u1,
                                                          const std::unordered_map<int, float>& u2)
{
    float numerador = 0.0f, mag1 = 0.0f, mag2 = 0.0f;

    for (const auto& [item, nota1] : u1) {
        if (u2.count(item)) {
            float nota2 = u2.at(item);
            numerador += nota1 * nota2;
        }
        mag1 += nota1 * nota1;
    }

    for (const auto& [_, nota2] : u2) {
        mag2 += nota2 * nota2;
    }

    if (mag1 == 0 || mag2 == 0) return 0.0f;
    return numerador / (std::sqrt(mag1) * std::sqrt(mag2));
}


