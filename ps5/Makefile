.PHONY: clean all

OS:=$(shell uname -s)

ifeq ($(OS),Darwin)
	CC:=gcc-5
else
	CC:=gcc 
endif

libs:=-lm
CFLAGS:=--std=c99 -O2 -Wall

serial_program: pythagoreanTriplets.c
		$(CC) -o $@ $^ $(libs) 

omp_program: pythagoreanTriplets.c
		$(CC) -o $@ $^ $(CFLAGS) $(libs) -fopenmp -DHAVE_OPENMP

mpi_program: pythagoreanTriplets.c
		mpicc -o $@ $^ $(CFLAGS) $(libs) -DHAVE_MPI

combined_program: pythagoreanTriplets.c
		mpicc -o $@ $^ $(CFLAGS) $(libs) -fopenmp -DHAVE_MPI -DHAVE_OPENMP

clean:
		rm -f serial_program omp_program mpi_program combined_program

