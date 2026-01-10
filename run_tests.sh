#!/bin/bash
# run_table_data.sh

# Lista de grafos que você escolheu para o trabalho (pelo menos uns 5 ou 10)
INSTANCIAS=("flat300_28_0.col" "r250.5.col" "dsjc250.5.col") 

# Parâmetros do problema (L(2,1) ou L(1,0) conforme seu objetivo agora)
P=1
Q=0
ITERACOES=10

g++ main.cpp Grafo.cpp Guloso.cpp -o trabalho_grafos -O3

echo "Iniciando bateria de testes..."
rm -f resultados.csv # Limpa arquivo anterior para não misturar

for INSTANCIA in "${INSTANCIAS[@]}"
do
    echo "Processando $INSTANCIA..."
    for ((i=1; i<=ITERACOES; i++))
    do
        # O programa C++ já roda Guloso, Randomizado (vários alphas) e Reativo de uma vez
        ./trabalho_grafos $INSTANCIA $P $Q
        sleep 1
    done
done
echo "Concluído!"