

CC = gcc
CFLAGS = -pthread -Wall -lm -std=c11

defaults: lab10Test findme

lab10Test: lab10Test.o
	$(CC) -o $@ $(CFLAGS) $^

findme: findme.o
	$(CC) -o $@ $(CFLAGS) $^

%.o: %.c
	$(CC) -c $(CFLAGS) $^


.PHONY: clean, run

clean:
	rm -f *.o findme lab10Test
	# rm -rf test

run:
	# Example run:
	# ./findme ~/Code/cmpt360/lab10 -type f -name "gl*b.txt" -user upscalepilot -maxdepth 5
	# ./findme ~/Code/cmpt360/lab10 -type f -name "?mark.txt" -user upscalepilot -maxdepth 5
	# ./findme ~/Code/cmpt360/lab10 -type f -name "[s]ingle.txt" -user upscalepilot -maxdepth 5
	# ./findme ~/Code/cmpt360/lab10 -type f -name "[^p]ingle.txt" -user upscalepilot -maxdepth 5
	# ./findme ~/Code/cmpt360/lab10 -type f -name "[^f]*.c" -user upscalepilot -maxdepth 5
	./lab10Test
