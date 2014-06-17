CC=g++
LD=g++
CFLAGS=-Wall -g -std=c++11
LDFLAGS=
RM=rm
OBJECTS=bqlocal.o grafo.o

.PHONY: all clean

all: $(OBJECTS)
	$(LD) $(LDFLAGS) -o bqlocal $(OBJECTS)

clean:
	- $(RM) -rf *.o *~ bqlocal

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

