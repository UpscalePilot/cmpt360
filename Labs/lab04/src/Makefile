
CC = gcc
CFLAGS = -std=c11 -Wall -I/usr/include/cairo
LIBS = -lcairo

SRCS = plotData.c
OBJS = $(SRCS:.c=.o)

TARGETS = app plotData

DEPS = dStruct.h
OBJ = dStruct.o procSim.o

.PHONY: clean

app: $(OBJ)
	$(CC) $(CFLAGS) -o app $(OBJ)

plotData: $(OBJS)
	$(CC) $(CFLAGS) -o plotData $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGETS) *.o
