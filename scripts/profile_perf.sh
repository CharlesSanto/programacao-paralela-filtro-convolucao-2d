#!/bin/bash

EXECUTABLE="./bin/baseline"
N=4000
P=30

if [ ! -f "$EXECUTABLE" ]; then
    echo "Executavel nao encontrado. Rode 'make' primeiro."
    exit 1
fi

echo "=== COLETA DE CONTADORES DE HARDWARE (PERF STAT) ==="
perf stat -e cycles,instructions,cache-references,cache-misses,L1-dcache-load-misses $EXECUTABLE $N $P