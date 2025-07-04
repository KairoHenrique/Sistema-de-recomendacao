#include "Configuracao.hpp"

// Construtor da classe Configuracao
// Inicializa os parâmetros de configuração do sistema de recomendação.
// kVizinhos: Número de vizinhos a serem considerados para a recomendação.
// nRecomendacoes: Número de recomendações a serem geradas.
// nUsuariosExplorar: Número de usuários a serem explorados para análise.
Configuracao::Configuracao(
    int kVizinhos,
    int nRecomendacoes,
    int nUsuariosExplorar
) : K_VIZINHOS(kVizinhos),
    N_RECOMENDACOES(nRecomendacoes),
    N_USUARIOS_EXPLORAR(nUsuariosExplorar) {}


