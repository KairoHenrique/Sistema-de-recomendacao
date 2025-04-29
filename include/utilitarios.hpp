 #ifndef UTILITARIOS_HPP
#define UTILITARIOS_HPP

#include <string>
#include <vector>
#include <unordered_map>

std::vector<std::string> dividir(const std::string& linha, char delimitador);
bool arquivoExiste(const std::string& caminho);
void leituraRapidaCSV(const std::string& caminho,
    std::unordered_map<int, std::unordered_map<int, float>>& dadosUsuarios);

float similaridadeUsuarios(const std::unordered_map<int, float>& u1,
                           const std::unordered_map<int, float>& u2);

#endif
