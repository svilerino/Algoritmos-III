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
echo -n "no" > no.txt
if ls test-cases/*.in &> /dev/null; then
	pushd test-cases

#	#Heuristica busqueda local
#	#------------------------------------------------------------------------------------------------------------
#		min_iteraciones=20000000
#		max_iteraciones=0
#		promedio_iters=0
#		cant_test_files=0
#		for file in *.in; do
#			heuristica="bqlocal"
#			echo -n "Corriendo $heuristica con archivo de input $file..."
#			"../$heuristica" < "$file" > "../$TESTS_OUTPUT/$heuristica/$file.out" 2> "../$TIMING_OUTPUT/$heuristica/$file.out"
#
#		    DIFF=$(diff "../$TESTS_OUTPUT/$heuristica/$file.out" "../no.txt") 
#			if [ "$DIFF" == "" ]
#			then			
#				echo -e "${red}No existia solucion! Descripcion de la salida:"
#				cat "../$TIMING_OUTPUT/$heuristica/$file.out"
#				echo -e -n "${NC}"
#			else    		    
#				cantNodos=$(cat "../$TIMING_OUTPUT/$heuristica/$file.out" | awk -F' ' '{print $1}')
#				cantAristas=$(cat "../$TIMING_OUTPUT/$heuristica/$file.out" | awk -F' ' '{print $2}')
#				cantIters=$(cat "../$TIMING_OUTPUT/$heuristica/$file.out" | awk -F' ' '{print $3}')
#			    timeElapsed=$(cat "../$TIMING_OUTPUT/$heuristica/$file.out" | awk -F' ' '{print $4}')
#			    echo "$cantNodos $cantAristas $timeElapsed " >> ../"$heuristica".tmpplot
#		    	echo -e "${green}Ok! $cantIters iterations in aprox. $timeElapsed micro-seconds per iteration ${NC}"
#
#		    	python ../plotter.py ../"$TESTS_OUTPUT"/"$heuristica"/"$file"_iters.png "evolucion_iteraciones.txt" 5
#
#		    	if [ $min_iteraciones -gt $cantIters ]
#				then
#					min_iteraciones=$cantIters					
#				fi
#
#				if [ $max_iteraciones -lt $cantIters ]
#				then
#					max_iteraciones=$cantIters					
#				fi
#				promedio_iters=$(($promedio_iters + $cantIters))
#			fi
#			rm -rf evolucion_iteraciones.txt
#			cant_test_files=$((cant_test_files+1))
#		done
#		promedio_iters=$((promedio_iters / cant_test_files))
#		rm -rf ../resumen_busqueda_local.txt
#		echo "Cantidad de tests realizados: $cant_test_files" >> ../resumen_busqueda_local.txt
#		echo "Iteraciones promedio: $promedio_iters" >> ../resumen_busqueda_local.txt
#		echo "Minima cantidad de iteraciones: $min_iteraciones" >> ../resumen_busqueda_local.txt
#		echo "Maxima cantidad de iteraciones: $max_iteraciones" >> ../resumen_busqueda_local.txt
#	#------------------------------------------------------------------------------------------------------------

	#Heuristica golosa
	#------------------------------------------------------------------------------------------------------------
		for file in *.in; do
			heuristica="golosa"
			echo -n "Corriendo $heuristica con archivo de input $file..."
			"../$heuristica" < "$file" > "../$TESTS_OUTPUT/$heuristica/$file.out" 2> "../$TIMING_OUTPUT/$heuristica/$file.out"

		    DIFF=$(diff "../$TESTS_OUTPUT/$heuristica/$file.out" "../no.txt") 
			if [ "$DIFF" == "" ]
			then			
				echo -e "${red}No existia solucion! Descripcion de la salida:"
				cat "../$TIMING_OUTPUT/$heuristica/$file.out"
				echo -e -n "${NC}"
			else
				cantNodos=$(cat "../$TIMING_OUTPUT/$heuristica/$file.out" | awk -F' ' '{print $1}')
				cantAristas=$(cat "../$TIMING_OUTPUT/$heuristica/$file.out" | awk -F' ' '{print $2}')
				cantIters=$(cat "../$TIMING_OUTPUT/$heuristica/$file.out" | awk -F' ' '{print $3}')
			    timeElapsed=$(cat "../$TIMING_OUTPUT/$heuristica/$file.out" | awk -F' ' '{print $4}')
			    echo "$cantNodos $cantAristas $timeElapsed " >> ../"$heuristica".tmpplot
		    	echo -e "${green}Ok! $cantIters iterations in aprox. $timeElapsed micro-seconds per iteration ${NC}"
		    fi
		done
	#------------------------------------------------------------------------------------------------------------
	popd
	./plot.sh
else
    echo "[WARN] NO existen archivos de testing"
fi
rm -f no.txt