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
	rm -rf diff_exacto_golosa.txt
	rm -rf diff_exacto_bqlocal.txt
	rm -rf diff_exacto_grasp.txt
	pushd test-cases
	#------------------------------------------------------------------------------------------------------------
	#seleccionar aca abajo en el for heuristica que heuristicas se quieren comparar entre si
	diffnumber=0
	testsnumber=0
	diff_exacto_golosa=0
	diff_exacto_bqlocal=0
	diff_exacto_grasp=0

	exacta_golosa_match_number=0
	exacta_bqlocal_match_number=0
	exacta_grasp_match_number=0

	rm -rf stddev.tmp.golosa.txt
	rm -rf stddev.tmp.bqlocal.txt
	rm -rf stddev.tmp.grasp.txt
	for file in *.in; do
		for heuristica in "bqlocal" "golosa" "grasp" "exacta"; do
			if [ "$heuristica" == "exacta" ]
			then
				if [ "${1}" != "--use-exacta" ]
				then
					#ignoro la corrida de exacta salvo que me pasen el parametro
					echo "Ignorando exacta para archivo de entrada $file"
					continue
				fi
			fi			
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

				if [ $pesow1 -gt $limitw1 ]
				then
					echo -e "${purple}Camino obtenido con peso $pesow1 pero excede el limite $limitw1."
					echo -e -n "${NC}"
					hay_solucion="no"
				else
				    echo "$cantNodos $cantAristas $pesow2 $heuristica " >> ../"comparacion_optimalidad_$heuristica".tmpplot
			    	echo -e "${green}Ok! Camino obtenido entre ($nodosrc) y ($nododst) de longuitud ${blue}$longuitudCaminoOutput${green} con peso w2: ${blue}$pesow2${green} (${red}peso w1: $pesow1 | limit w1: $limitw1${green}) in $cantIters iterations ${NC}"
			    	#cat "../$TESTS_OUTPUT/$heuristica/$file.out"
			    	#echo ""			    	
			    	echo "$pesow2" > tmp."$heuristica".optimalidad.actual.txt
			    	hay_solucion="si"
				fi
			fi
		done
		if [ "${1}" == "--use-exacta" ] && [ $hay_solucion == "si" ]
		then
			ultimo_peso_w2_exacta=$(head -1 tmp.exacta.optimalidad.actual.txt | awk -F' ' '{print $1}')
			ultimo_peso_w2_golosa=$(head -1 tmp.golosa.optimalidad.actual.txt | awk -F' ' '{print $1}')
			ultimo_peso_w2_bqlocal=$(head -1 tmp.bqlocal.optimalidad.actual.txt | awk -F' ' '{print $1}')
			ultimo_peso_w2_grasp=$(head -1 tmp.grasp.optimalidad.actual.txt | awk -F' ' '{print $1}')
			
			if [ $ultimo_peso_w2_exacta != 0 ]
			then
								
				diff_actual_exacto_golosa=$(echo "scale=3; 100 * (($ultimo_peso_w2_golosa/$ultimo_peso_w2_exacta) - 1)" | bc -l )				
				diff_actual_exacto_bqlocal=$(echo "scale=3; 100 * (($ultimo_peso_w2_bqlocal/$ultimo_peso_w2_exacta) - 1)" | bc -l )				
				diff_actual_exacto_grasp=$(echo "scale=3; 100 * (($ultimo_peso_w2_grasp/$ultimo_peso_w2_exacta) - 1)" | bc -l )				
				diffnumber=$(($diffnumber+1))
			else
				diff_actual_exacto_golosa=$(echo "scale=3; 100 * $ultimo_peso_w2_golosa" | bc -l )
				diff_actual_exacto_bqlocal=$(echo "scale=3; 100 * $ultimo_peso_w2_bqlocal" | bc -l )
				diff_actual_exacto_grasp=$(echo "scale=3; 100 * $ultimo_peso_w2_grasp" | bc -l )
			fi
			
			echo "$diffnumber $diff_actual_exacto_golosa" >> stddev.tmp.golosa.txt
			echo "$diffnumber $diff_actual_exacto_bqlocal" >> stddev.tmp.bqlocal.txt
			echo "$diffnumber $diff_actual_exacto_grasp" >> stddev.tmp.grasp.txt
			
			diff_exacto_golosa=$(echo "scale=3; $diff_exacto_golosa + $diff_actual_exacto_golosa" | bc -l )				
			diff_exacto_bqlocal=$(echo "scale=3; $diff_exacto_bqlocal + $diff_actual_exacto_bqlocal" | bc -l )				
			diff_exacto_grasp=$(echo "scale=3; $diff_exacto_grasp + $diff_actual_exacto_grasp" | bc -l )			

			res=$(echo $diff_actual_exacto_golosa'=='0 | bc -l)
			if [ $res -eq 1 ]
			then
				exacta_golosa_match_number=$(($exacta_golosa_match_number + 1))
			fi

			res=$(echo $diff_actual_exacto_bqlocal'=='0 | bc -l)
			if [ $res -eq 1 ]
			then					
				exacta_bqlocal_match_number=$(($exacta_bqlocal_match_number + 1))
			fi

			res=$(echo $diff_actual_exacto_grasp'=='0 | bc -l)
			if [ $res -eq 1 ]
			then
				exacta_grasp_match_number=$(($exacta_grasp_match_number + 1))
			fi
			testsnumber=$(($testsnumber+1))			
		fi
		echo "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"
	done

	exacta_golosa_match_number=$(echo "scale=3; 100*$exacta_golosa_match_number/$testsnumber" | bc -l )
	exacta_bqlocal_match_number=$(echo "scale=3; 100*$exacta_bqlocal_match_number/$testsnumber" | bc -l )
	exacta_grasp_match_number=$(echo "scale=3; 100*$exacta_grasp_match_number/$testsnumber" | bc -l )

	diff_exacto_golosa=$(echo "scale=3; $diff_exacto_golosa/$diffnumber" | bc -l )
	diff_exacto_bqlocal=$(echo "scale=3; $diff_exacto_bqlocal/$diffnumber" | bc -l )
	diff_exacto_grasp=$(echo "scale=3; $diff_exacto_grasp/$diffnumber" | bc -l )

	stddev_diff_exacto_golosa=$(awk '{sum+=$2; array[NR]=$2} END {for(x=1;x<=NR;x++){sumsq+=((array[x]-(sum/NR))^2);}print sqrt(sumsq/NR)}' stddev.tmp.golosa.txt)
	stddev_diff_exacto_bqlocal=$(awk '{sum+=$2; array[NR]=$2} END {for(x=1;x<=NR;x++){sumsq+=((array[x]-(sum/NR))^2);}print sqrt(sumsq/NR)}' stddev.tmp.bqlocal.txt)
	stddev_diff_exacto_grasp=$(awk '{sum+=$2; array[NR]=$2} END {for(x=1;x<=NR;x++){sumsq+=((array[x]-(sum/NR))^2);}print sqrt(sumsq/NR)}' stddev.tmp.grasp.txt)
	
	min_alejamiento_exacto_golosa=$(awk 'NR == 1 {max=$2 ; min=$2} $2 >= max {max=$2} $2 <= min {min=$2} END { print min }' stddev.tmp.golosa.txt)
	max_alejamiento_exacto_golosa=$(awk 'NR == 1 {max=$2 ; min=$2} $2 >= max {max=$2} $2 <= min {min=$2} END { print max }' stddev.tmp.golosa.txt)

	min_alejamiento_exacto_bqlocal=$(awk 'NR == 1 {max=$2 ; min=$2} $2 >= max {max=$2} $2 <= min {min=$2} END { print min }' stddev.tmp.bqlocal.txt)
	max_alejamiento_exacto_bqlocal=$(awk 'NR == 1 {max=$2 ; min=$2} $2 >= max {max=$2} $2 <= min {min=$2} END { print max }' stddev.tmp.bqlocal.txt)

	min_alejamiento_exacto_grasp=$(awk 'NR == 1 {max=$2 ; min=$2} $2 >= max {max=$2} $2 <= min {min=$2} END { print min }' stddev.tmp.grasp.txt)
	max_alejamiento_exacto_grasp=$(awk 'NR == 1 {max=$2 ; min=$2} $2 >= max {max=$2} $2 <= min {min=$2} END { print max }' stddev.tmp.grasp.txt)

	echo "Cantidad de tests realizados: $testsnumber" >> ../diff_exacto_golosa.txt
	echo "Porcentaje de aciertos(cantidad de veces que GOLOSA da la sol exacta/cantidad de tests hechos): $exacta_golosa_match_number" >> ../diff_exacto_golosa.txt
	echo "Porcentaje de alejamiento de la heuristica a la solucion exacta promedio entre golosa y exacta: $diff_exacto_golosa" >> ../diff_exacto_golosa.txt	
	echo "Desviacion estandar del alejamiento de la heuristica a la solucion exacta promedio entre golosa y exacta: $stddev_diff_exacto_golosa" >> ../diff_exacto_golosa.txt	
	echo "Minimo alejamiento porcentual entre golosa y exacta: $min_alejamiento_exacto_golosa" >> ../diff_exacto_golosa.txt
	echo "Maximo alejamiento porcentual entre golosa y exacta: $max_alejamiento_exacto_golosa" >> ../diff_exacto_golosa.txt

	echo "Cantidad de tests realizados: $testsnumber" >> ../diff_exacto_bqlocal.txt
	echo "Porcentaje de aciertos(cantidad de veces que BQLOCAL da la sol exacta/cantidad de tests hechos): $exacta_bqlocal_match_number" >> ../diff_exacto_bqlocal.txt
	echo "Porcentaje de alejamiento de la heuristica a la solucion exacta promedio entre bqlocal y exacta: $diff_exacto_bqlocal" >> ../diff_exacto_bqlocal.txt	
	echo "Desviacion estandar del alejamiento de la heuristica a la solucion exacta promedio entre bqlocal y exacta: $stddev_diff_exacto_bqlocal" >> ../diff_exacto_bqlocal.txt	
	echo "Minimo alejamiento porcentual entre bqlocal y exacta: $min_alejamiento_exacto_bqlocal" >> ../diff_exacto_bqlocal.txt
	echo "Maximo alejamiento porcentual entre bqlocal y exacta: $max_alejamiento_exacto_bqlocal" >> ../diff_exacto_bqlocal.txt
	
	echo "Cantidad de tests realizados: $testsnumber" >> ../diff_exacto_grasp.txt
	echo "Porcentaje de aciertos(cantidad de veces que GRASP da la sol exacta/cantidad de tests hechos): $exacta_grasp_match_number" >> ../diff_exacto_grasp.txt
	echo "Porcentaje de alejamiento de la heuristica a la solucion exacta promedio entre grasp y exacta: $diff_exacto_grasp" >> ../diff_exacto_grasp.txt	
	echo "Desviacion estandar del alejamiento de la heuristica a la solucion exacta promedio entre grasp y exacta: $stddev_diff_exacto_grasp" >> ../diff_exacto_grasp.txt	
	echo "Minimo alejamiento porcentual entre grasp y exacta: $min_alejamiento_exacto_grasp" >> ../diff_exacto_grasp.txt
	echo "Maximo alejamiento porcentual entre grasp y exacta: $max_alejamiento_exacto_grasp" >> ../diff_exacto_grasp.txt
	
	echo -e "${green}"		
	echo "Resultados:"	
	echo "----------------------------------------------------------------------------------------------"
	cat ../diff_exacto_golosa.txt	
	echo "----------------------------------------------------------------------------------------------"
	cat ../diff_exacto_bqlocal.txt	
	echo "----------------------------------------------------------------------------------------------"
	cat ../diff_exacto_grasp.txt	
	echo -e "${NC}"

	#estos archivos se crean en las ejecuciones de grasp y bqlocal. los borro
	rm -rf evolucion_iteraciones.txt
	rm -rf evolucion_iteraciones_grasp.txt
	rm -rf tmp.grasp.optimalidad.actual.txt
	rm -rf tmp.golosa.optimalidad.actual.txt
	rm -rf tmp.bqlocal.optimalidad.actual.txt
	rm -rf tmp.exacta.optimalidad.actual.txt
	popd
	if [ "${1}" != "--use-exacta" ]
	then
		python plotter.py comparacion_optimalidad.png "comparacion_optimalidad_golosa".tmpplot 6 "comparacion_optimalidad_bqlocal".tmpplot "comparacion_optimalidad_grasp".tmpplot
	else
		python plotter.py comparacion_optimalidad.png "comparacion_optimalidad_golosa".tmpplot 7 "comparacion_optimalidad_bqlocal".tmpplot "comparacion_optimalidad_grasp".tmpplot "comparacion_optimalidad_exacta".tmpplot
	fi
	#------------------------------------------------------------------------------------------------------------	
else
    echo "[WARN] NO existen archivos de testing"
fi
rm -f no.txt