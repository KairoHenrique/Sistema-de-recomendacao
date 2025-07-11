<h1> SISTEMA DE RECOMENDAÇÃO </h1>

- **Alunos:** Kairo Henrique, Tauane Luísa, Igor Moreira, Maria Eduarda, Heitor Henrique.
- **Professor:** Michel Pires da Silva  
- **Disciplina:** Algoritmos e Estruturas de Dados I  

---

## **Introdução**

Este projeto consiste no desenvolvimento de um sistema de recomendação capaz
de sugerir agrupamentos de elementos similares a partir de perfis de usuários e
características de itens. O trabalho foi desenvolvido como parte da disciplina de
Algoritmos e Estruturas de Dados I, no Centro Federal de Educação Tecnológica
de Minas Gerais (CEFET-MG)

---

## **Problema Proposto**

O objetivo principal deste trabalho é desenvolver um sistema de recomendação
que, a partir de perfis de usuários e características de itens (filmes, neste caso),
seja capaz de sugerir agrupamentos de elementos similares. O sistema deve ser
capaz de calcular medidas de similaridade para identificar afinidades entre usuários e
itens, gerando recomendações personalizadas.

---

## **Funcionalidades**

O sistema de recomendação implementa as seguintes funcionalidades:

- **Pré-processamento de Dados**: Realiza a limpeza e organização dos dados
brutos da base MovieLens 25M. Isso inclui a filtragem de usuários com
poucas avaliações, filmes com poucas avaliações, remoção de duplicatas
e a geração de um arquivo de entrada padronizado (input.bin) para
otimização do carregamento
- **Carregamento e Gerenciamento de Dados**: Eficientemente carrega e
gerencia os dados de usuários, filmes e suas avaliações a partir de arquivos
binários, otimizando o acesso para o processo de recomendação.
- **Cálculo de Similaridade**: Calcula a similaridade entre usuários utilizando
a métrica de similaridade de cosseno. Isso permite identificar usuários
com gostos e padrões de avaliação semelhantes.
- **Geração de Recomendações**: Com base nos usuários mais similares, o
sistema identifica filmes que o usuário-alvo ainda não avaliou, mas que
foram bem avaliados pelos seus vizinhos mais próximos. As
recomendações são então geradas e salvas em um arquivo de saída
(output.dat).
- **Suporte a Múltiplas Threads**: O processo de recomendação é otimizado
para utilizar múltiplas threads, aproveitando o hardware disponível para
acelerar o cálculo de similaridade e a geração de recomendações.
---

## **Requisitos Técnicos**

Para compilar e executar o sistema de recomendação, são necessários os
seguintes requisitos:

- **Sistema Operacional**: Linux Ubuntu 24.04 LTS (ou compatível).
- **Compilador**: g++ versão 13 ou superior.
- **Linguagens**: C e C++.
- **Bibliotecas**: Exclusivo uso da biblioteca padrão da linguagem C++.
- **Base de Dados**: MovieLens 25M (especificamente o arquivo ratings.csv e
movies.csv).

---

## **Estrutura de Dados e Metodologia**

Segue a seguir um fluxograma representando o funcionamento da estrutura do programa

**********Fluxograma

O sistema é modularizado e organizado em classes, cada uma com
responsabilidades bem definidas:

- **Configuracao.hpp/.cpp**: Gerencia os parâmetros de configuração do
sistema, como o número de vizinhos (K_VIZINHOS), o número de
recomendações a serem geradas (N_RECOMENDACOES) e o número de
usuários a serem explorados (N_USUARIOS_EXPLORAR).
- **Filme.hpp/.cpp**: Representa um filme, armazenando seu ID e,
potencialmente, outras informações relevantes.
- **Usuario.hpp/.cpp**: Representa um usuário, contendo seu ID e um vetor de
pares (ID do filme, nota) para suas avaliações. Também armazena a
magnitude do vetor de avaliações para otimização do cálculo de
similaridade.
- **Preprocessador.hpp/.cpp**: Responsável pelo pré-processamento dos
dados. Lê o arquivo ratings.csv, filtra usuários e filmes conforme os critérios
definidos (mínimo de 50 avaliações para usuários e filmes), remove
duplicatas e gera arquivos binários (input.bin e explore.bin) para
carregamento eficiente dos dados.
- **GerenciadorDeDados.hpp/.cpp**: Encarregado de carregar e gerenciar os
dados de usuários e filmes. Carrega os dados pré-processados de input.bin
e os nomes dos filmes de movies.csv, disponibilizando-os para o restante
do sistema.
- **CalculadorDeSimilaridade.hpp/.cpp**: Implementa o algoritmo para
calcular a similaridade entre dois usuários. A métrica utilizada é asimilaridade de cosseno, que mede o ângulo entre os vetores de avaliação
dos usuários. A implementação é otimizada para eficiência.
- **Recomendador.hpp/.cpp**: O coração do sistema de recomendação. Para
cada usuário no arquivo explore.bin, ele busca os K_VIZINHOS mais
similares utilizando o CalculadorDeSimilaridade. Em seguida, identifica
filmes bem avaliados por esses vizinhos que o usuário-alvo ainda não viu e
gera as N_RECOMENDACOES finais, salvando-as no arquivo output.dat. O
processo de recomendação é paralelizado usando múltiplas threads para
melhorar a performance.
- • **main.cpp**: Orquestra o fluxo principal do programa, chamando as funções
de pré-processamento, carregamento de dados e o processo de
recomendação. Também mede e exibe os tempos de execução de cada
etapa.


