#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
  #include <omp.h>
#else
  #define omp_get_thread_num() 0
#endif

char* printBool (int b) {
  char * ret;
  if (b == 0)
    ret = "False";
  else if (b == 1)
    ret = "True";
  else
    ret = "Error en printBool";
  return ret;
}

int main(int argc, char **argv) {
  int i, n=16,chunk,a[n],suma=0, printed = 0;
  if(argc < 2) {
    fprintf(stderr,"\nFalta chunk \n");
    exit(-1);
  }
  chunk = atoi(argv[1]);

  for (i=0; i<n; i++)
    a[i] = i;
  printf("\n\tFuera del bucle:\n Num threads: %d\n Num procs: %d\n In parallel: %s\n",
    omp_get_num_threads(), omp_get_num_procs(), printBool(omp_in_parallel()));

  #pragma omp parallel for firstprivate(suma) lastprivate(suma) schedule(dynamic,chunk)
  for (i=0; i<n; i++)
  {
    #pragma omp critical
    {
    if (printed == 0) {
      printed = 1;
      printf("\n\tDentro del bucle:\n Num threads: %d\n Num procs: %d\n In parallel: %s\n\n",
        omp_get_num_threads(), omp_get_num_procs(), printBool(omp_in_parallel()));
    }
    }
    suma = suma + a[i];
    printf(" thread %d suma a[%d]=%d suma=%d \n",
    omp_get_thread_num(),i,a[i],suma);
  }

  printf("Fuera de 'parallel for' suma=%d\n",suma);
}
