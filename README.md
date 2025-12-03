[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/EZhVTj8z)
# DSC 520: Bonus homework

**Points**: 50 bouns points

**Due date**: End of the day, Sunday, April 30

This is an extra bonus assignment worth 50 points. In order to get the points, you must work on your own, and, as always, submit your own work.

## Background: 10D Monte Carlo integration

In homework 1 you found that a Monte Carlo integral works better for a large dimensional parameter space. Now you'll put that to the test by performing an integral over 10 dimensions. In the first part, you'll test your code out on a simple function. In the second part you'll evaluate an integral for which the solution can only be solved numerically. To accomplish this, you will write parallelized code.

Recall (from lab 5) that any integral of a (potentially multivariate) function can be approximated by Monte Carlo integration

```math
\int_R f(\mathbf{x}) \mathrm{d}\mathbf{x} \approx V \frac{1}{N} \sum_{i=1}^N f(\mathbf{x}_i),
```

where the points $[\mathbf{x}\_{0}, ..., \mathbf{x}\_{N}]$ are random uniform draws from the "volume"

```math
V = \int_R \mathrm{d}\mathbf{x}. 
```

For example, if the parameter space is two dimensional so that $\mathbf{x} = \\{x_1,x_2\\}$ and the limits of the integral are $x_{i} \in [a, b)$ in both dimensions, then $V$ will be the area of a square: $(b-a)^2$. Likewise, if the parameter space is $D$ dimenionsal, so that $\mathbf{x} = \\{x_1, ..., x_{D}\\}$, and the limits are $x_{i} \in [a, b)$ in all dimensions, then $V$ will be the volume of a $D$ dimensional box $(b-a)^D$.

Here you will integrate two functions over the 10 dimensional box spanned by $x_{i} \in [-1, 1)$ for all $i \in [1, 10]$.

## Part 1: Simple test function (20 bonus points)

1. Create a program called `montecarlo` to use Monte Carlo integration to solve:

```math
I = \int_R f(\mathbf{x}) \mathrm{d}^{10}x
```

where the region R is a 10 dimensional box defined by:

```math
x_1, x_2, x_3, x_4, x_5, x_6, x_7, x_8, x_9, x_{10} \in [-1, 1)
```

and the function is given by:

```math
f(\mathbf{x}) = 1 + x_1 + x_2 + x_3 + x_4 + x_5 + x_6 + x_7 + x_8 + x_9 + x_{10}
```

To get full credit, your code **must**:

 * Be written in C, Cython, or Python.
 * Be parallelized either using OpenMP or MPI.
 * Take as command line arguments the number of points to use.
 * Prints the result of the integral to screen using scientific notation, not floating point. You need to do this for the next part, which will be a small number.
 * Uses a function to evaluate the integrand $f(\mathbf{x})$. This is so that you can easily swap out the function in the next part.
 * Also prints the time it takes to solve the integral to screen.
 * Runs faster for a large (say 1000000) number of points as you increase the number of cores from 1 to 8.

For example, if you write your code in C, it should look something like:

```C
// include statements

// declaration of the integration function
double integrand(double *x, int dim);

// main function
int main(int argc, char **argv){
  // SOME CODE TO PARSE THE COMMAND LINE
  // start the timer
  double start = omp_get_wtime();
  // PARALLEL CODE TO EXECUTE THE MONTE CARLO INTEGRAL,
  // WHICH WILL CALL THE integrand FUNCTION
  double end = omp_get_wtime();
  // PRINT RESULT, ELAPSED TIME TO SCREEN
  return 0;
}
 
double integrand(double *x, int dim){
  // CODE TO RETURN f(x)
 }
```

Running your code should look something like:

```bash
$ ./montecarlo 1000000
Result: 1.025103e+03
Elapsed time: 0.225009
```

2. The expected value of the integral is $2^{10} = 1024$. Check that your code approaches this value with increasing number of samples.

3. Test your code with differing number of cores (either threads or processes) on Stampede2 for a large number of samples (say 1000000). You should find that the execution time gets faster as you increase from the number of cores from 1, 2, 4, and 8 cores. If you are using multithreading (not MPI) then you may want to add the number of threads to use as a command-line argument, to make it easier to call your code. Otherwise, with MPI, the number of cores to use is based on the arguments you give to `mpiexec`.

4. Save the output of your code from running with 1, 2, 4, and 8 cores to a file called `montecarlo.out`.

5. Add/commit/push your source code (if you are using C, you do not need to push the binary) to GitHub, along with `montecarlo.out`.

