#!/bin/bash
#Se ejecuta SumaVectores, para Npotencia de 2 desde 2^14 a 2^26
echo "Start file:" > ValoresGraficaSecuencial.txt
for ((N=16384;N<67108865;N=N*2))
do
   ./../SumaVectores $N >> ValoresGraficaSecuencial.txt
done
