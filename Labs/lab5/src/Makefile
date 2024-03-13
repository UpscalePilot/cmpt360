
CC = gcc
CFLAGS = -std=c11 -Wall
DEPS = dStruct.h
OBJ = dStruct.o procSim.o

FIFO = -D ALGOR_FIFO=1
SJF = -D ALGOR_FIFO=1 -D ALGOR_SJF=1
RR = -D ALGOR_FIFO=1 -D ALGOR_SJF=0 -D ALGOR_RR=1
TIME = -D TIME_DT=0

all: app

app: $(OBJ)
	$(CC) $(CFLAGS) -o app $(OBJ)

dStruct.o: dStruct.c
	$(CC) $(CFLAGS) -c dStruct.c

procSim.o: procSim.c
	$(CC) $(CFLAGS) -c procSim.c 


clean:
	rm -f *.o
