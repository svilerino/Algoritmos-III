CC=g++
LD=g++
CFLAGS=-Wall -g
LDFLAGS=
RM=rm
OBJECTS=generar_grafos.o

.PHONY: all clean

all: generar_grafos

clean:
	- $(RM) -rf *.o *~ generar_grafos

exacta: $(OBJECTS)
	$(LD) $(LDFLAGS) -o generar_grafos $(OBJECTS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"

