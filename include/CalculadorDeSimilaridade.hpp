#ifndef CALCULADOR_DE_SIMILARIDADE_HPP
#define CALCULADOR_DE_SIMILARIDADE_HPP

#include <vector>
#include <utility>
#include <cmath> 

class CalculadorDeSimilaridade {
public:
    
    static float calcularSimilaridadeCosseno(
        const std::vector<std::pair<int, float>>& u1, float mag1,
        const std::vector<std::pair<int, float>>& u2, float mag2);
};

#endif


