#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define MAX 1048576 // = 2^20
double x[MAX], y[MAX], r[MAX];

int main(int argc, char ** argv)
{
  double ncgt, a = 1.1;
  struct timespec cgt1,cgt2;

  //Inicializar vector y matriz
  x[0] = y[0] = 2.5;
  for  (int i=1; i<MAX; i++) {
    y[i] = x[i] = -x[i-1];
  }

  // Calculamos el producto y medimos el tiempo
  clock_gettime(CLOCK_REALTIME,&cgt1);
  for (int i=1;i<=MAX;i++) r[i]= a*x[i] + y[i];
  clock_gettime(CLOCK_REALTIME,&cgt2);
  ncgt = (double) (cgt2.tv_sec - cgt1.tv_sec) +
       (double) ((cgt2.tv_nsec - cgt1.tv_nsec)/(1.e+9));

  //Mostramos los resultados por pantalla
  printf("Primer valor: %0.1f \t Último valor: %0.1f \n", r[0], r[MAX-1]);
  printf("\n Tiempo de ejecución(s): %11.9f\n", ncgt);
}
