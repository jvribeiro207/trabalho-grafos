#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // Para formatar o nome do arquivo (opcional)
#include "Grafo.h"
#include "Guloso.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "Uso: ./programa <instancia> <p> <q> [semente]" << endl;
        return 1;
    }

    string nomeArquivo = argv[1];
    int p = atoi(argv[2]);
    int q = atoi(argv[3]);
    
    unsigned int semente = time(NULL);
    if (argc > 4) semente = (unsigned int)atoi(argv[4]);
    
    // Inicializa random
    srand(semente);

    // Carrega Grafo
    Grafo g;
    g.lerArquivo(nomeArquivo);

    // Inicializa Solver
    Guloso solver(g, p, q);
    string arquivoSaida = "resultados.csv";

    // --- 1. GULOSO ---
    cout << "Executando Guloso..." << endl;
    Solucao solG = solver.resolverGuloso();
    solG.semente = semente; 
    cout << "Guloso: Melhor Cor = " << solG.maxCor << " em " << solG.tempoExecucao << "s" << endl;
    
    solver.salvarEstatisticas(arquivoSaida, nomeArquivo, "Guloso", "-", solG);
    
    // [NOVO] Salvar visualização do Guloso
    salvarSolucaoCSAcademy(g, solG.coloracao, "vis_guloso.txt"); // <--- CHAMADA AQUI


    // --- 2. RANDOMIZADO (Testando com alpha 0.1, 0.3, 0.5) ---
    vector<float> alphasTeste = {0.1f, 0.3f, 0.5f};
    int numIter = 100; 

    for (float alpha : alphasTeste) {
        cout << "Executando Randomizado (alpha=" << alpha << ")..." << endl;
        
        Solucao solR = solver.resolverRandomizado(alpha, numIter, semente);
        
        string params = "alpha=" + to_string(alpha) + ", iter=" + to_string(numIter);
        cout << "Randomizado (a=" << alpha << "): Melhor Cor = " << solR.maxCor << endl;
        solver.salvarEstatisticas(arquivoSaida, nomeArquivo, "Randomizado", params, solR);

        // [NOVO] Salvar visualização para cada alpha
        // Cria um nome de arquivo dinâmico, ex: vis_rand_0.300000.txt
        string nomeVis = "vis_rand_" + to_string(alpha) + ".txt";
        salvarSolucaoCSAcademy(g, solR.coloracao, nomeVis); // <--- CHAMADA AQUI
    }


    // --- 3. REATIVO ---
    cout << "Executando Reativo..." << endl;
    vector<float> alphasReativo = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f};
    int numIterReativo = 300; 
    int bloco = 50;           
    
    Solucao solReact = solver.resolverReativo(numIterReativo, bloco, alphasReativo, semente);
    
    string paramsReact = "iter=" + to_string(numIterReativo) + ", bloco=" + to_string(bloco);
    cout << "Reativo: Melhor Cor = " << solReact.maxCor << endl;
    solver.salvarEstatisticas(arquivoSaida, nomeArquivo, "Reativo", paramsReact, solReact);

    // [NOVO] Salvar visualização do Reativo
    salvarSolucaoCSAcademy(g, solReact.coloracao, "vis_reativo.txt"); // <--- CHAMADA AQUI

    cout << "Fim. Resultados salvos em " << arquivoSaida << endl;

    return 0;
}