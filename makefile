# Compiler
CC = g++
# Headers, target and flag for compiling
# PThreads
PTHEADERS = pthread_shortpath.h
PTTARGET = pthread_shortpath
PTFLAG = -pthread
# OpenMP
OMPHEADERS = omp_shortpath.h
OMPTARGET = omp_shortpath
OMPFLAG = -fopenmp
# FILES
PATH1 = 1.path


# Command: $ make
all: omp pt

# Command: $ make omp
omp: $(OMPTARGET).cpp $(OMPHEADERS)
	$(CC)-8 $(OMPFLAG) -o $(OMPTARGET).o $(OMPTARGET).cpp

# Command: $ make pt
pt: $(PTTARGET).cpp $(PTHEADERS)
	$(CC) $(PTFLAG) -o $(PTTARGET).o $(PTTARGET).cpp

# Command: $ make omp1
omp1: ./$(OMPTARGET).o
	./$(OMPTARGET).o $(PATH1)

# Command: $make pt1
pt1: ./$(PTTARGET).o
	./$(PTTARGET).o $(PATH1)

clean:
	rm *.o
