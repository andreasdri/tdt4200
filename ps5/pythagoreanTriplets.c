#include <stdio.h> // for stdin
#include <stdlib.h>
#include <unistd.h> // for ssize_t
#include <math.h>

#ifdef HAVE_MPI
#include <mpi.h>
#endif

#ifdef HAVE_OPENMP
#include <omp.h>
#endif



int gcd (int a, int b) {
  	int c;
  	while (a != 0) {
		c = a;
    	a = b%a;
    	b = c;
  	}
  	return b;
}


int isPythagoreanPrimitive(int a, int b, int c) {
	if(a*a + b*b == c*c) {
		if (gcd(gcd(a, b), c) == 1) {
			return 1;
		}
		return 0;
	}
	return 0;
}


int main(int argc, char **argv) {
	char *inputLine = NULL; size_t lineLength = 0;
	int *start, *stop, *numThreads, amountOfRuns = 0;

	#ifdef HAVE_MPI
	int rank;
	int size;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	#else
	int rank = 0;
	int size = 1;
	#endif

	if (rank == 0) {
		// Read in first line of input
		getline(&inputLine, &lineLength, stdin);
		sscanf(inputLine, "%d", &amountOfRuns);
	}

	#ifdef HAVE_MPI
	MPI_Bcast(&amountOfRuns, 1, MPI_INT, 0, MPI_COMM_WORLD);
	#endif

	stop = (int*) calloc(amountOfRuns, sizeof(int));
	start = (int*) calloc(amountOfRuns, sizeof(int));
	numThreads = (int*) calloc(amountOfRuns, sizeof(int));

	if (rank == 0) {
		int tot_threads, current_start, current_stop;
		for (int i = 0; i < amountOfRuns; ++i){

			// Read in each line of input that follows after first line
			free(inputLine); lineLength = 0; inputLine = NULL;
			ssize_t readChars = getline(&inputLine, &lineLength, stdin);

			// If there exists at least two matches (2x %d)...
			if (sscanf(inputLine, "%d %d %d", &current_start, &current_stop, &tot_threads) >= 2){
				if(current_start < 0 || current_stop < 0){
					current_start = 0, current_stop = 0;
				}
				stop[i] = current_stop;
				start[i] = current_start;
				numThreads[i] = tot_threads;

				if (tot_threads < 1 || tot_threads > 10) {
					numThreads[i] = 1;
				}

			}
		}
	}

	#ifdef HAVE_MPI
	MPI_Bcast(start, amountOfRuns, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(stop, amountOfRuns, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(numThreads, amountOfRuns, MPI_INT, 0, MPI_COMM_WORLD);
	#endif

	/*
	*	Remember to only print 1 (one) sum per start/stop.
	*	In other words, a total of <amountOfRuns> sums/printfs.
	*/


	for (int i = 0; i < amountOfRuns; i++) {


		int current_start = start[i];
		int current_stop = stop[i];
		int threads = numThreads[i];

		int sum = 0;
		int totalSum;

		#ifdef HAVE_MPI
		// Compute the local range, so that all the elements are accounted for.
	    int quotient = (current_stop - 4) / size;
	    int rest = (current_stop - 4) % size;
	    int begin = (rank * quotient) + 4;
		int end = (rank * quotient) + quotient + 4;
		// The last process must do some more work
	    if (rank + 1 == size) {
	        end += rest;
	    }

 		#else
 		int begin = 4;
 		int end = current_stop;
 		#endif

 		int b;
		# pragma omp parallel for num_threads(threads) \
			reduction(+: sum) schedule(static, 1)
		for (b = begin; b < end; b++) {
			int a;
			for (a = 3; a < b; a++) {
				int c = (int) sqrt(a * a + b * b);
				if (c >= current_start && c < current_stop && isPythagoreanPrimitive(a, b, c) == 1) {
					sum++;
				}
			}

		}

		#ifdef HAVE_MPI
		MPI_Reduce(&sum, &totalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		if(rank == 0)
			printf("%d\n", totalSum);
		#else
		printf("%d\n", sum);
		#endif

	}

	#ifdef HAVE_MPI
	MPI_Finalize();
	#endif

	return 0;
}
