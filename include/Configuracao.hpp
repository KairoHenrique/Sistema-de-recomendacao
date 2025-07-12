#ifndef CONFIGURACAO_HPP
#define CONFIGURACAO_HPP

// Parâmetros de configuração do sistema
class Configuracao {
public:
    int K_VIZINHOS;          // Número de vizinhos a serem considerados para a recomendação.
    int N_RECOMENDACOES;     // Número de recomendações a serem geradas por usuário.
    int N_USUARIOS_EXPLORAR; // Número de usuários a serem explorados para análise.

    Configuracao(
        int kVizinhos = 20,
        int nRecomendacoes = 10,
        int nUsuariosExplorar = 50
    );
};

#endif


