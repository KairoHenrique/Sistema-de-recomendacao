#ifndef FUNCOESDETEXTO_HPP
#define FUNCOESDETEXTO_HPP

#include <string>
#include <vector>

class FuncoesDeTexto {
  public:
    static std::vector<std::string> dividir(const std::string &linha, char delimitador);
};


#endif // !FUNCOESDETEXTO_HPP
