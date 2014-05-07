CC=g++
RM=rm
LDFLAGS=
CFLAGS=-g -Wall -O2 -std=c++0x

SALIDA=ej3
OBJETOS=ej3.o
HEADERS=ej3.h

.PHONY: all clean

all: $(SALIDA)
	@echo "Compiling ej3..."	
clean:
	@echo "Cleaning ej3..."	
	- rm -f *~ $(OBJETOS) $(SALIDA)

$(SALIDA): $(OBJETOS) $(HEADERS)
	$(CC) $(OBJETOS) $(LDFLAGS) -o $(SALIDA)

%.o: %.cpp $(HEADERS)
	$(CC) -c $(CFLAGS) -o "$@" "$<"

