#ifndef UTILITARIOS_HPP
#define UTILITARIOS_HPP

#include <string>
#include <vector>

// Declaração da função para que outros arquivos possam usá-la
std::string lerArquivoInteiro(const std::string& caminho);

std::vector<std::string> dividir(const std::string& linha, char delimitador);
bool arquivoExiste(const std::string& caminho);

#endif // UTILITARIOS_HPP