**Tips:**
 * Monte carlo integrals require you to draw random samples, in this case in the range $[-1, 1)$. **You have to be careful doing this in a parallel environment!** Random number generation involves using a pseudo-random number generator (PRNG). These use a starting seed value to produce random numbers. Although the sequence of numbers produced is random, if the same seed is given, then the same sequence of numbers will be produced. These means that to parallelize random number generation you need to either:
    * Have a single, global PRNG that produces random numbers for all threads. This is the safest thing to do, but leads to slow code, since the random number generation portion (which needs to occur for every sample point) is serial.
    * **OR** Have separate PRNGs for each thread / process, **each with their own starting seed**. To accomplish this in C, look at the [rand_r](https://www.ibm.com/docs/en/zos/2.2.0?topic=functions-rand-r-pseudo-random-number-generator) function. In Python, look at the [numpy.random](https://numpy.org/doc/stable/reference/random/index.html) module, and [SeedSequencing](https://numpy.org/doc/stable/reference/random/parallel.html#seedsequence-spawn) in particular.
 * You can print the result in scientific notation by putting `%e` instead of `%f` in the string printed.
 * If you are working in C with OpenMP, you can use `omp_get_wtime` to time how long it takes the integral to evaluate for.
 * Be mindful of floating-point errors. Some values of $\mathbf{x}_i$ will result in $f(\mathbf{x}_i)$ being very close to zero. If you multiply the individual $f(\mathbf{x}_i)$ by a large number (like $V$) you will end up adding/subtracting large numbers when you compute the sum. Numercial error can then lead to what's known as [catastrophic cancellation](https://en.wikipedia.org/wiki/Catastrophic_cancellation). Better to compute the sum $\sum_i f(\mathbf{x}_i)$ *first*, then multiply by $V$.
 
## Part 2: Application to a more difficult expression (30 points)

Now that you have a working parallelized Monte Carlo integrator, you will apply it to a function which can only be integrated numerically.

1. Copy your source code, renaming it `montecarlo2[.c|.pyx.py]` where `[.c|.pyx|.py]` is the extension if you used C, Cython, or Python, resepectively.

2. In your copied code, change the definition of the `integrand` function so that it evalutes the function:

```math
f(\mathbf{x}) = \exp\left(-\sum_{i=1}^{9}\left[(1-x_i)^2 + 100\left(x_{i+1}-x_i^2\right)^2\right]\right)
```

This function has no analytic solution to the integral over `[-1, 1]`. The only way to solve it is numerically, as you are about to do.

Since you should have written your code such that the evaluation of the integrand is a stand-alone function, you should only have to change the integrand function, not the code to compute the integral itself.

3. In order to evaluate this integral accurately, you will need to use between 100 million and 100 billion (100,000,000,000) points in your integral. In order to parse this many points, you will need to be careful about what data type you use for `N` and what data type you use for the summation index in your code. For example, if you are coding in C, you cannot use `int` for `N` nor the summation index, and you cannot use the `atoi` function to parse the command line. This is because the largest (signed) integer you can store in C is 2147483647. To exceed this number (as you will need to evaluate 10 billion and 100 billion points) in C you'll need to use the [long](https://en.cppreference.com/w/c/types/limits) data type for `N` and the summation index. (If you are using pure Python -- i.e., no Cython -- you won't need to worry about this, but will need to worry about other issues, like the GIL.) To read a large number from the command line and store it as `long` in C, you'll want to use the [strtol](https://en.cppreference.com/w/c/string/byte/strtol) function instead of `atoi`.

Make sure your code can correctly parse large numbers from the command line. You can do this by printing off what `N` is (to print a long integer use `%ld` in the `printf` string instead of `%d`).

4. Once you are confident your code is working it's time to actually do the analysis! You will need to use sbatch on Stampede with 64 cores. Write an sbatch script call `mc10d.job` that will:
  * Request 1 node and 64 cores.
  * Run for a maximum of 2 hours (`#SBATCH -t 02:00:00`).
  * Write the output of the job to `mc10d.out`.
  * Write any errors from the job to `mc10d.err`.
  * Email you when it is finished.
  * Run your `montecarlo2` executable 4 times, starting with 100 million points, and increasing to 100 billion. In other words, the script should run:
  
```bash
./montecarlo2 100000000
./montecarlo2 1000000000
./montecarlo2 10000000000
./montecarlo2 100000000000
```

**Do not request more than 2 hours.** Your code should be able to finish in this amount of time. If your code cannot finsih in this amount of time, see if there are further optimizations you can add to your code to get it to complete in time.

5. Launch your job using `sbatch`. When it has finished, you should have the results in `mc10d.out`, and any error messages in `mc10d.err`. What was the result of the integral? Did it look like the integral was converging to a common value as the number of points increased?

6. Add/commit/push the following files to GitHub:
  * Your source code for `montecarlo2`. (2 points)
  * Your `mc10d.job` file. (2 points)
  * Your `mc10d.out` and `mc10d.err` files. For each run that completes in the two hour time constraint (and with the right answer!), you will get:
     * 100 million points: 2 points
     * 1 billion points: 4 points
     * 10 billion points: 8 points
     * 100 billion points: 12 points
