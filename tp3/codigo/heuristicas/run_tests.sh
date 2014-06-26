#!/bin/bash
#iterar sobre la carpeta de resultados, 
#	los que no tengan stdout no => ignoro, o anoto que hubo error...
#	los que tengan salida piola por stdout => grafico y comparo sol vs exacta


red='\e[0;31m'
green='\e[0;32m'
NC='\e[0m' # No Color
echo ""
echo "Running tests..."
TESTS_INPUT="test-cases"
TESTS_OUTPUT="test-results"
TIMING_OUTPUT="timings-out"
if ls test-cases/*.in &> /dev/null; then
	pushd test-cases
	for file in *.in; do				
		echo -n "Corriendo heuristica constructiva golosa con archivo de input $file..."
		../golosa < "$file" > "../$TESTS_OUTPUT/golosa/$file.out" 2> "../$TIMING_OUTPUT/golosa/$file.out"
		echo "Ok!"
		echo -n "Corriendo heuristica de busqueda local con archivo de input $file..."
		../bqlocal < "$file" > "../$TESTS_OUTPUT/bqlocal/$file.out" 2> "../$TIMING_OUTPUT/bqlocal/$file.out"
		echo "Ok!"
		echo -n "Corriendo metaheuristica grasp con archivo de input $file..."
		../grasp < "$file" > "../$TESTS_OUTPUT/grasp/$file.out" 2> "../$TIMING_OUTPUT/grasp/$file.out"
		echo "Ok!"
#	    ../../../ej1/ej1 --measure-time < "$file" > "$TESTS_OUTPUT/result_$file.out" 2> "$TIMING_OUTPUT/timing_$file.out"
#	    ../../random_testcase_builder/solucionar_ej1 < "$file" > "$TESTS_OUTPUT/result_$file_solver_out.out"	    
#	    DIFF=$(diff "$TESTS_OUTPUT/result_$file.out" "$TESTS_OUTPUT/result_$file_solver_out.out") 
#		if [ "$DIFF" != "" ]
#		then
#			diff "$TESTS_OUTPUT/result_$file.out" "$TESTS_OUTPUT/result_$file_solver_out.out" > "$TESTS_OUTPUT/DIFF_EJ1_SOLVED_1.$file.out"			
#			echo -e "${red}BAD TEST!!!${NC}"
#		else		    		    
#		    timeElapsed=$(cat "$TIMING_OUTPUT/timing_$file.out" | awk -F' ' '{print $2}')
#	    	echo -e "${green}Ok! in $timeElapsed micro-seconds ${NC}"
#		fi
	done
#	#concateno los resultados
#	cat "$TIMING_OUTPUT"/*.out > tmp_plot.out
#	#ordeno plot por la primer columna de numeros 
#	sort tmp_plot.out -k1,1 --numeric-sort > plot.out
#	rm -f tmp_plot.out
#	rm -f "$TESTS_OUTPUT"/result_.out
	popd
else
    echo "[WARN] NO existen archivos de testing"
fi


#reset
#make clean all
#echo "instancia de input $1"
#valgrind --leak-check=full ./golosa < "$1" > "test-results/$1.out"
#valgrind --leak-check=full ./bqlocal < "$1" > "test-results/$1.out"
#valgrind --leak-check=full ./grasp < "$1" > "test-results/$1.out"

#red='\e[0;31m'
#green='\e[0;32m'
#NC='\e[0m' # No Color
#echo ""
#echo "Running tests..."
#TESTS_OUTPUT="tests_out"
#TIMING_OUTPUT="timings_out"
#if ls ../testcases/ejercicio1/*.ej1 &> /dev/null; then
#	pushd ../testcases/ejercicio1/
#	for file in *.ej1; do    
#		printf "[EJ1 $file]..."
#	    ../../../ej1/ej1 --measure-time < "$file" > "$TESTS_OUTPUT/result_$file.out" 2> "$TIMING_OUTPUT/timing_$file.out"
#	    ../../random_testcase_builder/solucionar_ej1 < "$file" > "$TESTS_OUTPUT/result_$file_solver_out.out"	    
#	    DIFF=$(diff "$TESTS_OUTPUT/result_$file.out" "$TESTS_OUTPUT/result_$file_solver_out.out") 
#		if [ "$DIFF" != "" ]
#		then
#			diff "$TESTS_OUTPUT/result_$file.out" "$TESTS_OUTPUT/result_$file_solver_out.out" > "$TESTS_OUTPUT/DIFF_EJ1_SOLVED_1.$file.out"			
#			echo -e "${red}BAD TEST!!!${NC}"
#		else		    		    
#		    timeElapsed=$(cat "$TIMING_OUTPUT/timing_$file.out" | awk -F' ' '{print $2}')
#	    	echo -e "${green}Ok! in $timeElapsed micro-seconds ${NC}"	    
#		fi
#	done
#	#concateno los resultados
#	cat "$TIMING_OUTPUT"/*.out > tmp_plot.out
#	#ordeno plot por la primer columna de numeros 
#	sort tmp_plot.out -k1,1 --numeric-sort > plot.out
#	rm -f tmp_plot.out
#	rm -f "$TESTS_OUTPUT"/result_.out
#	popd
#else
#    echo "[WARN] NO existen archivos de test para el ejercicio 1"
#fi
#
#if ls ../testcases/ejercicio2/*.ej2 &> /dev/null; then
#    pushd ../testcases/ejercicio2/ 
#	for file in *.ej2; do    
#		printf "[EJ2 $file]..."
#	    ../../../ej2/ej2 --measure-time < "$file" > "$TESTS_OUTPUT/result_$file.out" 2> "$TIMING_OUTPUT/timing_$file.out"
#	    timeElapsed=$(cat "$TIMING_OUTPUT/timing_$file.out" | awk -F' ' '{print $2}')
#	    echo -e "${green}Ok! in $timeElapsed micro-seconds ${NC}"	    
#	done
#	#concateno los resultados
#	cat "$TIMING_OUTPUT"/*.out > tmp_plot.out
#	#ordeno plot por la primer columna de numeros 
#	sort tmp_plot.out -k1,1 --numeric-sort > plot.out
#	rm -f tmp_plot.out
#	popd
#else
#    echo "[WARN] NO existen archivos de test para el ejercicio 2"
#fi
#
#if ls ../testcases/ejercicio3/*.ej3 &> /dev/null; then
#	pushd ../testcases/ejercicio3/ 
#	for file in *.ej3; do    
#		printf "[EJ3 $file]..."
#	    ../../../ej3/ej3 --measure-time < "$file" > "$TESTS_OUTPUT/result_$file.out" 2> "$TIMING_OUTPUT/timing_$file.out"
#	    timeElapsed=$(cat "$TIMING_OUTPUT/timing_$file.out" | awk -F' ' '{print $4}')
#	    #timeElapsed=$(cat "$TIMING_OUTPUT/timing_$file.out")
#	    echo -e "${green}Ok! in $timeElapsed micro-seconds ${NC}"	    	    
#	done
#	#concateno los resultados
#	cat "$TIMING_OUTPUT"/*.out > tmp_plot.out
#	#ordeno plot por la primer columna de numeros 
#	sort tmp_plot.out -k1,1 --numeric-sort > plot.out
#	rm -f tmp_plot.out
#	popd
#else
#    echo "[WARN] NO existen archivos de test para el ejercicio 3"
#fi