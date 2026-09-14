#!/bin/bash

EXECUTABLE="./bin/baseline"
RUNS=10
PASSES=30
SIZES=(1000 2000 4000)

if [ ! -f "$EXECUTABLE" ]; then
    echo "Executavel nao encontrado. Rode 'make' primeiro."
    exit 1
fi

echo "=== INICIANDO BENCHMARK BASELINE SEQUENCIAL ==="

for N in "${SIZES[@]}"; do
    echo "----------------------------------------------"
    echo "Executando para N = $N | P = $PASSES ($RUNS repeticoes)"
    
    $EXECUTABLE $N $PASSES > /dev/null
    
    for i in $(seq 1 $RUNS); do
        $EXECUTABLE $N $PASSES
    done
done

echo "=============================================="