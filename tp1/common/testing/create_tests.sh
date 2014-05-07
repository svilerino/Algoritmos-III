#!/bin/bash
echo ""
echo "Creando tests ejercicio 1"
red='\e[0;31m'
green='\e[0;32m'
NC='\e[0m' # No Color
for i in {1..200}
do
#	num=$(($i + 5))
#	echo $num
	if [ -f ../testcases/ejercicio1/tests-"$i".ej1 ]
	then
	    echo -e "${red}[EJ1]Ya existe tests-$i! Skipping...${NC}"
	else 
		dias_inspector=$((100*$i))
		cantidad_camiones=$((500*$i))
		max_random=$(($cantidad_camiones * 3))
		echo -e "${green}[EJ1]Creando tests-$i: cantidad_camiones: $cantidad_camiones | dias_inspector: $dias_inspector | max-random: $max_random ${NC}"
		../random_testcase_builder/crear_casos --ejercicio=1 --salida=../testcases/ejercicio1/tests-"$i".ej1 --variable-1="$dias_inspector" --variable-2="$cantidad_camiones" --maximo-random="$max_random"
	fi
done

for i in {1..200}
do
	if [ -f ../testcases/ejercicio2/tests-"$i".ej2 ]
	then
	    echo -e "${red}[EJ2]Ya existe tests-$i! Skipping...${NC}"
	else 
		cant_joyas=$(($i*250 + 10))
		max_random=$(($i + 20))
		echo -e "${green}[EJ2]Creando tests-$i: cant_joyas: $cant_joyas | max_random: $max_random ${NC}"
		../random_testcase_builder/crear_casos --ejercicio=2 --salida=../testcases/ejercicio2/tests-"$i".ej2 --variable-1="$cant_joyas" --maximo-random="$max_random"
	fi
done

#matrices cuadradas
for i in {1..3}
do
	if [ -f ../testcases/ejercicio3/tests-"$i".ej3 ]
	then
	    echo -e "${red}[EJ3]Ya existe tests-$i! Skipping...${NC}"
	else 
		mat_cant_colores=$(($i + 3))
		mat_n=$(($i + 1))
		mat_m=$(($i + 1))
		echo -e "${green}[EJ3]Creando tests-$i: mat_n: $mat_n | mat_m: $mat_m | mat_cant_colores: $mat_cant_colores ${NC}"
		../random_testcase_builder/crear_casos --ejercicio=3 --salida=../testcases/ejercicio3/tests-"$i".ej3 --variable-1="$mat_n" --variable-2="$mat_m" --variable-3="$mat_cant_colores"
	fi
done

#matrices NO cuadradas
for i in {1..3}
do
	if [ -f ../testcases/ejercicio3/tests_nocuad-"$i".ej3 ]
	then
	    echo -e "${red}[EJ3]Ya existe tests-$i! Skipping...${NC}"
	else 
		if [ $(( $i % 2 )) -eq 0 ] ; then
			mat_n=$(($i + 1))
			mat_m=$(($i))
			mat_cant_colores=$(($i + 1))
		else
			mat_n=$(($i))
			mat_m=$(($i + 1))
			mat_cant_colores=$(($i + 1))
		fi		
		echo -e "${green}[EJ3]Creando tests_nocuad-$i: mat_n: $mat_n | mat_m: $mat_m | mat_cant_colores: $mat_cant_colores ${NC}"
		../random_testcase_builder/crear_casos --ejercicio=3 --salida=../testcases/ejercicio3/tests_nocuad-"$i".ej3 --variable-1="$mat_n" --variable-2="$mat_m" --variable-3="$mat_cant_colores"
	fi
done
