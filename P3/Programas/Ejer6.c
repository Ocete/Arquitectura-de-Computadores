#include <stdlib.h>
#include <stdio.h>
#include<time.h>

//#define PRINT_ALL
//#define VECTOR_GLOBAL
#define VECTOR_DYNAMIC

#ifdef VECTOR_GLOBAL
#define MAX 1048 //=2^10
double v[MAX], m[MAX][MAX], r[MAX];
#endif

int main(int argc,char** argv){

  if  (argc<2){
    printf("Faltan nº componentes del vector \n");
    exit(-1);
  }


  struct  timespec cgt1,cgt2;
  double  ncgt;   //para tiempo de ejecución
  int i, j;
  unsigned int N = atoi(argv[1]);   // Máximo N =2^32  -1=4294967295 (sizeof(unsigned int) = 4 B)

  #ifdef VECTOR_GLOBAL
  if (N>MAX)
    N=MAX;
  #endif


  #ifdef VECTOR_DYNAMIC
  double *v, **m, *r;
  v = (double*) malloc(N*sizeof(double));  // malloc necesita el tamaño en bytes
  m = (double**) malloc(N*sizeof(double*));  //si no hay espacio suficiente malloc devuelve NULL
  for (i=0; i<N; i++)
    m[i] = (double*) malloc(N*sizeof(double));
  r = (double*) malloc(N*sizeof(double));
  if  ((v==NULL) || (m==NULL) || (r==NULL)) {
    printf("Error en la reserva de espacio para los vectores\n");
    exit(-2);
  }
  #endif

  //Inicializar vector y matriz
  for  (j=0; j<N; j++) {
    v[j] = 2.5;
    m[0][j] = 1.1;
    for (i=1; i<=j; i++)
      m[i][j] = -m[i-1][j];
    for (; i<N; i++)
      m[i][j] = 0;
  }

  //Comprobamos la incialización
  #ifdef PRINT_ALL
  printf("\n Vector:\n");
  for  (i=0; i<N; i++) {
    printf("\t%0.1f", v[i]);
  }

  printf("\n\n Matriz: \n");

  for  (i=0; i<N; i++) {
    for (j=0; j<N; j++)
      printf("\t%0.1f", m[i][j]);
    printf("\n\n");
  }
  #endif

  clock_gettime(CLOCK_REALTIME,&cgt1);
  //Calcular el producto
  double sum;
  for  (j=0; j<N; j++) {
    sum = 0;
    for (i=0; i<=j; i++)
      sum += v[i]*m[i][j];
    r[j] = sum;
  }

  clock_gettime(CLOCK_REALTIME,&cgt2);
  ncgt = (double) (cgt2.tv_sec - cgt1.tv_sec) +
       (double) ((cgt2.tv_nsec - cgt1.tv_nsec)/(1.e+9));

  //Imprimir resultado del producto
  printf("\n Resultado:\n");
  #ifdef  PRINT_ALL
  for  (i=0; i<N; i++) {
    printf("\t%0.2f", r[i]);
  }
  printf("\n");
  #else
  printf("Primer valor: %0.1f \t Último valor: %0.1f \n", r[0], r[N-1]);
  #endif
  printf("\n Tiempo de ejecución(s): %11.9f\n", ncgt);

  #ifdef VECTOR_DYNAMIC
  free(v);        // libera el espacio reservado para v
  free(m);        // libera el espacio reservado para m
  free(r);
  #endif
  return  0;
}
