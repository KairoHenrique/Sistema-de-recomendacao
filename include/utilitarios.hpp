#ifndef UTILITARIOS_HPP
#define UTILITARIOS_HPP
#include <string>
#include <vector>

// Função utilitária para ler o conteúdo completo de um arquivo para uma string.
// caminho: O caminho para o arquivo a ser lido.
// Retorna uma string contendo todo o conteúdo do arquivo.
std::string lerArquivoInteiro(const std::string& caminho);

// Função utilitária para dividir uma string em tokens com base em um delimitador.
// linha: A string a ser dividida.
// Retorna um vetor de strings contendo os tokens.
std::vector<std::string> dividir(const std::string& linha, char delimitador);

// Função utilitária para verificar se um arquivo existe no caminho especificado.
// Retorna true se o arquivo existe e pode ser aberto, false caso contrário.
bool arquivoExiste(const std::string& caminho);

#endif


