#pragma once 

#include <fstream>
#include <string>

class FileManager{
  public:
    static std::ifstream abrirLeitura(const std::string &caminhoArquivo);
    static std::ofstream abrirEscrita(const std::string &caminhoArquivo);
};

