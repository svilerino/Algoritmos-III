#!/bin/bash
file=${1}
tipoGrafico=${2}
fit_type=${3}
if [ -e $file ]; then
	echo "Procesando $file para tipo de grafico $tipoGrafico..."
	#ordeno plot por la primer columna de numeros 
	sort "$file" -k1,1 --numeric-sort > plot_"$file".out
	if [ $tipoGrafico -eq 0 ]
	then	
		echo -n "    * Graficando $file""_complexity_variation.png con fitting de orden $fit_type..."
		python plotter.py "$file"_complexity_variation.png plot_"$file".out 0 "$fit_type"
		echo "Ok"
	fi

	if [ $tipoGrafico -eq 1 ]
	then	
		echo -n "    * Graficando $file""_complexity_med_over_n.png..."
		python plotter.py "$file"_complexity_med_over_n.png plot_"$file".out 1
		echo "Ok"
	fi

	if [ $tipoGrafico -eq 2 ]
	then	
		echo -n "    * Graficando $file""_complexity_med_over_n_square.png..."
		python plotter.py "$file"_complexity_med_over_n_square.png plot_"$file".out 2
		echo "Ok"
	fi

	if [ $tipoGrafico -eq 3 ]
	then	
		echo -n "    * Graficando $file""_complexity_med_over_n_cube.png..."
		python plotter.py "$file"_complexity_med_over_n_cube.png plot_"$file".out 3
		echo "Ok"
	fi

	if [ $tipoGrafico -eq 4 ]
	then	
		echo -n "    * Graficando $file""_complexity_med_over_n_fourth.png..."
		python plotter.py "$file"_complexity_med_over_n_fourth.png plot_"$file".out 4
		echo "Ok"
	fi

	rm -f plot_"$file".out
else
	echo "[WARN] NO existe el archivo de datos para graficacion $file"
fi