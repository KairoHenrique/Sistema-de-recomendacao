#ifndef CONFIGURACAO_HPP
#define CONFIGURACAO_HPP

// Classe Configuracao
// Armazena os parâmetros de configuração para o sistema de recomendação.
class Configuracao {
public:
    int K_VIZINHOS;          // Número de vizinhos a serem considerados para a recomendação.
    int N_RECOMENDACOES;     // Número de recomendações a serem geradas por usuário.
    int N_USUARIOS_EXPLORAR; // Número de usuários a serem explorados para análise.

    // Construtor da classe Configuracao.
    // Inicializa os parâmetros com valores padrão ou valores fornecidos.
    // kVizinhos: Número de vizinhos (padrão: 20).
    // nRecomendacoes: Número de recomendações (padrão: 10).
    // nUsuariosExplorar: Número de usuários para explorar (padrão: 5).
    Configuracao(
        int kVizinhos = 20,
        int nRecomendacoes = 3,
        int nUsuariosExplorar = 1
    );
};

#endif


