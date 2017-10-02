#!/bin/bash
#Se ejecuta SumaVectores, para Npotencia de 2 desde 2^16 a 2^26
for ((N=65536;N<67108865;N=N*2))
do
  echo "Componentes del vector: $N"
  time ./../SumaVectores $N >> tmp
done
rm tmp
