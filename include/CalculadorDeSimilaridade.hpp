#ifndef CALCULADOR_DE_SIMILARIDADE_HPP
#define CALCULADOR_DE_SIMILARIDADE_HPP

#include <unordered_map>

class CalculadorDeSimilaridade {
public:
    static float calcularSimilaridadeCosseno(const std::unordered_map<int, float>& u1,
                                            const std::unordered_map<int, float>& u2);
};

#endif // CALCULADOR_DE_SIMILARIDADE_HPP

