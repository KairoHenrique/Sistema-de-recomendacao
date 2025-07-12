#ifndef PREPROCESSADOR_HPP
#define PREPROCESSADOR_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <string_view>

class GerenciadorDeDados;

// Contagens parciais e avaliações lidas de um chunk do CSV.
struct ContagemParcial {
    std::unordered_map<int, int> filmesCounts;
    std::unordered_map<int, int> usuariosCounts;
    std::vector<std::tuple<int, int, float>> avaliacoesBrutas;
};

// Classe com métodos estáticos para pré-processamento dos dados.
class Preprocessador {
private:
    static ContagemParcial processarChunk(std::string_view chunk);
    static void escreverInputBin(const std::string& arquivoInput, const std::unordered_map<int, std::vector<std::pair<int, float>>>& dados);

public:
    static void gerarInput(const std::string& arquivoCSV, const std::string& arquivoSaida);
    static void gerarExplore(GerenciadorDeDados& gerenciador, const std::string& exploreBin, int quantidade);
};

#endif
