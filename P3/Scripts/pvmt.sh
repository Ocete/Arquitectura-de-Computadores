#!/bin/bash
# Se asigna al trabajo el nombre SumaVectores
#PBS -N pvmt
# Se asigna al trabajo la cola ac
#PBS -q ac
# Se imprime información del trabajo usando variables de entorno de PBS
echo "Id. usuario del trabajo: $PBS_O_LOGNAME"
echo "Id. del trabajo: $PBS_JOBID"
echo "Nombre  del trabajo especificado por usuario: $PBS_JOBNAME"
echo "Nodo que ejecuta qsub: $PBS_O_HOST"
echo "Directorio en el que se ha ejecutado qsub: $PBS_O_WORKDIR"
echo "Cola: $PBS_QUEUE"
# Se ejecuta pvmt-OpenMP, que está en el directorio en el que se ha ejecutado qsub,
# para distintos valores de schedueling y chunk, con 15360 iteraciones.
echo "schedueling static -- Chunk por defecto -- 15360 iteraciones:" > salida.txt
./pvmt-OpenMP S 0 15360 >> salida.txt
echo "schedueling dynamic -- Chunk por defecto -- 15360 iteraciones:" >> salida.txt
./pvmt-OpenMP D 0 15360  >> salida.txt
echo "schedueling guided -- Chunk por defecto -- 15360 iteraciones:" >> salida.txt
./pvmt-OpenMP G 0 15360 >> salida.txt
echo "schedueling static -- Chunk 1 -- 15360 iteraciones:" >> salida.txt
./pvmt-OpenMP S 1 15360 >> salida.txt
echo "schedueling dynamic -- Chunk 1 -- 15360 iteraciones:" >> salida.txt
./pvmt-OpenMP D 1 15360 >> salida.txt
echo "schedueling guided -- Chunk 1 -- 15360 iteraciones:" >> salida.txt
./pvmt-OpenMP G 1 15360 >> salida.txt
echo "schedueling static -- Chunk 64 -- 15360 iteraciones:" >> salida.txt
./pvmt-OpenMP S 64 15360 >> salida.txt
echo "schedueling dynamic -- Chunk 64 -- 15360 iteraciones:" >> salida.txt
./pvmt-OpenMP D 64 15360 >> salida.txt
echo "schedueling guided -- Chunk 64 -- 15360 iteraciones:" >> salida.txt
./pvmt-OpenMP G 64 15360 >> salida.txt
echo "Fin del script - sin errores"
