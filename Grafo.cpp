#include "Grafo.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

Grafo::Grafo() {
    this->numVertices = 0;
    this->numArestas = 0;
    this->direcionado = false; 
}

Grafo::~Grafo() {
}

void Grafo::inicializar(int n) {
    this->numVertices = n;
    // Assume-se índices 0 a n (ajustado para n+1 por segurança)
    this->adj.assign(n + 1, vector<int>());
}

void Grafo::adicionarAresta(int u, int v) {
    bool existe = false;
    for(int vizinho : adj[u]) {
        if (vizinho == v) {
            existe = true;
            break;
        }
    }

    if (!existe) {
        adj[u].push_back(v);
        numArestas++;
        if (!direcionado) {
            adj[v].push_back(u); 
        }
    }
}

void Grafo::lerArquivo(string nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        exit(1);
    }

    string linha, token;
    while (getline(arquivo, linha)) {
        if (linha.empty()) continue;
        stringstream ss(linha);
        ss >> token;

        if (token == "c") continue;
        else if (token == "p") {
            string formato;
            int n, m;
            ss >> formato >> n >> m;
            inicializar(n);
            cout << "Lendo grafo com " << n << " vértices e " << m << " arestas." << endl;
        }
        else if (token == "e") {
            int u, v;
            ss >> u >> v;
            adicionarAresta(u, v);
        }
    }
    arquivo.close();
}

// --- CORREÇÃO: Adicionado const ---
int Grafo::getNumVertices() const {
    return numVertices;
}

// --- CORREÇÃO: Adicionado const ---
int Grafo::getNumArestas() const {
    return numArestas;
}

// --- CORREÇÃO: Adicionado const ---
const vector<int>& Grafo::getVizinhos(int u) const {
    if (u >= 0 && u < adj.size()) {
        return adj[u];
    }
    static vector<int> vazio;
    return vazio;
}

bool Grafo::saoAdjacentes(int u, int v) {
    // Aqui precisamos usar 'const_cast' ou chamar a versão const, 
    // mas como getVizinhos já retorna const ref, funciona direto.
    const vector<int>& vizinhos = getVizinhos(u);
    for (int vizinho : vizinhos) {
        if (vizinho == v) return true;
    }
    return false;
}

void Grafo::imprimir() {
    cout << "Representação do Grafo (Lista de Adjacência):" << endl;
    for (int i = 1; i <= numVertices; i++) {
        cout << i << " -> ";
        for (int v : adj[i]) {
            cout << v << " ";
        }
        cout << endl;
    }
}

// --- CORREÇÃO COMPLETA DA FUNÇÃO ---
void salvarSolucaoCSAcademy(const Grafo& G, const std::vector<int>& cores, std::string nomeArquivo) {
    std::ofstream arquivo(nomeArquivo);
    
    if (!arquivo.is_open()) {
        std::cout << "Erro ao criar arquivo de visualizacao: " << nomeArquivo << std::endl;
        return;
    }

    // Percorre todos os vértices
    // Nota: Se seus vértices no arquivo começam em 1, altere o loop para: for (int u = 1; ...
    // Se começam em 0, mantenha u = 0.
    for (int u = 1; u <= G.getNumVertices(); u++) {
        
        // CORREÇÃO: Usando o método público getVizinhos() em vez de listaAdj
        const std::vector<int>& vizinhos = G.getVizinhos(u);

        // Se o vértice não tem vizinhos, imprimimos ele sozinho
        if (vizinhos.empty()) {
            // Verifica se o índice u existe no vetor cores (segurança)
            if(u < cores.size())
                arquivo << u << "(" << cores[u] << ")" << std::endl;
            continue;
        }

        for (int v : vizinhos) {
            // Imprime apenas se u < v para não duplicar arestas
            if (u < v) {
                // Segurança de acesso ao vetor cores
                if(u < cores.size() && v < cores.size()) {
                    arquivo << u << "(" << cores[u] << ") " 
                            << v << "(" << cores[v] << ")" << std::endl;
                }
            }
        }
    }

    arquivo.close();
    // std::cout << "Visualizacao salva em: " << nomeArquivo << std::endl;
}