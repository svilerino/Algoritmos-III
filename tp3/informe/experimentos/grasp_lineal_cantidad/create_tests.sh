#!/bin/bash
echo "-------------------------------------------------------------------------------------------------------------------------"
echo "Compilando generador de tests-aleatorios..."
pushd	../generar_grafos/
	make clean all
popd
echo ""
echo "Creando tests aleatorios..."
echo "-------------------------------------------------------------------------------------------------------------------------"
red='\e[0;31m'
green='\e[0;32m'
NC='\e[0m' # No Color

cant_nodos_min=100
cant_nodos_max=600
peso_maximo_w1=250
peso_maximo_w2=400
limit_w1=200

for (( n=$cant_nodos_min; n<=$cant_nodos_max; n+=40 ))
do 
	#seleccionar aca abajo una combinacion de aristas... para establecer un valor fijo hay que poner max y min iguales.
	#aristas_minimas=$((n * (n-1)/3))
	#aristas_maximas=$((n * (n-1)/2))
	
	aristas_minimas=$((10*n))
	aristas_maximas=$((15*n))
	
	#aristas_minimas=$((n * (n-1)/10))
	#aristas_maximas=$((n * (n-1)/7))

	#aristas_minimas=$(echo "scale=0; $n*sqrt($n)" | bc -l )
	#aristas_maximas=$(echo "scale=0; 5*$n*sqrt($n)" | bc -l )

	echo "Generando grafos de $n nodos con cantidad de aristas aleatorias en el rango [$aristas_minimas .. $aristas_maximas]"
	echo "Los pesos W1 de las aristas van a estar en un rango preestablecido aleatorio entre [1..$peso_maximo_w1]"
	echo "Los pesos W2 de las aristas van a estar en un rango preestablecido aleatorio entre [1..$peso_maximo_w2]"
	for (( a=$aristas_minimas; a<=$aristas_maximas; a+=500 ))
	do
		echo -n "Generando grafo de $n nodos y $a aristas aleatorio..."		
		#../generar_grafos/generar_grafos "$n" "$a" "$peso_maximo_w1" "$peso_maximo_w2" "$limit_w1" > "test-cases/instancia_$n.$a.in"
		./generador_fisher_gates "$n" "$a" "$peso_minimo_w1" "$peso_maximo_w1" "$peso_minimo_w2" "$peso_maximo_w2" "$limit_w1" > "test-cases/instancia_$n.$a.in"
		echo "Ok!"
	done
	echo "-------------------------------------------------------------------------------------------------------------------------"
done
echo "-------------------------------------------------------------------------------------------------------------------------"
pushd	../generar_grafos/
	make clean
popd