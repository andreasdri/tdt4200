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
	
	// Flinkere på testing i python...
	// printf("isValidTrapiez returns: %i \n", isValidTrapiez(3,4,5));
	// printf("gcd returns: %i \n", gcd(3,9));
	// printf("elements_are_unique returns: %i \n", elements_are_unique(3,9,10));
	// printf("is valid sum %i \n", gives_valid_triplet_sum(3,4,5));
	// printf("no_common_dividors returns: %i \n", no_common_dividors(3,4,5));
	// printf("3,4,5 is a valid trapiez: %i \n", isValidTrapiez(3,4,5));
	// printf("10,24,43 is a valid trapiez: %i \n", isValidTrapiez(10,24,43));
	// printf("45,28,53 is a valid trapiez: %i \n", isValidTrapiez(45,28,53));

	int a,b,c;
	int sum = 0;
	for(c=start[0]; c < stop[0]; ++c){
		for(b=4; b < c; ++b){
			for(a=3; a < b; ++a){
				if (isValidTrapiez(a,b,c))
				{
					printf("[%i,%i,%i] is valid\n", a,b,c);
					sum++;
				}
			}
		}
	}

	fclose(fp);
	printf("%d\n", sum);

	return 0;
}

int isValidTrapiez(int a, int b, int c){
	// things to check 
		// [x] all are different/unique - could probably be checked elsewhere, like a cuntinue tag in loop
		// gcd of all are 1
		// rightsum a*a+b*b == c*c
	if (elements_are_unique(a,b,c) == 0){
		return 0;
	}
	else if (no_common_dividors(a,b,c) == 0)
	{
		return 0;
	}
	else if (gives_valid_triplet_sum(a,b,c) == 0)
	{
		return 0;
	}
	return 1;
}

int gcd(x,y){
	// optimalize with whileloop instad of requsive?
	if (y==0)
	{
		return x;
	}
	else
	{
		return gcd(y, x % y);
	}
}

int no_common_dividors(x,y,z){
	// returns true if gcd between all of them is 1
	if(gcd(x,y)==1 && gcd(x,z)==1 && gcd(y,z)==1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int elements_are_unique(int x, int y, int z){
	if (x != y && y != z && x != z)
	{
		return 1;	
	}
	else
	{
		return 0;	
	}


}

int gives_valid_triplet_sum(int x, int y, int z){
	//checks that x*x + y*y == z*z
	if (x*x + y*y == z*z)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}