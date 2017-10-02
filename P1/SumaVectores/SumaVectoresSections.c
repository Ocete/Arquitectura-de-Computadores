/* SumaVectoresSection.c
  Suma de dos vectores utilizando la directiva sections:
  v3 = v1 + v2
  Para compilar usar (-lrt: real time library):
    gcc  -O2 SumaVectores.c -o SumaVectores –lrt
    gcc  -O2 –S SumaVectores.c –lrt  //para generar el código ensamblador
  Para ejecutar use: SumaVectoresC longitud
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

//#define PRINTF_ALL

#define VECTOR_GLOBAL
#define MAX 67108864    //=2^26
double v1[MAX], v2[MAX], v3[MAX];

int main(int argc,char** argv){

  int i, tope;
  double t_inicial, t_final;   //para tiempo de ejecución

  //Leer argumento de entrada (nº de componentes del vector)
  if (argc<2){
    printf("Faltan nº componentes del vector \n");
    exit(-1);
  }

  unsigned int N = atoi(argv[1]);   // Máximo N =2^32  -1=4294967295 (sizeof(unsigned int) = 4 B)
  if (N>MAX)
    N=MAX;

  //Inicializar vectores
  #pragma omp parallel sections
  {
    #pragma omp section
    {
      tope = N/4;
      for  (i=0; i<tope; i++) {
        v1[i] = N*0.1 + i*0.1;
        v2[i] = N*0.1 - i*0.1;
      }
    }
    #pragma omp section
    {
      tope = N/2;
      for  (i=N/4; i<tope; i++) {
        v1[i] = N*0.1 + i*0.1;
        v2[i] = N*0.1 - i*0.1;
      }
    }
    #pragma omp section
    {
      tope = 3*N/4;
      for  (i=N/2; i<tope; i++) {
        v1[i] = N*0.1 + i*0.1;
        v2[i] = N*0.1 - i*0.1;
      }
    }
    #pragma omp section
    {
      tope = N;
      for  (i=3*N/4; i<tope; i++) {
        v1[i] = N*0.1 + i*0.1;
        v2[i] = N*0.1 - i*0.1;
      }
    }
  }

  t_inicial = omp_get_wtime();

  //Calcular suma de vectores
  #pragma omp parallel sections
  {
    #pragma omp section
    {
      tope = N/4;
      for  (i=0; i<tope; i++) {
        v3[i] = v1[i] + v2[i];
      }
    }
    #pragma omp section
    {
      tope = N/2;
      for  (i=N/4; i<tope; i++) {
        v3[i] = v1[i] + v2[i];
      }
    }
    #pragma omp section
    {
      tope = 3*N/4;
      for  (i=N/2; i<tope; i++) {
        v3[i] = v1[i] + v2[i];
      }
    }
    #pragma omp section
    {
      tope = N;
      for  (i=3*N/4; i<tope; i++) {
        v3[i] = v1[i] + v2[i];
      }
    }
  }


  t_final = omp_get_wtime() - t_inicial;

  //Imprimir resultado de la suma y el tiempo de ejecución
  #ifdef PRINTF_ALL
    printf("Tiempo(seg.):%11.9f \t/ Tamaño Vectores:%u \t/", t_final, N);
    for (i=0; i<N; i++)
      printf("v3[%d] = %11.9f\n", i, v3[i]);
  #else
    printf("Tiempo(seg.):%11.9f \t/ Tamaño Vectores:%u \t/"
        "V1[0]+V2[0]=V3[0](%8.6f+%8.6f=%8.6f) / /"
        "V1[%d]+V2[%d]=V3[%d](%8.6f+%8.6f=%8.6f) / \n",
        t_final, N, v1[0], v2[0], v3[0], N-1, N-1, N-1, v1[N-1], v2[N-1], v3[N-1]);
  #endif

  return  0;
}
