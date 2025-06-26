#include "CalculadorDeSimilaridade.hpp"
#include <cmath>

float CalculadorDeSimilaridade::calcularSimilaridadeCosseno(
    const std::unordered_map<int, float>& u1, float mag1,
    const std::unordered_map<int, float>& u2, float mag2)
{
    float numerador = 0.0f;
    
    // Iterar sobre o menor mapa para eficiência
    const auto& menorMapa = u1.size() < u2.size() ? u1 : u2;
    const auto& maiorMapa = u1.size() < u2.size() ? u2 : u1;
    
    for (const auto& [item, nota] : menorMapa) {
        auto it = maiorMapa.find(item);
        if (it != maiorMapa.end()) {
            numerador += nota * it->second;
        }
    }
    
    if (mag1 == 0 || mag2 == 0) return 0.0f;
    return numerador / (mag1 * mag2);
}
