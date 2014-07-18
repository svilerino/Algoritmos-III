CC=g++
LD=g++
CFLAGS=-Wall -O2 -g -std=c++0x -pedantic -Werror -Wno-unused-variable
LDFLAGS=
RM=rm

.PHONY: all clean

all: codigo

codigo: bqlocal golosa grasp 

bqlocal: bqlocal.o grafo.o
	$(LD) $(LDFLAGS) -o bqlocal bqlocal.o grafo.o

grasp: grasp.o grafo.o
	$(LD) $(LDFLAGS) -o grasp grasp.o grafo.o

golosa: golosa.o grafo.o
	$(LD) $(LDFLAGS) -o golosa golosa.o grafo.o

tests: codigo
	./create_tests.sh

graphics: tests
	#./run_performance_tests.sh bqlocal
	#./run_performance_tests.sh golosa
	#./run_performance_tests.sh grasp
	./run_optimality_tests.sh

clean:
	- $(RM) -rf *.o *~ bqlocal golosa grasp test-cases/*.* test-results/golosa/*.* test-results/grasp/*.* test-results/bqlocal/*.*
	- $(RM) -rf timings-out/golosa/*.* timings-out/grasp/*.* timings-out/bqlocal/*.*
	- $(RM) -rf *.tmpplot
	- $(RM) -rf *.png
	- $(RM) -rf resumen_grasp.txt
	- $(RM) -rf resumen_busqueda_local.txt
	- $(RM) -rf evolucion_iteraciones.txt
	- $(RM) -rf evolucion_iteraciones_grasp.txt

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"
