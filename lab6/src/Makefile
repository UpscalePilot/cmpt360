
CC = gcc
CFLAGS = -std=c11 -Wall
LIBS = -lcairo

SRCS = plotData.c
OBJS = $(SRCS:.c=.o)

TARGETS = fifo sjf rr mlfq plotData

DEPS = dStruct.h
OBJ = dStruct.o

FIFO = -D ALGOR_FIFO=1 -D ALGOR_SJF=0 -D ALGOR_RR=0
SJF = -D ALGOR_FIFO=0 -D ALGOR_SJF=1 -D ALGOR_RR=0
RR = -D ALGOR_FIFO=0 -D ALGOR_SJF=0 -D ALGOR_RR=1
MLFQ = -D ALGOR_FIFO=0 -D ALGOR_SJF=0 -D ALGOR_RR=1 -D ALGOR_MLFQ=1
TIME = -D TIME_DT=0 -D TIME_JIFFY=0

.PHONY: all

all: $(TARGETS)

fifo: $(OBJ) $(DEPS) fifo.o
	$(CC) $(CFLAGS) $(OBJ) fifo.o -o $@

fifo.o: procSim.c
	$(CC) $(CFLAGS) -c procSim.c $(FIFO) $(TIME) -o $@

sjf: $(OBJ) $(DEPS) sjf.o
	$(CC) $(CFLAGS) $(OBJ) sjf.o -o $@

sjf.o: procSim.c
	$(CC) $(CFLAGS) -c procSim.c $(SJF) $(TIME) -o $@

rr: $(OBJ) $(DEPS) rr.o
	$(CC) $(CFLAGS) $(OBJ) rr.o -o $@

rr.o: procSim.c 
	$(CC) $(CFLAGS) -c procSim.c $(RR) $(TIME) -o $@

mlfq: $(OBJ) $(DEPS) mlfq.o
	$(CC) $(CFLAGS) $(OBJ) mlfq.o -o $@

mlfq.o: procSim.c 
	$(CC) $(CFLAGS) -c procSim.c $(MLFQ) $(TIME) -o $@

dStruct.o: $(DEPS)
	$(CC) $(CFLAGS) -c dStruct.c

plotData: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) -lcairo

plotData.o: plotData.c
	$(CC) $(CFLAGS) -c plotData.c -o $@

.PHONY: clean

clean:
	rm -f *.o $(TARGETS) $(OBJS)
