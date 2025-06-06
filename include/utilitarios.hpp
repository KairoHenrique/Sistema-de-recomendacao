#ifndef UTILITARIOS_HPP
#define UTILITARIOS_HPP

#include <string>
#include <vector>

// Divide uma string por um delimitador
std::vector<std::string> dividir(const std::string& linha, char delimitador);

// Verifica se um arquivo existe
bool arquivoExiste(const std::string& caminho);

#endif // UTILITARIOS_HPP

