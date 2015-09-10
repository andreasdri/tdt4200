# Problem set 1, MPI Intro

TDT4200, Fall 2015

Andreas Drivenes (andredri)

## Part 1, Theory

### Problem 1, General Theory

a) Flynn's Taxonomy classifies computer architectures according to instruction and data (single or multiple).

|                | Single instruction| Multiple instruction |
| -------------- | ----------------- | -------------------- |
| Single data    | SISD              | MISD                 |
| Multiple data  | SIMD              | MIMD                 |

  i) MPI fits into Flynn's Taxonomy for a multiple instruction-multiple data architecture (MIMD).
Using MPI, we can split up work into several independent processes using its own stream of 
instructions and data running in parallel. 

b) MPI does not fit well with Shared Memory systems because it doesn't make sense to use a 
message-passing form of communcation.
This is because with shared memory, we have to use locks or transactions to prevent unwanted
side effects when all processes can access the same data in the same memory.

c) MPI fits well with Distributed Memory systems, because you can code as if the memory is 
independent. Each process has its own address space and you must only take care of 
sending and collecting the right data.

### Problem 2, Code Theory

a) The for loop receiving the data excepts to have everything in rank order, i.e: (1, 2, 3, ...)
. We must perhaps wait for some other process to finish before we can continue to receive.
For small ranges and many processes, the cost of setup, sending and receiving messages is larger
than the benefit of parallelization.

b)
Rank 0 (our master process) has to receive and sum all the sub computations. The book outlines 
an algorithm where core 0 adds the result of core 1, core 2 the result of core 3 etc. and in 
the next run, core 0 adds the result of core 2, 4 the result of 6 etc. 
Also, the last processes have to compute much larger numbers than the first processes. This 
could potentially be a bottleneck. 

## Part 2, Code

### Problem 1, MPI Intro

c) 

i) `O(n) = n, n = stop - start`. `Pi = n/P, i = 1..10`

ii) `O(P) = P * 6 = P , P = NumberOfProcesses`

iii) `O(P) =  , P = NumberOfProcesses`

iv) `O(P) = ..., P = NumberOfProcesses`

v) 

![Plot of range 2-100 with 1, 2, 4 and 8 processes](plots/2_100.png)

![Plot of range 2-100000 with 1, 2, 4 and 8 processes](plots/2_1000000.png)

![Plot of range 2-100000000 with 1, 2, 4 and 8 processes](plots/2_1000000000.png)

