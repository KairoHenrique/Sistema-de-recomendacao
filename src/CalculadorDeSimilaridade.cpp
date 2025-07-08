#include "CalculadorDeSimilaridade.hpp"

// Calcula a similaridade de cosseno entre dois usuários com base em suas avaliações.
// u1 e u2 são os vetores de pares (ID do filme, nota) para o primeiro e segundo usuários, já ordenado por ID do filme.
// mag1 e mag2 são a magnitude (norma) do vetor de avaliações dos usuários.
// Retorna um float representando a similaridade de cosseno, ou 0.0f se alguma magnitude for zero.
float CalculadorDeSimilaridade::calcularSimilaridadeCosseno(
    const std::vector<std::pair<int, float>>& u1, float mag1,
    const std::vector<std::pair<int, float>>& u2, float mag2)
{
    if (mag1 == 0.0f || mag2 == 0.0f) return 0.0f;

    float numerador = 0.0f; 
    size_t i = 0, j = 0; // os índices para o while

    // Percorre os vetores de avaliações (como em merge sort).
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





