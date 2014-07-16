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
	for file in *.in; do
		#for heuristica in "bqlocal" "golosa" "grasp"; do
		for heuristica in "bqlocal"; do
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
			    timeElapsed=$(cat "../$TIMING_OUTPUT/$heuristica/$file.out" | awk -F' ' '{print $4}')
			    echo "$cantNodos $cantAristas $timeElapsed " >> ../"$heuristica".tmpplot
		    	echo -e "${green}Ok! in $timeElapsed micro-seconds ${NC}"
			fi
		done
	done
	popd
	./plot.sh
else
    echo "[WARN] NO existen archivos de testing"
fi
rm -f no.txt