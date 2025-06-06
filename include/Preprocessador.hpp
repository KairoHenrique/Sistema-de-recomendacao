#ifndef PREPROCESSADOR_HPP
#define PREPROCESSADOR_HPP

#include <string>

class Preprocessador {
public:
    static void gerarInput(const std::string& arquivoCSV, const std::string& arquivoSaida);
    static void gerarExplore(const std::string& inputDat, const std::string& exploreDat, int quantidade);
};

#endif // PREPROCESSADOR_HPP

