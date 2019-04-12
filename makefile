# Compiler
CC = g++
# Headers, target and flag for compiling
# Sequential
SEQHEADERS = seq_shortpath.h
SEQTARGET = seq_shortpath
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
PATH3 = 3.path


# Command: $ make
all: seq omp pt

# Command: $ make seq
seq: $(SEQTARGET).cpp $(SEQHEADERS)
	$(CC) -o $(SEQTARGET).o $(SEQTARGET).cpp

# Command: $ make omp
omp: $(OMPTARGET).cpp $(OMPHEADERS)
	$(CC)-8 $(OMPFLAG) -o $(OMPTARGET).o $(OMPTARGET).cpp

# Command: $ make pt
pt: $(PTTARGET).cpp $(PTHEADERS)
	$(CC) $(PTFLAG) -o $(PTTARGET).o $(PTTARGET).cpp

# Command: $ make seq1a
seq1a: ./$(SEQTARGET).o
	./$(SEQTARGET).o $(PATH1) a

# Command: $ make seq1b
seq1b: ./$(SEQTARGET).o
	./$(SEQTARGET).o $(PATH1) b

# Command: $ make seq1c
seq1c: ./$(SEQTARGET).o
	./$(SEQTARGET).o $(PATH1) c

# Command: $ make seq1c
seq3a: ./$(SEQTARGET).o
	./$(SEQTARGET).o $(PATH3) a

# Command: $ make omp1a
omp1a: ./$(OMPTARGET).o
	./$(OMPTARGET).o $(PATH1) a

# Command: $ make omp1b
omp1b: ./$(OMPTARGET).o
	./$(OMPTARGET).o $(PATH1) b

# Command: $ make omp3a
omp3a: ./$(OMPTARGET).o
	./$(OMPTARGET).o $(PATH3) a

# Command: $ make omp1b
omp1c: ./$(OMPTARGET).o
	./$(OMPTARGET).o $(PATH1) c

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
