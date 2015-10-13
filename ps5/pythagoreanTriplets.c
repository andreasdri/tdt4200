#include <stdio.h> // for stdin
#include <stdlib.h>
#include <unistd.h> // for ssize_t

#ifdef HAVE_MPI
#include <mpi.h>
#endif

#ifdef HAVE_OPENMP
#include <omp.h>
#endif

int main(int argc, char **argv) {
	//char *inputLine = NULL; size_t lineLength = 0;
	int *start, *stop, *numThreads, amountOfRuns = 0;  // amountOfRuns is the same as (#linesInFile - 1) ? 
	int i; // have to have c99 to be able to define variable in forloop, define it here in the top and use it throughout the code
	char buff[1024];
	
	for (i = 0; i < argc; ++i)
	{
		printf("arg #%i is: %s \n", i, argv[i]);
	}
	
	if(argc != 2){
		printf("cERROR: Number of args shall be two, %i was given\n", argc);
		exit(1);
	}
	
	
	// try to open file, assumes that argument is right
	
	FILE *fp; // file pointer
	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		printf("cERROR: unable to open file with name: '%s'\n", argv[1]);
		exit(1);
	}
	else{
		printf("was able to open it!\n");
	}
	
	fgets(buff, 1024, fp);
	printf("first line is: %s\n", buff);
	
	//da har jeg lært at best å ha hente ut hver linje å så etterpå kjøre en sscanf på det... ikke fscanf med en gang..
	
	sscanf(buff, "%i", &amountOfRuns);
	

	//getline(&inputLine, &lineLength, stdin);
	//sscanf(inputLine, "%d", &amountOfRuns);

	stop = (int*) calloc(amountOfRuns, sizeof(int));
	start = (int*) calloc(amountOfRuns, sizeof(int));
	numThreads = (int*) calloc(amountOfRuns, sizeof(int));

	int tot_threads, current_start, current_stop;
	for (i = 0; i < amountOfRuns; ++i){

		// Read in each line of input that follows after first line
		//free(inputLine); lineLength = 0; inputLine = NULL;
		//ssize_t readChars = getline(&inputLine, &lineLength, stdin);
		
		//printf("buff: %s\n", buff);
		
		// If there exists at least two matches (2x %d)...
		
		
		fgets(buff, 1024, fp);
		if (sscanf(buff, "%i %i %i", &current_start, &current_stop, &tot_threads) == 3){
			if(current_start < 0 || current_stop < 0){
				current_start = 0, current_stop = 0;
			}
			stop[i] = current_stop;
			start[i] = current_start;
			numThreads[i] = tot_threads;
			
			printf("%d: [%d,%d], nrThreads:%i\n", i,start[i],stop[i],numThreads[i]);
		}
		else
		{
			if(current_start < 0 || current_stop < 0){
				current_start = 0, current_stop = 0;
			}
			stop[i] = current_stop;
			start[i] = current_start;
			numThreads[i] = 0;
			
			printf("%d: [%d,%d], nrThreads:%i\n", i,start[i],stop[i],numThreads[i]);
		}
	}

	/*
	*	Remember to only print 1 (one) sum per start/stop.
	*	In other words, a total of <amountOfRuns> sums/printfs.
	*/
	fclose(fp);
	int sum;
	printf("%d\n", sum);

	return 0;
}

//make help function to se if they have gcd == 1...