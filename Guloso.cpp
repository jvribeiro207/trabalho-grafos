#include "Guloso.h"

Guloso::Guloso(Grafo& g, int paramP, int paramQ) : grafo(g), p(paramP), q(paramQ) {}

bool Guloso::corValida(int u, int c, const vector<int>& coresAtuais) {
    // Restrição 1: Distância 1 (vizinhos diretos)
    // |f(u) - f(v)| >= p
    const vector<int>& vizinhos = grafo.getVizinhos(u);
    for (int v : vizinhos) {
        if (coresAtuais[v] != -1) { // Se vizinho já tem cor
            if (abs(c - coresAtuais[v]) < p) return false;
        }

        // Restrição 2: Distância 2 (vizinhos dos vizinhos)
        // |f(u) - f(w)| >= q
        // w é vizinho de v. Se w != u, dist(u, w) = 2.
        const vector<int>& vizinhosDeV = grafo.getVizinhos(v);
        for (int w : vizinhosDeV) {
            if (w == u) continue; // Ignora o próprio u
            if (coresAtuais[w] != -1) {
                if (abs(c - coresAtuais[w]) < q) return false;
            }
        }
    }
    return true;
}

Solucao Guloso::construirSolucao(float alpha, unsigned int seed) {
    // Configura semente local para esta construção (importante para reprodutibilidade interna)
    // Mas normalmente o srand é chamado fora. Aqui garantimos variação se for loop.
    // Para simplificar, assumimos que srand já foi chamado corretamente no loop principal.
    
    int n = grafo.getNumVertices();
    Solucao sol;
    sol.coloracao.assign(n + 1, -1);
    sol.maxCor = 0; // Cores >= 0

    // Lista de candidatos (vértices não coloridos)
    vector<int> candidatos;
    for (int i = 1; i <= n; i++) candidatos.push_back(i);

    // Heurística: Grau do vértice (estática)
    auto getGrau = [&](int v) {
        return (int)grafo.getVizinhos(v).size();
    };

    while (!candidatos.empty()) {
        int escolhido = -1;
        int idxEscolhido = -1;

        if (alpha < 0) {
            // --- GULOSO PURO ---
            // Escolhe sempre o de maior grau
            int maxGrau = -1;
            for (int i = 0; i < candidatos.size(); i++) {
                int g = getGrau(candidatos[i]);
                if (g > maxGrau) {
                    maxGrau = g;
                    escolhido = candidatos[i];
                    idxEscolhido = i;
                }
            }
        } else {
            // --- GULOSO RANDOMIZADO (RCL) ---
            // 1. Encontrar min e max grau na lista de candidatos
            int minG = 1e9, maxG = -1;
            for (int v : candidatos) {
                int g = getGrau(v);
                if (g < minG) minG = g;
                if (g > maxG) maxG = g;
            }

            // 2. Construir RCL (Restricted Candidate List)
            // Critério: Grau >= min + alpha * (max - min)
            // Se alpha = 1 -> Apenas os de grau MAX (Guloso)
            // Se alpha = 0 -> Qualquer um (Aleatório puro)
            // Threshold = Max - alpha * (Max - Min). 
            // Vértices com Grau >= Threshold entram na lista.
            
            float threshold = maxG - alpha * (maxG - minG);
            
            vector<int> rcl;
            for (int i = 0; i < candidatos.size(); i++) {
                if (getGrau(candidatos[i]) >= threshold) {
                    rcl.push_back(i); // Guarda o índice no vetor candidatos
                }
            }

            // 3. Escolher aleatoriamente da RCL
            if (!rcl.empty()) {
                int r = rand() % rcl.size();
                idxEscolhido = rcl[r];
                escolhido = candidatos[idxEscolhido];
            }
        }

        // Remove escolhido da lista de candidatos
        // Troca com o último e pop_back (O(1))
        candidatos[idxEscolhido] = candidatos.back();
        candidatos.pop_back();

        // Atribuir a menor cor válida
        int cor = 0; // Começando da cor 0 (ou 1, ajustável)
        while (true) {
            if (corValida(escolhido, cor, sol.coloracao)) {
                sol.coloracao[escolhido] = cor;
                if (cor > sol.maxCor) sol.maxCor = cor;
                break;
            }
            cor++;
        }
    }
    
    return sol;
}

Solucao Guloso::resolverGuloso() {
    auto inicio = chrono::high_resolution_clock::now();
    
    // Alpha -1 indica guloso puro
    Solucao sol = construirSolucao(-1.0, 0); 
    
    auto fim = chrono::high_resolution_clock::now();
    sol.tempoExecucao = chrono::duration_cast<chrono::duration<double>>(fim - inicio).count();
    
    return sol;
}

