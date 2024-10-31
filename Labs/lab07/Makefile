# Makefile for Ben Morley/3142870

# C compiler
CC:= gcc

# C Flags
CFLAGS:=-std=c11 -Wall -lm

# V Path
VPATH:= src bin obj

test_pi: test_pi.o		# create  app
	$(CC) -o $@ $(CFLAGS) $^
	mv *.o obj	

#main.o: main.c main.h 
#	$(CC) $(CFLAGS) -c $^
#	mv src/*.gch obj

test_pi.o: test_pi.c			# compile .c files
	$(CC) -o $@ -c $(CFLAGS) $^


.PHONY: clean, run
clean:				# removes all .o and app files
	rm -f *.o test_pi obj/*.o obj/*.gch


run:				# runs app_name application
	./test_pi




