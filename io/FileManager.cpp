#include "FileManager.hpp"
#include <stdexcept>
#include <iostream>

// Tenta abrir o arquivo para escrita, se falhar, retorna uma excessão - Igor
std::ofstream FileManager::abrirEscrita(const std::string &caminhoArquivo) {
  std::ofstream arquivoEscrita;
  try {
    arquivoEscrita.open(caminhoArquivo);
    if (!arquivoEscrita.is_open()) {
      throw std::ios_base::failure("Erro, não foi possível abrir o arquivo para escrita " + caminhoArquivo);
    }
  } catch (std::runtime_error &e) {
      std::cout << e.what() << std::endl;
  }
  return arquivoEscrita;
}

// Tenta abrir o arquivo para Leitura, se falhar, retorna uma excessão - Igor
std::ifstream FileManager::abrirLeitura(const std::string &caminhoArquivo) {
  std::ifstream arquivoLeitura;
  try {
    arquivoLeitura.open(caminhoArquivo); 
      if (!arquivoLeitura.is_open()) {
        throw std::ios_base::failure("Erro, não foi possível abrir o arquivo para leitura " + caminhoArquivo);
      }
  } catch (std::runtime_error &e) {
    std::cout << e.what() << std::endl;
  }
  return arquivoLeitura;
}
