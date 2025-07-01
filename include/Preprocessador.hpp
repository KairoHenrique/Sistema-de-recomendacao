#ifndef PREPROCESSADOR_HPP
#define PREPROCESSADOR_HPP

#include <string>

// Forward declaration para evitar include circular
class GerenciadorDeDados;

class Preprocessador {
public:
    static void gerarInput(const std::string& arquivoCSV, const std::string& arquivoSaida);
    // --- MODIFICAÇÃO APLICADA AQUI ---
    // Agora recebe uma referência ao gerenciador em vez de ler um arquivo .dat
    static void gerarExplore(GerenciadorDeDados& gerenciador, const std::string& exploreDat, int quantidade);
};

#endif
