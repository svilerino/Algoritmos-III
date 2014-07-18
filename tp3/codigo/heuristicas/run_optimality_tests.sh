#!/bin/bash
#iterar sobre la carpeta de resultados, 
#	los que no tengan stdout no => ignoro, o anoto que hubo error...
#	los que tengan salida piola por stdout => grafico y comparo sol vs exacta

red='\e[0;31m'
green='\e[0;32m'
blue='\e[0;34m'
purple='\e[0;35m'
NC='\e[0m' # No Color
echo ""
echo "Running tests..."
TESTS_INPUT="test-cases"
TESTS_OUTPUT="test-results"
TIMING_OUTPUT="timings-out"
echo -n "no" > no.txt
if ls test-cases/*.in &> /dev/null; then
	rm -rf comparacion_optimalidad.tmpplot
	rm -rf comparacion_optimalidad_bqlocal.tmpplot
	rm -rf comparacion_optimalidad_golosa.tmpplot
	rm -rf comparacion_optimalidad_grasp.tmpplot
	pushd test-cases
	#------------------------------------------------------------------------------------------------------------
	#seleccionar aca abajo en el for heuristica que heuristicas se quieren comparar entre si
	for file in *.in; do
		for heuristica in "bqlocal" "golosa" "grasp"; do
			echo -n "Corriendo $heuristica con archivo de input $file..."
			limitw1=$(cat "$file" | awk -F' ' '{print $5}')
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
				pesow1=$(cat "../$TESTS_OUTPUT/$heuristica/$file.out" | awk -F' ' '{print $1}')
				pesow2=$(cat "../$TESTS_OUTPUT/$heuristica/$file.out" | awk -F' ' '{print $2}')

				if [ $pesow1 -gt $limitw1 ]
				then
					echo -e "${purple}Camino obtenido con peso $pesow1 pero excede el limite $limitw1."
					echo -e -n "${NC}"
				else
				    echo "$cantNodos $cantAristas $pesow2 $heuristica " >> ../"comparacion_optimalidad_$heuristica".tmpplot
			    	echo -e "${green}Ok! Camino obtenido con peso w2: ${blue}$pesow2${green} (${red}peso w1: $pesow1 | limit w1: $limitw1${green}) in $cantIters iterations ${NC}"
			    	#cat "../$TESTS_OUTPUT/$heuristica/$file.out"
			    	#echo ""
				fi
			fi
		done
	done
	popd
	python plotter.py comparacion_optimalidad.png "comparacion_optimalidad_golosa".tmpplot 6 "comparacion_optimalidad_bqlocal".tmpplot "comparacion_optimalidad_grasp".tmpplot
	#------------------------------------------------------------------------------------------------------------	
else
    echo "[WARN] NO existen archivos de testing"
fi
rm -f no.txt