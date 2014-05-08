CC=g++
RM=rm
LDFLAGS=
CFLAGS=-g -Wall -O2 -std=c++0x

SALIDA=ej2
OBJETOS=ej2.o
HEADERS=ej2.h timing.h

.PHONY: all clean

all: $(SALIDA)
	@echo "Compiling ej2..."	
clean:
	@echo "Cleaning ej2..."	
	- rm -f *~ $(OBJETOS) $(SALIDA)

$(SALIDA): $(OBJETOS) $(HEADERS)
	$(CC) $(OBJETOS) $(LDFLAGS) -o $(SALIDA)

%.o: %.cpp $(HEADERS)
	$(CC) -c $(CFLAGS) -o "$@" "$<"

