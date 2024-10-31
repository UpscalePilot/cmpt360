# Makefile for Ben Morley

# C compiler
CC:= gcc

# C Flags
CFLAGS:=-std=c11 -Wall -lc

# V Path
VPATH:= src bin obj

fileType: fileType.o		# create  app
	$(CC) -o $@ $(CFLAGS) $^
	mv *.o obj	

#main.o: main.c main.h 
#	$(CC) $(CFLAGS) -c $^
#	mv src/*.gch obj

%.o: %.c			# compile .c files
	$(CC) -c $(CFLAGS) $^


.PHONY: clean, run
clean:				# removes all .o and app files
	rm -f *.o fileType obj/*.o obj/*.gch


run:				# runs app_name application
	./fileType



