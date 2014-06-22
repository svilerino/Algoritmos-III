CC=g++
LD=g++
CFLAGS=-Wall -g -std=c++11
LDFLAGS=
RM=rm

.PHONY: all clean

all: bqlocal golosa

bqlocal: bqlocal.o grafo.o
	$(LD) $(LDFLAGS) -o bqlocal bqlocal.o grafo.o

golosa: golosa.o grafo.o
	$(LD) $(LDFLAGS) -o golosa golosa.o grafo.o

clean:
	- $(RM) -rf *.o *~ bqlocal golosa

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

