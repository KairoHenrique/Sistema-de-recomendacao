#include "utilitarios.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

// Função para verificar se um arquivo existe no caminho especificado.
// caminho: O caminho completo do arquivo.
// Retorna true se o arquivo existe e pode ser aberto, false caso contrário.
bool arquivoExiste(const std::string& caminho) {
    std::ifstream file(caminho); // Tenta abrir o arquivo.
    return file.good(); // Retorna true se a abertura foi bem-sucedida.
}


