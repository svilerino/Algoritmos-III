CC=g++
LD=g++
CFLAGS=-Wall -g
LDFLAGS=
RM=rm
OBJECTS=bqlocal.o

.PHONY: all clean

all: bqlocal

clean:
	- $(RM) -rf *.o *~ bqlocal

bqlocal: $(OBJECTS)
	$(LD) $(LDFLAGS) -o bqlocal $(OBJECTS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

