#!/bin/bash 

make clean
make serial_program
make omp_program
make mpi_program
make combined_program

echo Serial
time ./serial_program < big.txt
echo OpenMP
time ./omp_program < big.txt
echo MPI
time mpirun -n 4 ./mpi_program < big.txt
echo Combined
time mpirun -n 4 ./combined_program < big.txt

