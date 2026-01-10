#include "Grafo.h"
#include <sstream>

Grafo::Grafo() {
    this->numVertices = 0;
    this->numArestas = 0;
    this->direcionado = false; // Problema L(p,q) especifica grafo não direcionado [cite: 71]
}

Grafo::~Grafo() {
    // Vetores são limpos automaticamente pelo destrutor do C++
}

void Grafo::inicializar(int n) {
    this->numVertices = n;
    // Redimensiona o vetor de adjacência para n + 1 (para suportar índices começando em 1, se necessário)
    // Se as instâncias forem 1-based, usaremos índices 1 a n. Se 0-based, 0 a n-1.
    // O código abaixo assume que ajustaremos para 0-based internamente ou alocaremos extra.
    this->adj.assign(n + 1, vector<int>());
}

void Grafo::adicionarAresta(int u, int v) {
    // Evita duplicatas se o arquivo for mal formado
    // Nota: Em implementações de alta performance, pode-se remover esta verificação se a entrada for garantida.
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
            adj[v].push_back(u); // Grafo não direcionado [cite: 71]
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

        // Formato DIMACS comum
        if (token == "c") {
            continue; // Comentário
        }
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
            // Ajuste de índice se necessário (muitos arquivos DIMACS são 1-based)
            // Vamos assumir que armazenamos como lido, mas ao acessar vetor usamos índice correto.
            // Aqui assumirei conversão para 0-based se o arquivo for 1-based, 
            // mas a lógica depende da instância específica do site.
            // Por segurança, vou manter os valores originais e alocar n+1 no inicializar.
            adicionarAresta(u, v);
        }
    }

    arquivo.close();
}

int Grafo::getNumVertices() {
    return numVertices;
}

int Grafo::getNumArestas() {
    return numArestas; // Retorna o total de arestas adicionadas (cuidado: em não direcionado, addAresta conta 1 vez lógica)
}

const vector<int>& Grafo::getVizinhos(int u) {
    if (u >= 0 && u < adj.size()) {
        return adj[u];
    }
    static vector<int> vazio;
    return vazio;
}

bool Grafo::saoAdjacentes(int u, int v) {
    const vector<int>& vizinhos = getVizinhos(u);
    for (int vizinho : vizinhos) {
        if (vizinho == v) return true;
    }
    return false;
}

void Grafo::imprimir() {
    cout << "Representação do Grafo (Lista de Adjacência):" << endl;
    for (int i = 1; i <= numVertices; i++) { // Assumindo vértices de 1 a N
        cout << i << " -> ";
        for (int v : adj[i]) {
            cout << v << " ";
        }
        cout << endl;
    }
}