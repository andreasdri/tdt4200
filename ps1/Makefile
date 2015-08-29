.PHONY: clean all

START:=2
STOP:=10000000
PROCESSES:=4

serial: computeSerial.c
	gcc -o $@ computeSerial.c -std=c99 -lm -O2

run_serial: serial
	./serial $(START) $(STOP)

all: computeMPI.c
	mpicc -Wall -std=c99 -o parallel computeMPI.c
	mpirun -np $(PROCESSES) ./parallel $(START) $(STOP)

clean:
	rm -f serial parallel
