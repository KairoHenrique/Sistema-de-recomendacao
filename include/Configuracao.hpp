#ifndef CONFIGURACAO_HPP
#define CONFIGURACAO_HPP

class Configuracao {
public:
    const int K_VIZINHOS;
    const int N_RECOMENDACOES;
    const int N_USUARIOS_EXPLORAR;

    Configuracao(
        int kVizinhos = 5,
        int nRecomendacoes = 10,
        int nUsuariosExplorar = 5
    );
};

#endif // CONFIGURACAO_HPP
