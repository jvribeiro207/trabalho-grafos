#ifndef GULOSO_H
#define GULOSO_H

#include "Grafo.h"
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

using namespace std;

// Estrutura para retornar o resultado de uma execução
struct Solucao {
    vector<int> coloracao; // Cor de cada vértice (indice 1 a N)
    int maxCor;            // Maior cor utilizada (objetivo a minimizar)
    double tempoExecucao;  // Em segundos
    unsigned int semente;
    
    // Construtor padrão
    Solucao() : maxCor(-1), tempoExecucao(0.0), semente(0) {}
};

class Guloso {
private:
    Grafo& grafo;
    int p;
    int q;

    // Verifica se é válido pintar o vértice u com a cor c
    bool corValida(int u, int c, const vector<int>& coresAtuais);

    // Constrói uma solução completa usando a lógica gulosa (ou randomizada)
    // Se alpha = -1, usa guloso puro (ordem de maior grau).
    // Se alpha >= 0, usa estratégia randomizada (RCL).
    Solucao construirSolucao(float alpha, unsigned int seed);

public:
    Guloso(Grafo& g, int paramP, int paramQ);

    // 1. Algoritmo Guloso Básico (Ordenação por grau decrescente)
    Solucao resolverGuloso();

    // 2. Algoritmo Guloso Randomizado (Roda 'iteracoes' vezes com parâmetro alpha fixo)
    Solucao resolverRandomizado(float alpha, int iteracoes, unsigned int seed);

    // 3. Algoritmo Randomizado Reativo
    // Escolhe alphas de um pool, atualiza probabilidades a cada 'bloco' iterações
    Solucao resolverReativo(int iteracoes, int tamanhoBloco, vector<float> alphas, unsigned int seed);

    // Método auxiliar para salvar no CSV (conforme pedido no trabalho)
    void salvarEstatisticas(string arquivoCsv, string nomeInstancia, string algoritmo, 
                            string params, Solucao sol);
};

#endif