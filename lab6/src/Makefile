VPATH:= bin/ obj/
CC = gcc
CFLAGS = -std=c11 -Wall
# LIBS = -lcairo

# SRCS = plotData.c
# OBJS = $(SRCS:.c=.o)

TARGETS = fifo sjf rr mlfq plotData

# DEPS = dStruct.h
# OBJ = dStruct.o

FIFO = -D ALGOR_FIFO=1 -D ALGOR_SJF=0 -D ALGOR_RR=0
SJF = -D ALGOR_FIFO=0 -D ALGOR_SJF=1 -D ALGOR_RR=0
RR = -D ALGOR_FIFO=0 -D ALGOR_SJF=0 -D ALGOR_RR=1
MLFQ = -D ALGOR_FIFO=0 -D ALGOR_SJF=0 -D ALGOR_RR=1 -D ALGOR_MLFQ=1
TIME = -D TIME_DT=0 -D TIME_JIFFY=0


.PHONY: all

#all: $(TARGETS) sort copy
all: $(TARGETS) sort

fifo: fifo.o dStruct.o
	$(CC) -o $@ $(CFLAGS) $^

fifo.o: procSim.c
	$(CC) $(CFLAGS) -c $^ $(FIFO) $(TIME) -o $@

sjf: sjf.o dStruct.o
	$(CC) -o $@ $(CFLAGS) $^

sjf.o: procSim.c
	$(CC) $(CFLAGS) -c $^ $(SJF) $(TIME) -o $@

rr: rr.o dStruct.o
	$(CC) -o $@ $(CFLAGS) $^

rr.o: procSim.c 
	$(CC) $(CFLAGS) -c $^ $(RR) $(TIME) -o $@

mlfq: mlfq.o dStruct.o
	$(CC) -o $@ $(CFLAGS) $^

mlfq.o: procSim.c 
	$(CC) $(CFLAGS) -c $^ $(MLFQ) $(TIME) -o $@

dStruct.o: dStruct.c dStruct.h
	$(CC) $(CFLAGS) -c $^

plotData: plotData.o
	$(CC) -o $@ $(CFLAGS) $^ -lcairo

plotData.o: plotData.c
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: clean

clean:
	rm -rf obj/* bin/* 
	rm -f *.o $(TARGETS)

sort:
	mv *.o *.gch obj 2>/dev/null; true
	mv $(TARGETS) bin 2>/dev/null; true

copy:
	cp ../newProcB/* ../newProc/


# fifo: fifo.o $(OBJ) $(DEPS) 
# 	$(CC) $(CFLAGS) $(OBJ) $< -o $@

# fifo.o: procSim.c
# 	$(CC) $(CFLAGS) -c procSim.c $(FIFO) $(TIME) -o $@

# sjf: $(OBJ) $(DEPS) sjf.o
# 	$(CC) $(CFLAGS) $(OBJ) sjf.o -o $@

# sjf.o: procSim.c
# 	$(CC) $(CFLAGS) -c procSim.c $(SJF) $(TIME) -o $@

# rr: $(OBJ) $(DEPS) rr.o
# 	$(CC) $(CFLAGS) $(OBJ) rr.o -o $@

# rr.o: procSim.c 
# 	$(CC) $(CFLAGS) -c procSim.c $(RR) $(TIME) -o $@

# mlfq: $(OBJ) $(DEPS) mlfq.o
# 	$(CC) $(CFLAGS) $(OBJ) mlfq.o -o $@

# mlfq.o: procSim.c 
# 	$(CC) $(CFLAGS) -c procSim.c $(MLFQ) $(TIME) -o $@

# rm -f *.o $(TARGETS) $(OBJS)