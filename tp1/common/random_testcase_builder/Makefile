TESTING_DIR = ../testing/
TESTS_DIR = ../testcases/
PLOT_DIR = ../plotting/
CC=gcc
RM=rm
LDFLAGS=
CFLAGS=-g -Wall

SALIDA=crear_casos
OBJETOS=crear_casos.o parsear_args.o
HEADERS=crear_casos.h parsear_args.h

.PHONY: all clean

all: $(SALIDA) solucionar_ej1
	
clean:
	@echo "Cleaning testcase builder..."	
	- rm *~ $(OBJETOS) $(SALIDA) solucionar_ej1.o solucionar_ej1
	rm -f $(TESTS_DIR)ejercicio1/*.ej1
	rm -f $(TESTS_DIR)ejercicio2/*.ej2
	rm -f $(TESTS_DIR)ejercicio3/*.ej3

	rm -f $(TESTS_DIR)ejercicio1/*.out
	rm -f $(TESTS_DIR)ejercicio2/*.out
	rm -f $(TESTS_DIR)ejercicio3/*.out

	rm -f $(TESTS_DIR)ejercicio1/tests_out/*.out
	rm -f $(TESTS_DIR)ejercicio2/tests_out/*.out
	rm -f $(TESTS_DIR)ejercicio3/tests_out/*.out

	rm -f $(TESTS_DIR)ejercicio1/timings_out/*.out
	rm -f $(TESTS_DIR)ejercicio2/timings_out/*.out
	rm -f $(TESTS_DIR)ejercicio3/timings_out/*.out

	rm -f $(PLOT_DIR)plots/*.png
	rm -f $(PLOT_DIR)plots/*.png
	rm -f $(PLOT_DIR)plots/*.png

solucionar_ej1: solucionar_ej1.o
	@echo ""
	@echo "Compiling testcase ej1 solver..."	
	$(CC) solucionar_ej1.o $(LDFLAGS) -o solucionar_ej1

$(SALIDA): $(OBJETOS) $(HEADERS)
	@echo ""
	@echo "Compiling testcase builder..."	
	$(CC) $(OBJETOS) $(LDFLAGS) -o $(SALIDA)

%.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) -o "$@" "$<"

crear_tests: all
	cd $(TESTING_DIR) ; ./create_tests.sh
	
