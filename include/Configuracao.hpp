#ifndef CONFIGURACAO_HPP
#define CONFIGURACAO_HPP

class Configuracao {
public:
    int K_VIZINHOS;
    int N_RECOMENDACOES;
    int N_USUARIOS_EXPLORAR;

    Configuracao(
        int kVizinhos = 5,
        int nRecomendacoes = 10,
        int nUsuariosExplorar = 5
    );
};

#endif // CONFIGURACAO_HPP
