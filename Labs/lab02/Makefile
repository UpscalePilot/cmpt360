# Makefile for Ben Morley

# C compiler
CC:= gcc

# C Flags
CFLAGS:=-std=c11 -Wall

# V Path
VPATH:= src obj

default: procSim plotData

procSim: procSim.o dStruct.o		# create  app
	$(CC) -o $@ $(CFLAGS) $^
	mv *.o obj

plotData: plotData.o obj/dStruct.o 
	$(CC) -o $@ $(CFLAGS) -lcairo $^
	mv *.o obj

dStruct.o: dStruct.c dStruct.h 
	$(CC) $(CFLAGS) -c $^
	mv src/*.gch obj

%.o: %.c			# compile .c files
	$(CC) -c $(CFLAGS) $^


.PHONY: clean, run
clean:				# removes all .o and app files
	rm -f *.o procSim obj/*.o obj/*.gch log/*


run:				# runs app_name application
	./procSim

plot:
	./plotData

