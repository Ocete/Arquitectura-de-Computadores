#include <stdlib.h>
#include <stdio.h>
#include<time.h>
#ifdef _OPENMP
  #include <omp.h>
#else
  #define omp_get_thread_num() 0
#endif

//#define PRINT_ALL
//#define VECTOR_GLOBAL
#define VECTOR_DYNAMIC

#ifdef VECTOR_GLOBAL
#define MAX 32768 //=2^10
double v[MAX], m[MAX][MAX], r[MAX];
#endif

omp_sched_t charToSchedType (char c) {
  omp_sched_t t;
  if (c == 'S')
    t = omp_sched_static;
  else if (c == 'D')
    t = omp_sched_dynamic;
  else if (c == 'G')
    t = omp_sched_guided;
  else if (c == 'A')
    t = omp_sched_auto;
  else {
    printf(" Error en tipo de schedule.\n"
      " Puede ser (S)tatic - (D)ynamic - (G)uided - (A)uto\n");
    exit(-1);
  }
  return t;
}

char* printEnum (omp_sched_t type) {
  char * ret;
  if (type == omp_sched_static)
    ret = "Static";
  else if (type == omp_sched_dynamic)
    ret = "Dynamic";
  else if (type == omp_sched_guided)
    ret = "Guided";
  else if (type == omp_sched_auto)
    ret = "Auto";
  return ret;
}

int main(int argc,char** argv){

  if  (argc<4){
    printf("Error en nº de parámetros. Ejecución:\n %s <Tipo de schedule> <chunk> <iteraciones>\n", argv[0]);
    printf("El schedule puede ser (S)tatic - (D)ynamic - (G)uided - (A)uto\n");
    printf("Introducir chunk = 0 para tomar el valor por defecto según el schedueling.\n");
    exit(-1);
  }

  omp_sched_t sched_type = charToSchedType(argv[1][0]);

  int chunk = atoi(argv[2]);
  if (chunk == 0) {
    if (sched_type == 'S' || sched_type == 'G')
      chunk = omp_get_num_threads();
    else
      chunk = 1;
  } else if (chunk < 0) {
    chunk = 1;
    printf("\n Valor de chunk negativo. Queda fijado a 1.\n");
  }

  unsigned int N = atoi(argv[3]);   // Máximo N =2^32  -1=4294967295 (sizeof(unsigned int) = 4 B)
  if (N < 1) {
    printf("Error - Número de iteraciones negativo.\n");
    exit(-1);
  }

  struct  timespec cgt1,cgt2;
  double  ncgt;   //para tiempo de ejecución
  int i, j;

  omp_set_schedule(sched_type, chunk);
/*
  // Comprobamos que hemos fijado bien el schedueling y el chunk
  omp_get_schedule(&sched_type, &chunk);
  printf("\n run-shed-var: Schedule %s --- Chunk  = %d\n",
    printEnum(sched_type), chunk);
*/

  #ifdef VECTOR_GLOBAL
  if (N>MAX)
    N=MAX;
    printf("\n Número de iteraciones refijado al máximo posible: %d\n", N);
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
  #pragma omp parallel for private(i)
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
  #pragma omp parallel for private(sum, i)
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
  printf("\n Tiempo de ejecución(s): %11.9f\n\n", ncgt);

  #ifdef VECTOR_DYNAMIC
  free(v);        // libera el espacio reservado para v
  free(m);        // libera el espacio reservado para m
  free(r);
  #endif
  return  0;
}
