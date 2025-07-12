<h> 📽️SISTEMA DE RECOMENDAÇÃO📽️ </h>

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
brutos da base <a href ="https://www.kaggle.com/datasets/garymk/movielens-25m-dataset">MovieLens 25M</a>. Isso inclui a filtragem de usuários com
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

- **Sistema Operacional**: Debian GNU/Linux 12.7 (ou compatível).
- **Compilador**: g++ versão 13 ou superior.
- **Linguagens**: C e C++.
- **Bibliotecas**: Exclusivo uso da biblioteca padrão da linguagem C++.
- **Base de Dados**: MovieLens 25M (especificamente o arquivo ratings.csv e
movies.csv).

---

## **Estrutura de Dados e Metodologia**

Segue a seguir um fluxograma representando o funcionamento da estrutura do programa

<details> 
  <summary><strong>Fluxograma</strong></summary>
<img src="Fluxograma.png"/>

</details> 

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
- **main.cpp**: Orquestra o fluxo principal do programa, chamando as funções
de pré-processamento, carregamento de dados e o processo de
recomendação. Também mede e exibe os tempos de execução de cada
etapa.


---

## **Otimizações**
<details> 
  <summary><strong>Grafico de custo e tempo medio entre as versões</strong></summary>
<img src="imagem a ser utilizada.png"/>

</details> 
******************Graficos sobre as Versoes e falando sobre otimizacoes flags de otimizacao makefile

---

## **Fluxo de Execução**
1. **Pré-processamento**: O Preprocessador lê ratings.csv e movies.csv, filtra os
dados e gera input.bin (dados de usuários e avaliações) e explore.bin
(usuários para os quais gerar recomendações).
2. **Carregamento de Dados**: O GerenciadorDeDados carrega input.bin e
movies.csv para a memória.
3. **Recomendação**: O Recomendador itera sobre os usuários em explore.bin.
- Para cada usuário, o CalculadorDeSimilaridade é usado para
encontrar os K_VIZINHOS mais similares.
- Filmes não avaliados pelo usuário-alvo, mas bem avaliados pelos
vizinhos, são identificados.
- As N_RECOMENDACOES são geradas e escritas em output.dat.
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

```

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

```
---

## **Bibliotecas Utilizadas**

Este projeto foi construído utilizando exclusivamente recursos da **Biblioteca Padrão do C++**. Nenhuma biblioteca de terceiros foi necessária. 
Abaixo estão as principais bibliotecas utilizadas e suas finalidades no projeto:

### **Manipulação de Dados e Contêineres**
* **`<vector>`**: A principal estrutura de dados do projeto. Armazena a lista de avaliações (pares de ID do filme e nota) para cada usuário.
* **`<string>`** e **`<string_view>`**: Utilizadas para toda a manipulação de texto. `std::string` armazena dados que precisam de posse (como nomes de arquivos), enquanto `std::string_view` é uma otimização crucial usada para visualizar e processar "fatias" de strings sem criar cópias, sendo fundamental no parsing de alta performance dos arquivos CSV.
* **`<unordered_map>`**: Estrutura de hash de alta performance, essencial para mapear os IDs de usuários e filmes aos seus objetos e dados correspondentes. Garante acesso em tempo médio constante O(1).
* **`<unordered_set>`**: Usado para validações rápidas, como verificar os filmes que um usuário já assistiu ou filtrar usuários e filmes válidos durante o pré-processamento.
* **`<utility>`**: Fornece o `std::pair`, utilizado para agrupar o ID de um filme e a nota dada pelo usuário em um único elemento dentro do vetor de avaliações.
* **`<charconv>`**: (C++17) Acesso à função `std::from_chars`, uma forma moderna e extremamente rápida de converter sequências de caracteres para tipos numéricos, usada para otimizar o parsing do arquivo `ratings.csv`.

### **Algoritmos e Funções Matemáticas**
* **`<algorithm>`**: Fornece algoritmos essenciais e otimizados, como `std::sort`, `std::partial_sort` (para encontrar os K vizinhos e N recomendações de forma eficiente) e `std::shuffle`.
* **`<cmath>`**: Utilizada para a função `std::sqrt`, necessária para calcular a magnitude (norma euclidiana) do vetor de avaliações de cada usuário.
* **`<random>`**: Usada para gerar aleatoriedade de boa qualidade (`std::random_device`, `std::mt19937`) para o `std::shuffle`.

### **Entrada/Saída (I/O)**
* **`<iostream>`**: Para operações de entrada e saída no console (`std::cout`, `std::cerr`).
* **`<fstream>`**: Essencial para a manipulação de arquivos (leitura de `.csv` e leitura/escrita de `.bin`).


### **Programação Concorrente e Paralelismo**
* **`<thread>`**: A base do paralelismo no projeto, usada para criar e gerenciar as threads que dividem o trabalho.
* **`<mutex>`**: Garante a segurança na escrita concorrente no arquivo `output.dat` com `std::lock_guard`.
* **`<future>`**: Utilizada com `std::async` para gerenciar de forma assíncrona o pré-processamento dos dados.
* **`<atomic>`**: Fornece `std::atomic` para controlar o índice do próximo usuário a ser processado pelas threads de forma segura e sem a necessidade de um `mutex`.

### **#Medição de Tempo**
* **`<chrono>`**: Usada para medir com alta precisão o tempo de execução do sistema.
---
# **Resultados**
******tempos com numeros recomendacoes diferente (grafico de escalonamento) e o resultado do outpout
<details> 
  <summary><strong>Grafico de escalonamento do tempo de acordo com o número de usuários</strong></summary>
<img src="imagem a ser utilizada.png"/>

</details>

---

<details> 
  <summary><strong> Exemplo de Output</strong></summary>

```

45024 123:Chungking Express (Chung Hing sam lam) (1994) 1411:Hamlet (1996) 860:Maybe, Maybe Not (Bewegte Mann, Der) (1994)

```

Este Output foi gerado utilizando os seguintes valores N_RECOMENDACOES = 3 e N_USUARIOS_EXPLORAR = 1 
```
formato do output:'ID Usuário' 'ID Filme':'Nome Filme'
```

</details> 

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
recomendação (precisão, recall, F1-score) para quantificar a
qualidade das sugestões geradas.

---
 
# **Conclusão**
