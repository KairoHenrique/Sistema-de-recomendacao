#ifndef CALCULADOR_DE_SIMILARIDADE_HPP
#define CALCULADOR_DE_SIMILARIDADE_HPP

#include <vector>
#include <utility>
#include <cmath> // Para std::sqrt

// Classe responsável por calcular a similaridade entre usuários.
class CalculadorDeSimilaridade {
public:
    // Calcula a similaridade de cosseno entre dois usuários com base em suas avaliações.
    // u1: Vetor de pares (ID do filme, nota) para o primeiro usuário, já ordenado por ID do filme.
    // mag1: Magnitude (norma) do vetor de avaliações do primeiro usuário.
    // u2: Vetor de pares (ID do filme, nota) para o segundo usuário, já ordenado por ID do filme.
    // mag2: Magnitude (norma) do vetor de avaliações do segundo usuário.
    // Retorna um float representando a similaridade de cosseno, ou 0.0f se alguma magnitude for zero.
    inline static float calcularSimilaridadeCosseno(
        const std::vector<std::pair<int, float>>& u1, float mag1,
        const std::vector<std::pair<int, float>>& u2, float mag2)
    {
        // Se uma das magnitudes for zero, não há similaridade.
        if (mag1 == 0.0f || mag2 == 0.0f) return 0.0f;

        float numerador = 0.0f; // Inicializa o numerador da fórmula do cosseno.
        size_t i = 0, j = 0;    // Índices para percorrer os vetores de avaliações.

        // Percorre os vetores de avaliações de forma otimizada (como em merge sort).
        while (i < u1.size() && j < u2.size()) {
            if (u1[i].first < u2[j].first) {
                i++; // Avança no vetor do usuário 1 se o filme for menor.
            } else if (u2[j].first < u1[i].first) {
                j++; // Avança no vetor do usuário 2 se o filme for menor.
            } else {
                // Se os IDs dos filmes são iguais, adiciona o produto das notas ao numerador.
                numerador += u1[i].second * u2[j].second;
                i++;
                j++;
            }
        }
        
        // Retorna a similaridade de cosseno (numerador dividido pelo produto das magnitudes).
        return numerador / (mag1 * mag2);
    }
};

#endif // CALCULADOR_DE_SIMILARIDADE_HPP


