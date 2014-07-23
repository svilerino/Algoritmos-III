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
testsnumber=0
min_match_golosa=0
min_match_bqlocal=0
min_match_grasp=0
echo -n "no" > no.txt
if ls test-cases/*.in &> /dev/null; then
	pushd test-cases
	#------------------------------------------------------------------------------------------------------------
	for file in *.in; do
		for heuristica in "bqlocal" "golosa" "grasp"; do
			limitw1=$(head -1 "$file" | awk -F' ' '{print $5}')
			nodosrc=$(head -1 "$file" | awk -F' ' '{print $3}')
			nododst=$(head -1 "$file" | awk -F' ' '{print $4}')
			echo -n "Corriendo $heuristica con archivo de input $file..."

			"../$heuristica" < "$file" > "../$TESTS_OUTPUT/$heuristica/$file.out" 2> "../$TIMING_OUTPUT/$heuristica/$file.out"

		    DIFF=$(diff "../$TESTS_OUTPUT/$heuristica/$file.out" "../no.txt") 
			if [ "$DIFF" == "" ]
			then			
				echo -e "${red}No existia solucion! Descripcion de la salida:"
				cat "../$TIMING_OUTPUT/$heuristica/$file.out"
				echo -e -n "${NC}"
				hay_solucion="no"
			else
				cantNodos=$(cat "../$TIMING_OUTPUT/$heuristica/$file.out" | awk -F' ' '{print $1}')
				cantAristas=$(cat "../$TIMING_OUTPUT/$heuristica/$file.out" | awk -F' ' '{print $2}')
				cantIters=$(cat "../$TIMING_OUTPUT/$heuristica/$file.out" | awk -F' ' '{print $3}')				    
				pesow1=$(cat "../$TESTS_OUTPUT/$heuristica/$file.out" | awk -F' ' '{print $1}')
				pesow2=$(cat "../$TESTS_OUTPUT/$heuristica/$file.out" | awk -F' ' '{print $2}')
				longuitudCaminoOutput=$(cat "../$TESTS_OUTPUT/$heuristica/$file.out" | awk -F' ' '{print $3}')
				timeElapsed=$(cat "../$TIMING_OUTPUT/$heuristica/$file.out" | awk -F' ' '{print $4}')

				if [ $pesow1 -gt $limitw1 ]
				then
					echo -e "${purple}Camino obtenido con peso $pesow1 pero excede el limite $limitw1."
					echo -e -n "${NC}"
					hay_solucion="no"
				else				    
			    	echo -e "${green}Ok! Camino obtenido entre ($nodosrc) y ($nododst) de longuitud ${blue}$longuitudCaminoOutput${green} con peso w2: ${blue}$pesow2${green} (${red}peso w1: $pesow1 | limit w1: $limitw1${green}) in $cantIters iterations in $timeElapsed microseconds ${NC}"
			    	echo "$pesow2 $timeElapsed" > tmp.solactual."$heuristica".optimalidad.txt
			    	hay_solucion="si"
				fi
			fi
		done
		if [ $hay_solucion == "si" ]
		then
			golosa_w2=$(head -1 tmp.solactual.golosa.optimalidad.txt | awk -F' ' '{print $1}')
			bqlocal_w2=$(head -1 tmp.solactual.bqlocal.optimalidad.txt | awk -F' ' '{print $1}')
			grasp_w2=$(head -1 tmp.solactual.grasp.optimalidad.txt | awk -F' ' '{print $1}')

			minimo=$(../min $golosa_w2 $bqlocal_w2 $grasp_w2)

			if [ $minimo -eq $golosa_w2 ]
			then
				#incremento golosa
				min_match_golosa=$((min_match_golosa+1))
			fi

			if [ $minimo -eq $bqlocal_w2 ]
			then
				#incremento bqlocal
				min_match_bqlocal=$((min_match_bqlocal+1))
			fi

			if [ $minimo -eq $grasp_w2 ]
			then
				#incremento grasp
				min_match_grasp=$((min_match_grasp+1))
			fi

			testsnumber=$(($testsnumber+1))
		fi
		echo "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"
	done
	rm -rf tmp.solactual.grasp.optimalidad.txt
	rm -rf tmp.solactual.golosa.optimalidad.txt
	rm -rf tmp.solactual.bqlocal.optimalidad.txt
	popd
	
	min_match_golosa=$(echo "scale=3; $min_match_golosa/$testsnumber" | bc -l )
	min_match_bqlocal=$(echo "scale=3; $min_match_bqlocal/$testsnumber" | bc -l )
	min_match_grasp=$(echo "scale=3; $min_match_grasp/$testsnumber" | bc -l )

	echo "Resultados" > resumen_minimalidad.txt
	echo "Cantidad de instancias ejecutadas: $testsnumber" >> resumen_minimalidad.txt
	echo "Cantidad de instancias donde da el minimo w2:" >> resumen_minimalidad.txt
	echo "        Golosa: $min_match_golosa" >> resumen_minimalidad.txt
	echo "        Busqueda local: $min_match_bqlocal" >> resumen_minimalidad.txt
	echo "        Grasp: $min_match_grasp" >> resumen_minimalidad.txt

	echo -e "${green}"		
	echo "Resultados:"	
	echo "----------------------------------------------------------------------------------------------"
	cat resumen_minimalidad.txt	
	echo "----------------------------------------------------------------------------------------------"
	echo -e "${NC}"
	#------------------------------------------------------------------------------------------------------------	
else
    echo "[WARN] NO existen archivos de testing"
fi
rm -f no.txt