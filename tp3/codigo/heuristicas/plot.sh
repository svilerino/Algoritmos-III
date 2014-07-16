#!/bin/bash
if ls *.tmpplot &> /dev/null; then
	for file in *.tmpplot; do		
		echo "Procesando $file..."
		#ordeno plot por la primer columna de numeros 
		sort "$file" -k1,1 --numeric-sort > plot_"$file".out

		echo -n "    * Graficando $file""_complexity_variation.png..."
		python plotter.py "$file"_complexity_variation.png plot_"$file".out 0
		echo "Ok"
		echo -n "    * Graficando $file""_complexity_med_over_n.png..."
		python plotter.py "$file"_complexity_med_over_n.png plot_"$file".out 1
		echo "Ok"
		echo -n "    * Graficando $file""_complexity_med_over_n_square.png..."
		python plotter.py "$file"_complexity_med_over_n_square.png plot_"$file".out 2
		echo "Ok"
		echo -n "    * Graficando $file""_complexity_med_over_n_cube.png..."
		python plotter.py "$file"_complexity_med_over_n_cube.png plot_"$file".out 3
		echo "Ok"
		echo -n "    * Graficando $file""_complexity_med_over_n_fourth.png..."
		python plotter.py "$file"_complexity_med_over_n_fourth.png plot_"$file".out 4
		echo "Ok"
		rm -f plot_"$file".out
	done
else
	echo "[WARN] NO existen archivos de datos para graficacion"
fi