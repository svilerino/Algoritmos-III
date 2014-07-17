import numpy as np
import matplotlib.pyplot as plt
import sys
import math

#parameters validation
if(len(sys.argv) < 3):
    print "Use with parameters: plotter.py <outputPNGPath> <inputDataFile of lines x y f(x) separated by space> <graphic_type_number 0=(x,fx) 1=(x,fx/x**2) 2=(x,fx/x**3)>"
    sys.exit(1); 

#read input data from file
outputFilePath = str(sys.argv[1])
dataFile = open(sys.argv[2], 'r')
graphictype = int(sys.argv[3])


if(graphictype <=4):
	#transform input data into 2 arrays representing the points in the cartesian plane.
	#para cada valor de x, en y se ponen puntitos para diferentes densidades de aristas
	graphic_x = []
	graphic_y = []

	#para cada valor de x, en y se pone el promedio de los puntitos con una curva
	graphic_med_x = []
	graphic_med_y = []
	graphic_med_y_over_n = []
	graphic_med_y_over_n_square = []
	graphic_med_y_over_n_cube = []
	graphic_med_y_over_n_fourth = []

	cant_densidades_por_cant_nodos = 0
	ultima_cant_nodos = -1
	acum = 0

	for line in dataFile:
		#parse line data
		nodos = float(line.split()[0])
		aristas = float(line.split()[1])
		tiempo = float(line.split()[2])
		if(ultima_cant_nodos != nodos):
			if(ultima_cant_nodos != -1):						
				graphic_med_x.append(ultima_cant_nodos)
				value = acum / float(cant_densidades_por_cant_nodos)
				graphic_med_y.append(value)
				graphic_med_y_over_n.append(float(value / ultima_cant_nodos))
				graphic_med_y_over_n_square.append(float(value / math.pow(ultima_cant_nodos, 2)))
				graphic_med_y_over_n_cube.append(float(value / math.pow(ultima_cant_nodos, 3)))
				graphic_med_y_over_n_fourth.append(float(value / math.pow(ultima_cant_nodos, 4)))
				acum = 0
				cant_densidades_por_cant_nodos = 0
		
		ultima_cant_nodos = nodos
		cant_densidades_por_cant_nodos = cant_densidades_por_cant_nodos + 1
		acum = acum + tiempo

		#append to the graphic vectors
		graphic_x.append(nodos)
		graphic_y.append(tiempo)#asociado tambien a la cantidad de aristas en la variable aristas

	#el ultimo valor de cant_nodos no va a entrar al if de ultima_cant_nodos
	graphic_med_x.append(ultima_cant_nodos)
	value = acum / float(cant_densidades_por_cant_nodos)
	graphic_med_y.append(value)
	graphic_med_y_over_n.append(float(value / ultima_cant_nodos))
	graphic_med_y_over_n_square.append(float(value / math.pow(ultima_cant_nodos, 2)))
	graphic_med_y_over_n_cube.append(float(value / math.pow(ultima_cant_nodos, 3)))
	graphic_med_y_over_n_fourth.append(float(value / math.pow(ultima_cant_nodos, 4)))

	#plot the data
	if(graphictype == 0):
		plt.plot(graphic_med_x, graphic_med_y, 'g-', graphic_med_x, graphic_med_y, 'go', graphic_x, graphic_y, 'ro', label="")	
		plt.xlabel('x = Cantidad de nodos', fontsize=14)
		plt.ylabel('f(x) = Verde: Tiempo consumido en promedio para densidades de este orden\nRojo: Variacion de tiempos para las densidades', fontsize=14)
		plt.title("Rendimiento")
		#plt.yscale('log')
		plt.grid()
		#plt.show()
		plt.savefig(outputFilePath)
	else:
		if(graphictype == 1):
			plt.plot(graphic_med_x, graphic_med_y_over_n, 'g-', label="")	
			plt.xlabel('x = Cantidad de nodos', fontsize=14)
			plt.ylabel('f(x)/x', fontsize=14)
			plt.title("Rendimiento")
			#plt.yscale('log')
			plt.grid()
			#plt.show()
			plt.savefig(outputFilePath)
		else:
			if(graphictype == 2):
				plt.plot(graphic_med_x, graphic_med_y_over_n_square, 'g-', label="")	
				plt.xlabel('x = Cantidad de nodos', fontsize=14)
				plt.ylabel('f(x)/(n^2)', fontsize=14)
				plt.title("Rendimiento")
				#plt.yscale('log')
				plt.grid()
				#plt.show()
				plt.savefig(outputFilePath)
			else:
				if(graphictype == 3):
					plt.plot(graphic_med_x, graphic_med_y_over_n_cube, 'g-', label="")	
					plt.xlabel('x = Cantidad de nodos', fontsize=14)
					plt.ylabel('f(x)/(n^3)', fontsize=14)
					plt.title("Rendimiento")
					#plt.yscale('log')
					plt.grid()
					#plt.show()
					plt.savefig(outputFilePath)
				else:
					if(graphictype == 4):
						plt.plot(graphic_med_x, graphic_med_y_over_n_fourth, 'g-', label="")	
						plt.xlabel('x = Cantidad de nodos', fontsize=14)
						plt.ylabel('f(x)/(n^4)', fontsize=14)
						plt.title("Rendimiento")
						#plt.yscale('log')
						plt.grid()
						#plt.show()
						plt.savefig(outputFilePath)
else:
	if(graphictype == 5):
		#evolucion iters (x, fx)
		graphic_x = []
		graphic_fx = []
		for line in dataFile:
			#parse line data
			iteracion = int(line.split()[0])
			mejora_parcial = int(line.split()[1])
			graphic_x.append(iteracion)
			graphic_fx.append(mejora_parcial)			

		plt.plot(graphic_x, graphic_fx, 'g-', graphic_x, graphic_fx, 'go', label="")	
		plt.xlabel('x = Numero de iteracion', fontsize=14)
		plt.ylabel('Mejora en dicha iteracion', fontsize=14)
		plt.title("Rendimiento evolutivo de busqueda local")
		#plt.yscale('log')
		plt.grid()
		#plt.show()
		#print outputFilePath
		plt.savefig(outputFilePath)