Solucao Guloso::resolverRandomizado(float alpha, int iteracoes, unsigned int seed) {
    auto inicioTotal = chrono::high_resolution_clock::now();
    
    Solucao melhorSol;
    melhorSol.maxCor = 1e9; // Infinito

    // O loop externo no main controla a semente global. 
    // Aqui assumimos que rand() gerará valores diferentes a cada chamada.
    
    for (int i = 0; i < iteracoes; i++) {
        Solucao atual = construirSolucao(alpha, seed);
        
        if (atual.maxCor < melhorSol.maxCor) {
            melhorSol = atual;
            melhorSol.semente = seed; // Registra a semente (simbólico se não reiniciar srand)
        }
    }
    
    auto fimTotal = chrono::high_resolution_clock::now();
    melhorSol.tempoExecucao = chrono::duration_cast<chrono::duration<double>>(fimTotal - inicioTotal).count();
    
    return melhorSol;
}

Solucao Guloso::resolverReativo(int iteracoes, int tamanhoBloco, vector<float> alphas, unsigned int seed) {
    auto inicioTotal = chrono::high_resolution_clock::now();

    int m = alphas.size();
    vector<double> probabilidades(m, 1.0 / m); // Probabilidade inicial uniforme
    vector<double> somaSolucoes(m, 0.0);       // Soma dos resultados para cada alpha
    vector<int> qtdVezes(m, 0);                // Quantas vezes cada alpha foi usado
    
    Solucao melhorGlobal;
    melhorGlobal.maxCor = 1e9;

    // Melhor solução conhecida para normalização (pode ser atualizada dinamicamente)
    int melhorConhecido = 1e9; 

    for (int iter = 0; iter < iteracoes; iter++) {
        
        // 1. Escolher alpha baseado nas probabilidades (Roleta)
        double r = (double)rand() / RAND_MAX;
        double somaP = 0;
        int idxAlpha = 0;
        for (int i = 0; i < m; i++) {
            somaP += probabilidades[i];
            if (r <= somaP) {
                idxAlpha = i;
                break;
            }
        }
        // Segurança contra erro de arredondamento
        if (idxAlpha >= m) idxAlpha = m - 1;

        // 2. Executar construtivo
        Solucao solAtual = construirSolucao(alphas[idxAlpha], seed);
        
        // Atualiza estatísticas
        somaSolucoes[idxAlpha] += solAtual.maxCor;
        qtdVezes[idxAlpha]++;
        
        if (solAtual.maxCor < melhorConhecido) {
            melhorConhecido = solAtual.maxCor;
        }
        if (solAtual.maxCor < melhorGlobal.maxCor) {
            melhorGlobal = solAtual;
        }

        // 3. Atualizar probabilidades ao fim do bloco
        if ((iter + 1) % tamanhoBloco == 0) {
            vector<double> qualidades(m, 0.0);
            double somaQ = 0.0;
            
            for (int i = 0; i < m; i++) {
                if (qtdVezes[i] > 0) {
                    double media = somaSolucoes[i] / qtdVezes[i];
                    // Qualidade = (Melhor / Média)^beta. 
                    // Como queremos MINIMIZAR, quanto menor a média, melhor.
                    // Q = MelhorGlobal / Media.
                    qualidades[i] = (double)melhorConhecido / media;
                } else {
                    // Se não foi usado, mantém probabilidade ou reduz? 
                    // Estratégia simples: qualidade média atual.
                    qualidades[i] = 0.01; // Valor baixo para penalizar não uso
                }
                somaQ += qualidades[i];
            }

            // Recalcula probabilidades
            for (int i = 0; i < m; i++) {
                probabilidades[i] = qualidades[i] / somaQ;
                // Opcional: impor probabilidade mínima para não zerar totalmente
            }
        }
    }

    auto fimTotal = chrono::high_resolution_clock::now();
    melhorGlobal.tempoExecucao = chrono::duration_cast<chrono::duration<double>>(fimTotal - inicioTotal).count();
    
    return melhorGlobal;
}

void Guloso::salvarEstatisticas(string arquivoCsv, string nomeInstancia, string algoritmo, 
                                string params, Solucao sol) {
    ofstream arquivo;
    arquivo.open(arquivoCsv, ios::app); // Append mode

    // Cabeçalho deve ser criado manualmente na primeira vez ou fora daqui
    // Formato sugerido: Data, Instancia, Algoritmo, Params, Semente, Tempo, MelhorSolucao
    
    // Pegando data atual
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", ltm);

    arquivo << buffer << ","
            << nomeInstancia << ","
            << algoritmo << ","
            << "\"" << params << "\"," // Aspas para proteger virgulas nos params
            << sol.semente << ","
            << fixed << setprecision(4) << sol.tempoExecucao << ","
            << sol.maxCor << endl;

    arquivo.close();
}