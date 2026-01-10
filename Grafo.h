#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

class Grafo {
private:
    int numVertices;
    int numArestas;
    // Lista de Adjacência: vetor de vetores onde adj[u] contém os vizinhos de u
    vector<vector<int>> adj; 
    
    // Variável para indicar se o grafo é direcionado (o problema pede não direcionado, mas é bom parametrizar)
    bool direcionado;

public:
    // Construtor
    Grafo();

    // Destrutor
    ~Grafo();

    // Inicializa o grafo com n vértices
    void inicializar(int n);

    // Adiciona aresta entre u e v
    void adicionarAresta(int u, int v);

    // Leitura do arquivo de instância (formato DIMACS padrão sugerido pelas bibliotecas citadas)
    void lerArquivo(string nomeArquivo);

    // Getters úteis para os algoritmos gulosos
    int getNumVertices();
    int getNumArestas();
    
    // Retorna a lista de vizinhos (distância 1) de um vértice u
    // Essencial para verificar a restrição |f(u) - f(v)| >= p 
    const vector<int>& getVizinhos(int u);

    // Método auxiliar para verificar se dois vértices são adjacentes
    bool saoAdjacentes(int u, int v);

    // Imprime o grafo (para depuração)
    void imprimir();
};

#endif