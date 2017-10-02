/* SumaVectores.c
  Suma de dos vectores:
  v3 = v1 + v2
  Para compilar usar (-lrt: real time library):
    gcc  -O2 SumaVectores.c -o SumaVectores –lrt
    gcc  -O2 –S SumaVectores.c –lrt  //para generar el código ensamblador
  Para ejecutar use: SumaVectoresC longitud
*/

#include <stdlib.h>
#include<stdio.h>
#include<time.h>

#define VECTOR_GLOBAL
#define MAX 67108864    //=2^26
double v1[MAX], v2[MAX], v3[MAX];

int main(int argc,char** argv){
  int  i;

  struct  timespec cgt1,cgt2;
  double  ncgt;   //para tiempo de ejecución
  //Leer argumento de entrada (nº de componentes del vector)

  if  (argc<2){
    printf("Faltan nº componentes del vector \n");
    exit(-1);
  }

  unsigned int N = atoi(argv[1]);   // Máximo N =2^32  -1=4294967295 (sizeof(unsigned int) = 4 B)
  if (N>MAX)
    N=MAX;

  //Inicializar vectores
  for  (i=0; i<N; i++){
    v1[i] =  N*0.1+  i*0.1; v2[i] = N*0.1  -  i*0.1;
    //los valores dependen de N
  }

  clock_gettime(CLOCK_REALTIME,&cgt1);
  //Calcular suma de vectores
  for  (i=0; i<N; i++)
    v3[i] = v1[i] + v2[i];

  clock_gettime(CLOCK_REALTIME,&cgt2);
  ncgt = (double) (cgt2.tv_sec - cgt1.tv_sec) +
       (double) ((cgt2.tv_nsec - cgt1.tv_nsec)/(1.e+9));

  //Imprimir resultado de la suma y el tiempo de ejecución

  printf("Tiempo(seg.):%11.9f \t/ Tamaño Vectores:%u \t/"
      "V1[0]+V2[0]=V3[0](%8.6f+%8.6f=%8.6f) / /"
      "V1[%d]+V2[%d]=V3[%d](%8.6f+%8.6f=%8.6f) / \n",
      ncgt, N, v1[0], v2[0], v3[0], N-1, N-1, N-1, v1[N-1], v2[N-1], v3[N-1]);

  return  0;
}
