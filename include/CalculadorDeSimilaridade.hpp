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
    static float calcularSimilaridadeCosseno(
        const std::vector<std::pair<int, float>>& u1, float mag1,
        const std::vector<std::pair<int, float>>& u2, float mag2);
};

#endif // CALCULADOR_DE_SIMILARIDADE_HPP


