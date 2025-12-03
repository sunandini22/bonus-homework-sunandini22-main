#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

double integrand(double *x, int dim);

double integrand(double *x, int dim)
{
 double sum = 0;
 for(int i = 0; i<dim - 1; i++)
 {
  double diff1 = 1 - x[i];
  double diff2 = x[i+1] - x[i]*x[i];
  sum += diff1 * diff1 + 100 * diff2 * diff2;
 }
return exp(-sum);
}

int main(int argc, char **argv)
{
 if(argc!=2)
 {
  printf("Mention the number of points\n");
  return 1;
 }
 long long num_points = strtol(argv[1], NULL, 10);;
 int num_threads, dim = 10;
 double volume = pow(2,dim);
 double sum = 0, result;

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
