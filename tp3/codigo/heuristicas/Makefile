CC=g++
LD=g++
CFLAGS=-Wall -O2 -g -std=c++11
LDFLAGS=
RM=rm

.PHONY: all clean

all: bqlocal golosa grasp

bqlocal: bqlocal.o grafo.o
	$(LD) $(LDFLAGS) -o bqlocal bqlocal.o grafo.o

grasp: grasp.o grafo.o
	$(LD) $(LDFLAGS) -o grasp grasp.o grafo.o

golosa: golosa.o grafo.o
	$(LD) $(LDFLAGS) -o golosa golosa.o grafo.o

clean:
	- $(RM) -rf *.o *~ bqlocal golosa grasp

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"
