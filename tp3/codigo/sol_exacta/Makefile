CC=g++
LD=g++
CFLAGS=-Wall
LDFLAGS=
RM=rm
OBJECTS=exacta.o

.PHONY: all clean

all: exacta

clean:
	- $(RM) *.o *~ exacta

exacta: $(OBJECTS)
	$(LD) $(LDFLAGS) -o exacta $(OBJECTS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

