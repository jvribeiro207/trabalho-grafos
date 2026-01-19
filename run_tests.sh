#!/bin/bash
# run_table_data.sh

INSTANCIAS=("dsjc1000.1.col" "r250.5.col" "dsjc250.5.col") 


P=1
Q=0
ITERACOES=10

g++ main.cpp Grafo.cpp Guloso.cpp -o trabalho_grafos -O3

echo "Iniciando bateria de testes..."
rm -f resultados.csv 

for INSTANCIA in "${INSTANCIAS[@]}"
do
    echo "Processando $INSTANCIA..."
    for ((i=1; i<=ITERACOES; i++))
    do

        ./trabalho_grafos $INSTANCIA $P $Q
        sleep 1
    done
done
echo "Concluído!"