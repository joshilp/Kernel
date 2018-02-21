# Define what compiler to use and the flags.
CC=gcc
CXX=g++
CCFLAGS= -std=c99 -D _GNU_SOURCE -static 


all: array_stats_test process_ancestors_test


# Compile all .c files into .o files
# % matches all (like * in a command)
# $< is the source file (.c file)
%.o : %.c
	$(CC) -c $(CCFLAGS) $<

array_stats_test: array_stats_test.o 
	$(CC) $(CCFLAGS) array_stats_test.o -o array_stats_test  

process_ancestors_test: process_ancestors_test.o
	$(CC) $(CCFLAGS) process_ancestors_test.o -o process_ancestors_test

transfer:
	scp -P 2222 array_stats_test process_ancestors_test root@localhost:~

clean:
	rm -f core *.o array_stats_test process_ancestors_test