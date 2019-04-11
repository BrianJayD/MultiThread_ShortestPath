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
PATH2 = 2.path


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

# Command: $make pt1a
pt2s: ./$(PTTARGET).o
	./$(PTTARGET).o $(PATH2) s

# Command: $make pt1a
pt1a: ./$(PTTARGET).o
	./$(PTTARGET).o $(PATH1) a

# Command: $make pt1b
pt1b: ./$(PTTARGET).o
	./$(PTTARGET).o $(PATH1) b

# Command: $make pt1c
pt1c: ./$(PTTARGET).o
	./$(PTTARGET).o $(PATH1) c

clean:
	rm *.o
