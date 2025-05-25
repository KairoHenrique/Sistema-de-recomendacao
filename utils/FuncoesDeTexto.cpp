#include <sstream>
#include "FuncoesDeTexto.hpp"


// Recebe strings para e retorna um vetor, sem o delimitador - Igor
std::vector<std::string> FuncoesDeTexto::dividir(const std::string &linha, char delimitador) {
  std::vector<std::string> tokens;
  std::stringstream ss(linha);
  std::string item;

  while (std::getline(ss, item, delimitador)) {
    tokens.push_back(item);
  }
  return tokens;
}
