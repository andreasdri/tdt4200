# Problem set 2, CUDA Intro 

TDT4200, Fall 2015

Andreas Drivenes (andredri)

## Part 1, Theory

### Problem 1, Architectures & Programming Models 

a) Differences between Nvidia Maxwell, ARM big.LITTLE, Vilje @ NTNU, Typical modern-day CPU.
Keywords being: Homogeneous cores, heterogeneous cores, clusters, NUMA, threads.

ARM big.LITTLE has a heterogeneous architecture, meaning that it has two different clusters
for different needs. There is one low-power cluster with low performance cores, and one
high-power cluster with high performance cores. The workload between the two can be swapped,
and memory is shared. 
https://en.wikipedia.org/wiki/ARM_big.LITTLE

The Nvidia Maxwell architecture has a lot of homogeneous cores that can compute in parallel.

Vilje @ NTNU has 1404 nodes in a cluster. Each node has 16 cores and 32 Gib memory. The cores are
homogeneous, but very general purpose, and the system uses NUMA for the memory design, since
the cores share a large, slower amount of memory in addition to having its own faster memory. 
https://www.hpc.ntnu.no/display/hpc/About+Vilje

A typical modern day CPU is organized a lot like one node from Vilje. It can have multiple
homogenous cores with access to different types of memory.

b) Explain Nvidia's SIMT addition to Flynn's Taxonomy, and how it is realized, if at all, in
each of the architectures from a).

c) For each architecture from a), report which classification from Flynn's Taxonomy is best
suited. If applicable, also state if the architecture fits Nvidia's SIMT classification.
Give the reasoning(s) behind your answers.

### Problem 2, CUDA GPGPUs

(Maxwell architecture)

a) Explain the terms Threads, Grids, Blocks, and how they relate to one another (if at all)
Threads, grids and blocks are a software abstraction used to divide work when programming
in the CUDA environment. Threads are the main abstraction, and are divided into blocks, and
and blocks into grids. Both blocks and grids can be multiple dimensions. This level of abstraction with grids and blocks makes better GPU's able to execute the program faster by simply running more blocks concurrently.

*Thread* is a sequence of instructions that a processor can run. Threads are part of a program and they can somewhat run independent, though synchronisation often is needed. This gives the a program with different threads to have some threads running while others are blocked while waiting for i/o. In CUDA programming there are usually way more threads than in regular CPU programming.
*Blocks* are "Blocks" or a group of threads. The threads in a block can communicate with each other.
*Grids* are collection of blocks which are passed to the kernel for execution.

b)

c)

d)
A *Warp* is a group of 32 threads that are passed for execution to a Streaming Multiprocessor(SM). Inctructions to the cores are defined per warp. The Fermi arcitecture by Nvidia can have 48 threads in a warp.
*Occupancy* is a measure defining $\dfrac{Active warps}{Maximum Active warps}$. It's possible to limit the occupancy by register usage, shared memory usage and block size.
*Memory Coalescing*
*Local Memory* is a partion of main memory allocated to a program.
*Shared Memory* is memory that are shared for all CPU's. When all the CPU's have access the same memory there is often need for concurency controll. In parallel computing we have differ between shared memory and distributed memory.

## Part 2, Code

### Problem 1, CUDA Intro

c)
Transfer to device: 0.180544 ms
Transfer from device: 0.154688 ms
Total execution time (with I/O): 0.199 s
Total execution time (without I/O): 0.693408 ms

Transfer/Execution time without I/O = `100 * (0.180544 + 0.154688) ms / 0.693408 ms = 48.35 %`

