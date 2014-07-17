CC=g++
LD=g++
CFLAGS=-Wall -g
LDFLAGS=
RM=rm
OBJECTS=generar_grafos2.o

.PHONY: all clean

all: generar_grafos2

clean:
	- $(RM) -rf *.o *~ generar_grafos2

exacta: $(OBJECTS)
	$(LD) $(LDFLAGS) -o generar_grafos2 $(OBJECTS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

