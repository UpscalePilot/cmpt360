
CC = gcc
CFLAGS = -std=c11 -Wall


DEPS = dStruct.h
OBJ = dStruct.o procSim.o


all: app plotData

plotData: plotData.py
	python3 plotData.py 

app: $(OBJ)
	$(CC) $(CFLAGS) -o app $(OBJ)

dStruct.o: dStruct.c
	$(CC) $(CFLAGS) -c dStruct.c

procSim.o: procSim.c
	$(CC) $(CFLAGS) -c procSim.c


clean:
	rm -f *.o
