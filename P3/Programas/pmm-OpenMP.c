#include <stdlib.h>
#include <stdio.h>
#include<time.h>

#define PRINT_ALL
//#define VECTOR_GLOBAL
#define VECTOR_DYNAMIC

#ifdef VECTOR_GLOBAL
#define MAX 32768 //=2^10
double m1[MAX][MAX], m2[MAX][MAX], r[MAX][MAX];
#endif

int main(int argc,char** argv){

  if  (argc<2){
    printf("Faltan nº componentes del vector \n");
    exit(-1);
  }


  struct  timespec cgt1,cgt2;
  double  ncgt;   //para tiempo de ejecución
  int i, j, k;
  unsigned int N = atoi(argv[1]);   // Máximo N =2^32  -1=4294967295 (sizeof(unsigned int) = 4 B)

  #ifdef VECTOR_GLOBAL
  if (N>MAX)
    N=MAX;
  #endif


  #ifdef VECTOR_DYNAMIC
  double **m1, **m2, **r;
  m1 = (double**) malloc(N*sizeof(double*));  // malloc necesita el tamaño en bytes
  for (i=0; i<N; i++)
    m1[i] = (double*) malloc(N*sizeof(double));
  m2 = (double**) malloc(N*sizeof(double*));  //si no hay espacio suficiente malloc devuelve NULL
  for (i=0; i<N; i++)
    m2[i] = (double*) malloc(N*sizeof(double));
  r = (double**) malloc(N*sizeof(double*));
  for (i=0; i<N; i++)
    r[i] = (double*) malloc(N*sizeof(double));
  if  ((m1==NULL) || (m2==NULL) || (r==NULL)) {
    printf("Error en la reserva de espacio para los vectores\n");
    exit(-2);
  }
  #endif

  // Inicializar matrices
  #pragma omp parallel for private(j)
  for (i=0; i<N; i++) {
    m1[i][0] = 1.1;
    m2[i][0] = 1.1;
    for (j=1; j<N; j++) {
      m1[i][j] = - m1[i][j-1];
      m2[i][j] = - m2[i][j-1];
    }
  }

  //Comprobamos la incialización
  #ifdef PRINT_ALL
  printf("\n\n Matriz 1: \n");
  for  (i=0; i<N; i++) {
    for (j=0; j<N; j++)
      printf("\t%0.1f", m1[i][j]);
    printf("\n\n");
  }
  printf("\n\n Matriz 2: \n");
  for  (i=0; i<N; i++) {
    for (j=0; j<N; j++)
      printf("\t%0.1f", m2[i][j]);
    printf("\n\n");
  }
  #endif

  clock_gettime(CLOCK_REALTIME,&cgt1);
  //Calcular el producto
  double sum;

  for (i=0; i<N; i++) {
    #pragma omp parallel for private(j,k,sum)
    for (j=0; j<N; j++) {
      sum = 0;
      for (k=0; k<N; k++) {
        sum += m1[i][k] * m2[k][j];
      }
      r[i][j] = sum;
    }
  }

  clock_gettime(CLOCK_REALTIME,&cgt2);
  ncgt = (double) (cgt2.tv_sec - cgt1.tv_sec) +
       (double) ((cgt2.tv_nsec - cgt1.tv_nsec)/(1.e+9));

  //Imprimir resultado del producto
  printf("\n Resultado:\n");
  #ifdef  PRINT_ALL
  for  (i=0; i<N; i++) {
    for (j=0; j<N; j++)
      printf("\t%0.2f", r[i][j]);
    printf("\n\n");
  }
  printf("\n");
  #else
  printf("Primer valor: %0.2f \t Último valor: %0.2f \n", r[0][0], r[N-1][N-1]);
  #endif
  printf("\n Tiempo de ejecución(s): %11.9f\n", ncgt);

  #ifdef VECTOR_DYNAMIC
  for (i=0; i<N; i++)
    free(m1[i]);
  free(m1);
  for (i=0; i<N; i++)
    free(m2[i]);
  free(m2);
  for (i=0; i<N; i++)
    free(r[i]);
  free(r);;
  #endif
  return  0;
}