---

## **Otimizações**

******************Graficos sobre as Versoes e falando sobre otimizacoes flags de otimizacao makefile

---

## **Fluxo de Execução**
1. **Pré-processamento**: O Preprocessador lê ratings.csv e movies.csv, filtra os
dados e gera input.bin (dados de usuários e avaliações) e explore.bin
(usuários para os quais gerar recomendações).
2. **Carregamento de Dados**: O GerenciadorDeDados carrega input.bin e
movies.csv para a memória.
3. **Recomendação**: O Recomendador itera sobre os usuários em explore.bin.
o Para cada usuário, o CalculadorDeSimilaridade é usado para
encontrar os K_VIZINHOS mais similares.
o Filmes não avaliados pelo usuário-alvo, mas bem avaliados pelos
vizinhos, são identificados.
o As N_RECOMENDACOES são geradas e escritas em output.dat.
4. **Otimização**: O sistema utiliza std::ios_base::sync_with_stdio(false) e
std::cin.tie(NULL) para otimização de I/O, e paralelização com std::thread
para acelerar o processo de recomendação.

---

## **Ambiente de Teste**
O projeto foi desenvolvido e testado no seguinte ambiente:
- **Sistema Operacional**: Linux Debian GNU 12
- **Hardware**: Processador Ryzen 7 5700x - 32GB de Memoria - SSD NVME
- **Compilador**: g++ 13 ou superior.
- **Linguagens**: C e C++.
- **Base de Dados**: MovieLens 25M Ratings e Movies (disponível no Kaggle).

## **Organização do Repositório**

 
Sistema-de-recomendacao/  
├── src/ # Código fonte do projeto  
│ ├── CalculadorDeSimilaridade.cpp  
│ ├── Configuracao.cpp  
│ ├── Filme.cpp  
│ ├── GerenciadorDeDados.cpp  
│ ├── main.cpp  
│ ├── Preprocessador.cpp  
│ ├── Recomendador.cpp  
│ └── Usuario.cpp  
├── include/ # Arquivos de cabeçalho (.hpp)  
│ ├── CalculadorDeSimilaridade.hpp  
│ ├── Configuracao.hpp  
│ ├── Filme.hpp  
│ ├── GerenciadorDeDados.hpp  
│ ├── Preprocessador.hpp  
│ ├── Recomendador.hpp  
│ └── Usuario.hpp  
├── dados/ # Diretório para os arquivos de entrada (ratings.csv, movies.csv)  
├── resultados/ # Diretório para os arquivos de saída (output.dat)    
├── Makefile # Arquivo para compilação e execução do projeto  
├── README.md # Este arquivo README  
└── TrabalhoFinal.pdf # Documento com a especificação do trabalho

---

## **Blibliotecas Utilizada**
********** FALAR DE CADA UMA
Este projeto utiliza exclusivamente a biblioteca padrão da linguagem C++
(std::chrono, std::iostream, std::thread, std::string, std::vector, std::map,
std::algorithm, std::fstream, std::sstream, std::set, std::tuple, std::numeric). Não
foram utilizadas bibliotecas de terceiros

---
# **Resultados**
******tempos com numeros recomendacoes diferente (grafico de escalonamento) e o resultado do outpout

---


## **implementadas descartadas**
*****falar sobre utilizacoes implementadas descartadas como memory pool, Cuda (placa de video)

---

## **Melhorias Futuras**
Possíveis melhorias e funcionalidades a serem implementadas no futuro incluem:
- Implementação de Outras Métricas de Similaridade: Adicionar suporte para
outras métricas de similaridade, como Distância Euclidiana ou
Similaridade de Jaccard, permitindo a comparação de desempenho entre
elas.
- Otimização de Memória: Explorar estruturas de dados mais eficientes ou
técnicas de compressão para reduzir o consumo de memória,
especialmente para bases de dados maiores.
- Interface de Usuário: Desenvolver uma interface gráfica (GUI) ou uma
interface de linha de comando mais interativa para facilitar a interação com
o sistema.
- Persistência de Recomendações: Implementar um mecanismo para
persistir as recomendações geradas em um banco de dados ou outro
formato, permitindo o acesso posterior sem a necessidade de reexecutar o
processo completo.
- Recomendações Baseadas em Itens: Adicionar um módulo de
recomendação baseado em itens, que sugere filmes similares aos que o
usuário já gostou, em vez de apenas basear-se em usuários similares.
- Atualização Dinâmica da Base de Dados: Permitir que a base de dados de
avaliações seja atualizada dinamicamente, sem a necessidade de um préprocessamento completo a cada nova avaliação.
- Avaliação de Recomendações: Implementar métricas de avaliação de
recomendação (e.g., precisão, recall, F1-score) para quantificar a
qualidade das sugestões geradas.

---
 
# **Conclusão**
