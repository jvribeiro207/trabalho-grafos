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
    vector<vector<int>> adj; 
    bool direcionado;

public:
    Grafo();
    ~Grafo();
    void inicializar(int n);
    void adicionarAresta(int u, int v);
    void lerArquivo(string nomeArquivo);
    int getNumVertices() const;
    int getNumArestas() const; 
    const vector<int>& getVizinhos(int u) const;
    bool saoAdjacentes(int u, int v);
    void imprimir();
};

void salvarSolucaoCSAcademy(const Grafo& G, const std::vector<int>& cores, std::string nomeArquivo);

#endif