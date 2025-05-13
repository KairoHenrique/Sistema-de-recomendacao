#ifndef UTILITARIOS_HPP
#define UTILITARIOS_HPP

#include <string>
#include <vector>
#include <unordered_map>

// Divide uma string por um delimitador – Kairo
std::vector<std::string> dividir(const std::string& linha, char delimitador);

// Verifica se um arquivo existe – Kairo
bool arquivoExiste(const std::string& caminho);

// Le input.dat e carrega os dados em memoria – Kairo
void leituraRapidaCSV(const std::string& caminho,
    std::unordered_map<int, std::unordered_map<int, float>>& dadosUsuarios);

// Calcula similaridade do cosseno entre dois usuarios – Kairo
float similaridadeUsuarios(const std::unordered_map<int, float>& u1,
                           const std::unordered_map<int, float>& u2);

// Le movies.csv e carrega os nomes dos filmes – Kairo
void carregarNomesFilmes(const std::string& caminhoCSV,
    std::unordered_map<int, std::string>& nomesFilmes);

#endif
