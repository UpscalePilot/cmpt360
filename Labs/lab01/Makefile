# Makefile for Ben Morley/3142870 Nabil Gelle/3104375

# C compiler
CC:= gcc

# C Flags
CFLAGS:=-std=c11 -Wall

# V Path
VPATH:= src bin obj

test_dStruct: test_dStruct.o dStruct.o		# create  app
	$(CC) -o $@ $(CFLAGS) $^
	mv *.o obj	

dStruct.o: dStruct.c dStruct.h 
	$(CC) $(CFLAGS) -c $^
	mv src/*.gch obj

%.o: %.c			# compile .c files
	$(CC) -c $(CFLAGS) $^


.PHONY: clean, run
clean:				# removes all .o and app files
	rm -f *.o test_dStruct obj/*.o obj/*.gch


run:				# runs app_name application
	./test_dStruct



