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
	pushd test-cases
	if [ "${1}" == "bqlocal" ]
	then
		#Heuristica busqueda local
		#------------------------------------------------------------------------------------------------------------
			min_iteraciones=20000000
			max_iteraciones=0
			promedio_iters=0
			cant_test_files=0
			rm -rf tmp.stddev.mejora.txt				
			rm -rf tmp.stddev.iters.txt	
			for file in *.in; do
				heuristica="bqlocal"
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

			    	python ../plotter.py ../"$TESTS_OUTPUT"/"$heuristica"/"$file"_iters_w2_differential.png "evolucion_iteraciones.txt" 5
			    	python ../plotter.py ../"$TESTS_OUTPUT"/"$heuristica"/"$file"_iters_w2_absolute_value.png "costos_absolutos_iteraciones_bqlocal.txt" 5
			    	costo_w2_inicial=$(cat costos_absolutos_iteraciones_bqlocal_analisis.txt | awk -F' ' '{print $1}')
			    	costo_w2_final=$(cat costos_absolutos_iteraciones_bqlocal_analisis.txt | awk -F' ' '{print $2}')
			    	costo_w2_diferencia=$(cat costos_absolutos_iteraciones_bqlocal_analisis.txt | awk -F' ' '{print $3}')
			    	
			    	echo -e "${green}Ok! ${purple}Mejora total en w2 de $costo_w2_diferencia ${green}with $cantIters iterations in aprox. $timeElapsed micro-seconds per iteration ${NC}"
	
					linenumber=$((cant_test_files+1)) 
					echo "$linenumber $costo_w2_diferencia" >> tmp.stddev.mejora.txt
					echo "$linenumber $cantIters" >> tmp.stddev.iters.txt
			    	
			    	promedio_mejora=$(($promedio_mejora + $costo_w2_diferencia))

			    	if [ $min_iteraciones -gt $cantIters ]
					then
						min_iteraciones=$cantIters					
					fi
	
					if [ $max_iteraciones -lt $cantIters ]
					then
						max_iteraciones=$cantIters
					fi
					promedio_iters=$(($promedio_iters + $cantIters))
				fi
				rm -rf evolucion_iteraciones.txt
				rm -rf costos_absolutos_iteraciones_bqlocal.txt
				rm -rf costos_absolutos_iteraciones_bqlocal_analisis.txt
				cant_test_files=$((cant_test_files+1))
			done
			promedio_iters=$((promedio_iters / cant_test_files))
			promedio_mejora=$((promedio_mejora / cant_test_files))
			stddev_mejora=$(awk '{sum+=$2; array[NR]=$2} END {for(x=1;x<=NR;x++){sumsq+=((array[x]-(sum/NR))^2);}print sqrt(sumsq/NR)}' tmp.stddev.mejora.txt)
			stddev_iters=$(awk '{sum+=$2; array[NR]=$2} END {for(x=1;x<=NR;x++){sumsq+=((array[x]-(sum/NR))^2);}print sqrt(sumsq/NR)}' tmp.stddev.iters.txt)

			min_mejora=$(awk 'NR == 1 {max=$2 ; min=$2} $2 >= max {max = $2} $2 <= min {min = $2} END { print min }' tmp.stddev.mejora.txt)
			max_mejora=$(awk 'NR == 1 {max=$2 ; min=$2} $2 >= max {max = $2} $2 <= min {min = $2} END { print max }' tmp.stddev.mejora.txt)


			rm -rf ../resumen_busqueda_local.txt
			echo "  ---- Resultados de los experimentos (Bqlocal)----" >> ../resumen_busqueda_local.txt
			echo "Cantidad de tests realizados: $cant_test_files" >> ../resumen_busqueda_local.txt
			echo "Iteraciones promedio: $promedio_iters" >> ../resumen_busqueda_local.txt
			echo "Iteraciones stddev: $stddev_iters" >> ../resumen_busqueda_local.txt
			echo "Minima cantidad de iteraciones: $min_iteraciones" >> ../resumen_busqueda_local.txt
			echo "Maxima cantidad de iteraciones: $max_iteraciones" >> ../resumen_busqueda_local.txt
			echo "Mejora promedio del costo w2: $promedio_mejora" >> ../resumen_busqueda_local.txt
			echo "Mejora stddev del costo w2: $stddev_mejora" >> ../resumen_busqueda_local.txt
			echo "Minima mejora en w2 registrada: $min_mejora" >> ../resumen_busqueda_local.txt
			echo "Maxima mejora en w2 registrada: $max_mejora" >> ../resumen_busqueda_local.txt
			
			#imprimimos resultados
			echo -e "${purple}"			
			cat ../resumen_busqueda_local.txt
			echo -e "${NC}"
			
			popd

			#curve fit para heuristica
			fitType=2
			echo "$heuristica -> fitType=$fitType"

			./plot.sh "$heuristica".tmpplot 0 "$fitType"
			./plot.sh "$heuristica".tmpplot 1
			./plot.sh "$heuristica".tmpplot 2
			./plot.sh "$heuristica".tmpplot 3
			./plot.sh "$heuristica".tmpplot 4
		#------------------------------------------------------------------------------------------------------------	
	elif [ "${1}" == "golosa" ]
	then
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
			popd
			#curve fit para heuristica
			fitType=2
			echo "$heuristica -> fitType=$fitType"

			./plot.sh "$heuristica".tmpplot 0 "$fitType"
			./plot.sh "$heuristica".tmpplot 1
			./plot.sh "$heuristica".tmpplot 2
			./plot.sh "$heuristica".tmpplot 3
			./plot.sh "$heuristica".tmpplot 4		
		#------------------------------------------------------------------------------------------------------------	
	elif [ "${1}" == "grasp" ]
	then
		#Metaheuristica grasp
		#------------------------------------------------------------------------------------------------------------
			min_iteraciones=20000000
			max_iteraciones=0
			promedio_iters=0
			cant_test_files=0
			for file in *.in; do
				heuristica="grasp"
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
	
			    	python ../plotter.py ../"$TESTS_OUTPUT"/"$heuristica"/"$file"_iters.png "evolucion_iteraciones_grasp.txt" 5
	
			    	if [ $min_iteraciones -gt $cantIters ]
					then
						min_iteraciones=$cantIters
					fi
	
					if [ $max_iteraciones -lt $cantIters ]
					then
						max_iteraciones=$cantIters					
					fi
					promedio_iters=$(($promedio_iters + $cantIters))
				fi
				rm -rf evolucion_iteraciones_grasp.txt
				cant_test_files=$((cant_test_files+1))
			done
			promedio_iters=$((promedio_iters / cant_test_files))
			rm -rf ../resumen_grasp.txt
			echo "Cantidad de tests realizados: $cant_test_files" >> ../resumen_grasp.txt
			echo "Iteraciones promedio: $promedio_iters" >> ../resumen_grasp.txt
			echo "Minima cantidad de iteraciones: $min_iteraciones" >> ../resumen_grasp.txt
			echo "Maxima cantidad de iteraciones: $max_iteraciones" >> ../resumen_grasp.txt
			popd
			#curve fit para heuristica
			fitType=2
			echo "$heuristica -> fitType=$fitType"

			./plot.sh "$heuristica".tmpplot 0 "$fitType"
			./plot.sh "$heuristica".tmpplot 1
			./plot.sh "$heuristica".tmpplot 2
			./plot.sh "$heuristica".tmpplot 3
			./plot.sh "$heuristica".tmpplot 4		
		#------------------------------------------------------------------------------------------------------------	
	elif [ "${1}" == "exacta" ]
	then 
		#Algoritmo exacto
		#------------------------------------------------------------------------------------------------------------
			for file in *.in; do
				heuristica="exacta"
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
			popd
			#curve fit para exacta
			fitType=$((seq -s "*" 1 "$cantNodos" |bc))
			echo "$heuristica -> fitType=$fitType"

			./plot.sh "$heuristica".tmpplot 0 "$fitType"
			./plot.sh "$heuristica".tmpplot 1
			./plot.sh "$heuristica".tmpplot 2
			./plot.sh "$heuristica".tmpplot 3
			./plot.sh "$heuristica".tmpplot 4		
		#------------------------------------------------------------------------------------------------------------	
	else
		echo "Modo invalido de ejecucion: ./run_performance_tests.sh <algoritmo> donde las opciones para algoritmo son: bqlocal - golosa - grasp"
		popd
	fi
else
    echo "[WARN] NO existen archivos de testing"
fi
rm -f no.txt