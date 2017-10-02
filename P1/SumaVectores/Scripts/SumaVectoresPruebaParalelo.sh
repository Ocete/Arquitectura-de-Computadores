#!/bin/bash
#Se asigna al trabajo el nombre SumaVectoresPruebaParalelo
#PBS -N SumaVectoresPruebaParalelo
#Se asigna al trabajo la cola ac
#PBS -q ac
#Se imprime información del trabajo usando variables de entorno de PBS
echo "Id. usuario del trabajo: $PBS_O_LOGNAME"
echo "Id. del trabajo: $PBS_JOBID"
echo "Nombre  del trabajo especificado por usuario: $PBS_JOBNAME"
echo "Nodo que ejecuta qsub: $PBS_O_HOST"
echo "Directorio en el que se ha ejecutado qsub: $PBS_O_WORKDIR"
echo "Cola: $PBS_QUEUE"
echo "Nodos asignados al trabajo:"
#cat $PBS_NODEFILE
#Se ejecuta SumaVectorParalel,que está en el directorio en el que se ha ejecutado qsub
echo "Para N = 10:"
	./SumaVectoresParalelo 8
echo "Para N = 10M:"
	./SumaVectoresParalelo 11
