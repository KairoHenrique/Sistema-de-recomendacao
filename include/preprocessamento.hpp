#ifndef PREPROCESSAMENTO_HPP
#define PREPROCESSAMENTO_HPP

#include <string>

// Carrega input.dat para memoria – Kairo
void carregarDados(const std::string& caminhoArquivo);

// Gera input.dat a partir de ratings.csv aplicando os filtros do trabalho – Kairo
void gerarInput(const std::string& arquivoCSV, const std::string& arquivoSaida);

// Gera explore.dat com N usuarios aleatorios de input.dat – Kairo
void gerarExplore(const std::string& inputDat, const std::string& exploreDat, int quantidade);

#endif
