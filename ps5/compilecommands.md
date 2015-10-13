# Compilation of PS5
## Compilation of the serial executable:
`$ gcc −o serial_program pythagoreanTriplets.c`

## Compilation of the OpenMP executable:
`$ gcc −o omp_program pythagoreanTriplets.c −fopenmp −DHAVE_OPENMP`

## Compilation of the MPI executable:
`$ mpicc −o mpi_program pythagoreanTriplets.c −DHAVE_MPI`

## Compilation of the executable running OpenMP and MPIcombined:
`$ mpicc −o combined_program pythagoreanTriplets.c −fopenmp −DHAVE_MPI −DHAVE_OPENMP`

> gotten from the recitation slides.
