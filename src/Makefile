CC=cc
CFLAGS=-O2
OMPFLAG=-fopenmp
SRC=openmp_yield.c
BIN=openmp_yield

all: $(BIN) Makefile

openmp_yield: $(SRC) Makefile
	$(CC) $(CFLAGS) $(OMPFLAG) -o $@ $<

clean:
	rm -rf $(BIN) *.o
