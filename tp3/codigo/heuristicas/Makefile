CC=g++
LD=g++
CFLAGS=-Wall -O2 -ggdb -std=c++0x -pedantic -Werror -Wno-unused-variable
LDFLAGS=
RM=rm

.PHONY: all clean

all: codigo

codigo: bqlocal golosa grasp exacta minprogram generador_fisher_gates

exacta:
	$(MAKE) -C ../sol_exacta && cp ../sol_exacta/exacta ./exacta

bqlocal: bqlocal.o grafo.o
	$(LD) $(LDFLAGS) -o bqlocal bqlocal.o grafo.o

grasp: grasp.o grafo.o
	$(LD) $(LDFLAGS) -o grasp grasp.o grafo.o

golosa: golosa.o grafo.o
	$(LD) $(LDFLAGS) -o golosa golosa.o grafo.o

generador:
	$(MAKE) -C ../generar_grafos	

generador_fisher_gates:
	g++ -Wall -O2 -ggdb -std=c++0x -pedantic -Werror -Wno-unused-variable generador_fisher_gates.cpp -o generador_fisher_gates

tests: generador codigo
	./create_tests.sh

minprogram: 
	g++ -O2 min.cpp -o min

graphics: tests
	#./run_performance_tests.sh bqlocal
	#./run_performance_tests.sh golosa
	#./run_performance_tests.sh grasp
	#./run_performance_tests.sh exacta
	#./run_optimality_tests.sh --use-exacta
	#./run_optimality_tests.sh
	./run_minimality_tests.sh

clean:
	$(MAKE) -C ../generar_grafos clean
	$(MAKE) -C ../sol_exacta clean
	- $(RM) -rf *.o *~ bqlocal golosa grasp test-cases/*.* test-results/golosa/*.* test-results/grasp/*.* test-results/bqlocal/*.* test-results/exacta/*.*
	- $(RM) -rf timings-out/golosa/*.* timings-out/grasp/*.* timings-out/bqlocal/*.* timings-out/exacta/*.*
	- $(RM) -rf exacta
	- $(RM) -rf *.tmpplot
	- $(RM) -rf *.png
	- $(RM) -rf min
	- $(RM) -rf resumen_minimalidad.txt
	- $(RM) -rf resumen_grasp.txt
	- $(RM) -rf generador_fisher_gates
	- $(RM) -rf instancias_grasp_mejoran.txt	
	- $(RM) -rf resumen_busqueda_local.txt
	- $(RM) -rf test-cases/evolucion_iteraciones.txt
	- $(RM) -rf test-cases/evolucion_iteraciones_grasp.txt
	- $(RM) -rf costos_absolutos_iteraciones_bqlocal.txt
	- $(RM) -rf costos_absolutos_iteraciones_bqlocal_analisis.txt
	- $(RM) -rf costos_absolutos_iteraciones_grasp.txt
	- $(RM) -rf costos_absolutos_iteraciones_grasp_analisis.txt
	- $(RM) -rf evolucion_iteraciones.txt
	- $(RM) -rf evolucion_iteraciones_grasp.txt


%.o: %.cpp
	$(CC) -c $(CFLAGS) -o "$@" "$<"
