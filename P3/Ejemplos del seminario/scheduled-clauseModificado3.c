#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
  #include <omp.h>
#else
  #define omp_get_thread_num() 0
#endif

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
  int i, n=16,chunk,a[n],suma=0, printed = 0, chunk_read;
  omp_sched_t sched_type;
  if(argc < 2) {
    fprintf(stderr,"\nFalta chunk \n");
    exit(-1);
  }
  chunk = atoi(argv[1]);

  for (i=0; i<n; i++)
    a[i] = i;

  omp_get_schedule(&sched_type, &chunk_read);
  printf("\n\tAntes de la modificación:\n dyn var: %s\n nthreads-var: %d\n run-shed-var: %s --- %d\n",
    printBool(omp_get_dynamic()), omp_get_max_threads(), printEnum(sched_type), chunk_read);

  omp_set_dynamic(1);
  omp_set_num_threads(2);
  omp_set_schedule(omp_sched_auto, chunk);

  omp_get_schedule(&sched_type, &chunk_read);
  printf("\n\tDespués de la modificación:\n dyn var: %s\n nthreads-var: %d\n run-shed-var: %s --- %d\n\n",
    printBool(omp_get_dynamic()), omp_get_max_threads(), printEnum(sched_type), chunk_read);


  #pragma omp parallel for firstprivate(suma) lastprivate(suma) schedule(dynamic,chunk)
  for (i=0; i<n; i++)
  {
    suma = suma + a[i];
    printf(" thread %d suma a[%d]=%d suma=%d \n",
    omp_get_thread_num(),i,a[i],suma);
  }

  printf("Fuera de 'parallel for' suma=%d\n",suma);
}
