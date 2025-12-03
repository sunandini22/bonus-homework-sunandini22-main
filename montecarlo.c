#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

double integrand(double *x, int dim);

double integrand(double *x, int dim)
{
 double sum = 1;
 for(int i = 0; i<dim; i++)
 {
  sum += x[i];
 }
return sum;
}

int main(int argc, char **argv)
{
 if(argc!=2)
 {
  printf("Mention the number of points\n");
  return 1;
 }
 long long num_points = atoll(argv[1]);
 int num_threads,dim = 10;
 double volume=pow(2,dim);
 double sum=0, result;

 double start = omp_get_wtime();

 #pragma omp parallel
 {
  unsigned int seed = omp_get_thread_num();
  num_threads = omp_get_num_threads();
  #pragma omp for reduction(+:sum)
  for(long long i = 0; i<num_points; i++)
  {
   double x[dim];
   for(int j = 0; j<dim; j++)
   {
    x[j] = (double)rand_r(&seed)/RAND_MAX*2-1;   
   }
   sum += integrand(x,dim);  
  } 
 }
 result = volume * sum/num_points;
 double end = omp_get_wtime();
 printf("Result: %.6e\n", result);
 printf("Elapsed time: %.6f\n", end - start);
 return 0;
}
