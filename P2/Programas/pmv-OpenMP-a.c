#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//#define PRINT_ALL
#define VECTOR_GLOBAL
//#define VECTOR_DYNAMIC

#ifdef VECTOR_GLOBAL
#define MAX 1073741824    //=2^10
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
  #pragma omp parallel for
  for  (i=0; i<N; i++) {
    v[i] = N*0.1+  i*0.1;
    for (j=0; j<N; j++)
      m[i][j] = v[i]*0.1+j*0.1;
  }

  //Comprobamos la incialización
  #ifdef PRINT_ALL
  printf(" Vector:\n");
  for  (i=0; i<N; i++) {
    printf("\t%f", v[i]);
  }

  printf("\n\n Matriz: \n");

  for  (i=0; i<N; i++) {
    for (j=0; j<N; j++)
      printf("\t%f", m[i][j]);
    printf("\n\n");
  }
  #endif

  clock_gettime(CLOCK_REALTIME,&cgt1);
  //Calcular el producto
  int sum;
  #pragma omp parallel for private(j, sum)
  for (i=0; i<N; i++) {
    sum = 0;
    for (j=0; j<N; j++)
      sum += m[i][j]*v[j];
    r[i] = sum;
  }

  clock_gettime(CLOCK_REALTIME,&cgt2);
  ncgt = (double) (cgt2.tv_sec - cgt1.tv_sec) +
       (double) ((cgt2.tv_nsec - cgt1.tv_nsec)/(1.e+9));

  //Imprimir resultado del producto
  printf("\n Resultado:\n");
  #ifdef  PRINT_ALL
  for  (i=0; i<N; i++) {
    printf("\t%f", r[i]);
  }
  printf("\n");
  #else
  printf("Primer valor: %f \t Último valor: %f \n", r[0], r[N-1]);
  #endif
  printf("\n Tiempo de ejecución(s): %11.9f\n", ncgt);

  #ifdef VECTOR_DYNAMIC
  free(v);        // libera el espacio reservado para v
  free(m);        // libera el espacio reservado para m
  free(r);
  #endif
  return  0;
}
