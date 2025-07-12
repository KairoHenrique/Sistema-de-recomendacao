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


Além dos algoritmos, a alta performance do sistema é garantida por um conjunto de otimizações de baixo nível, focadas em extrair o máximo do hardware e do sistema operacional.

---

### **Otimizações de Entrada e Saída (I/O)**

A manipulação de arquivos é frequentemente um gargalo. Para mitigar isso, foram implementadas as seguintes estratégias:

#### **Leitura de Arquivos em Bloco Único**
Em vez de ler os arquivos de dados (`.csv` ou `.bin`) linha por linha, o que envolveria múltiplas chamadas de sistema custosas, o sistema adota uma leitura em bloco.

* **Como Funciona:**
    1.  O arquivo é aberto e o ponteiro de leitura é movido para o final (`std::ios::ate`) para medir seu tamanho total com uma única chamada.
    2.  Um buffer (`std::string`) é pré-alocado na memória com o tamanho exato do arquivo. Isso evita múltiplas realocações dinâmicas, que são lentas.
    3.  O conteúdo inteiro do arquivo é lido para o buffer de memória com uma única operação `read()`.

* **O Ganho**: Esta técnica **minimiza a sobrecarga de comunicação com o sistema operacional** e elimina o custo de realocação de memória, resultando em um carregamento de arquivos drasticamente mais rápido.

#### **Desvinculação dos Streams de I/O do C++**
No início da função `main`, duas linhas preparam o ambiente de I/O para máxima velocidade em C++:

* **`std::ios_base::sync_with_stdio(false);`**: Por padrão, os streams de I/O do C++ (`cin`, `cout`) são sincronizados com os streams do C (`printf`, `scanf`) por questões de compatibilidade. Desativar essa sincronização remove uma camada de sobrecarga significativa, tornando o `std::cout` muito mais rápido.
* **`std::cin.tie(NULL);`**: Por padrão, `cout` está "amarrado" a `cin`, o que significa que antes de qualquer operação de leitura, o buffer de saída é automaticamente "descarregado" (flushed). Como nosso programa não tem entrada interativa, desamarrar os dois remove essa operação de flush desnecessária.

* **O Ganho**: Acelera todas as operações de escrita no console, como a exibição de logs e tempos de execução, que, embora pareçam simples, podem somar um tempo considerável em programas que geram muita saída.

---

### **Otimizações de Compilação (Flags)**

O `Makefile` do projeto está configurado para instruir o compilador `g++` a realizar otimizações agressivas, transformando o código C++ em um código de máquina altamente eficiente.

* **`-O3`**: É o nível de otimização mais agressivo. Ele habilita um vasto conjunto de técnicas, como *loop unrolling* (desenrolamento de laços), *inlining* de funções e reordenação de instruções para melhor aproveitar o pipeline do processador.

* **`-march=native`**: Esta flag é crucial para performance. Ela instrui o compilador a gerar código otimizado especificamente para a arquitetura da CPU da máquina onde a compilação está ocorrendo. Isso permite o uso de conjuntos de instruções mais modernos e rápidos (como **AVX** e **FMA**), que não seriam usados em um binário genérico e podem acelerar massivamente os cálculos de ponto flutuante.

* **`-flto` (Link-Time Optimization)**: Uma otimização poderosa que ocorre na fase final de linkagem. Ela permite que o compilador analise e otimize o programa **como um todo**, enxergando as interações entre todos os diferentes arquivos de código-fonte, em vez de otimizar cada um isoladamente. Isso possibilita otimizações mais profundas.

* **`-ffast-math`**: Relaxa algumas regras estritas de precisão de ponto flutuante do padrão IEEE 754. Isso dá ao compilador a liberdade de fazer otimizações matemáticas mais agressivas, como reassociar operações, o que é especialmente útil em laços computacionais intensos como os do cálculo de similaridade.

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



### **Medição de Tempo**
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


## Implementações Descartadas

Durante o desenvolvimento, algumas técnicas de otimização avançada foram exploradas. No entanto, elas foram descartadas por não apresentarem um ganho de desempenho significativo que justificasse o aumento na complexidade do código ou por introduzirem novos gargalos. As principais foram:

### **Memory Pool**
* **A Ideia**: Substituir o alocador de memória padrão do C++ por um *memory pool*. A teoria era que, ao alocar um grande bloco de memória de uma só vez e gerenciá-lo manualmente para os objetos `Usuario` e suas avaliações, poderíamos reduzir a sobrecarga de múltiplas chamadas ao sistema operacional e melhorar a localidade de cache.
* **Motivo do Descarte**: O padrão de alocação deste projeto consiste em poucas e grandes alocações no início da execução para carregar os dados. Os alocadores padrão das bibliotecas C++ modernas (como o `glibc malloc`) já são extremamente eficientes para este cenário. Nos testes, o ganho de performance com o *memory pool* foi **marginal e inconsistente**, não justificando a complexidade de implementar, depurar e manter um gerenciador de memória customizado.

### **Computação em GPU com CUDA**
* **A Ideia**: Portar o cálculo da similaridade de cosseno, que é a parte mais intensiva do ponto de vista computacional, para ser executado em paralelo na GPU utilizando a plataforma CUDA da NVIDIA.
* **Motivo do Descarte**: Embora a GPU seja massivamente mais rápida para cálculos de produto escalar, o principal gargalo do sistema não é apenas a computação, mas também a **transferência de dados**. Para cada usuário sendo processado, seria necessário copiar os vetores de avaliação de milhares de outros usuários da memória RAM para a memória VRAM. O tempo gasto nessa transferência de dados **superou o ganho obtido com o processamento acelerado**. O resultado final foi um desempenho inferior à abordagem multi-thread na CPU, que opera diretamente nos dados já carregados na RAM. Além disso, a implementação adicionaria uma forte dependência do ecossistema NVIDIA, reduzindo a portabilidade do projeto e dificuldade em testes, além disso vimos que ela so seria superior com uma quantitade muito maior de dados

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


falar sobre analise assintotica e concluir
