CC=g++
LD=g++
CFLAGS=-Wall -O2 -std=c++0x
LDFLAGS=
RM=rm
OBJECTS=exacta.o

.PHONY: all clean

all: exacta

clean:
	- $(RM) -rf *.o *~ exacta

exacta: $(OBJECTS)
	$(LD) $(LDFLAGS) -o exacta $(OBJECTS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

