import numpy as np
import matplotlib.pyplot as plt
import sys
import math

#from mpl_toolkits.mplot3d import Axes3D
#from matplotlib import cm
#from matplotlib.ticker import LinearLocator, FormatStrFormatter
#import numpy as np

#parameters validation
if(len(sys.argv) < 3):
    print "Use with parameters: plotter.py <outputPNGPath> <inputDataFile of pairs (x, y, z = f(x,y)) separated by space> <graphic_type_number 0=(x,fx) 1=(x,fx/x**2) 2=(x,fx/x**3)>"
    sys.exit(1); 

#read input data from file
outputFilePath = str(sys.argv[1])
dataFile = open(sys.argv[2], 'r')
graphictype = int(sys.argv[3])

#transform input data into 2 arrays representing the points in the cartesian plane.
graphic_x = []
graphic_x_med = []
graphic_fx = []
graphic_fx_med = []
graphic_fx_fact = []
graphic_fx_fact_med = []
graphic_fx_divided_ncube = []
graphic_fx_divided_nsquare = []
i = 0
last_x = -1
fx_sum = 0
fx_fact_sum = 0
for line in dataFile:
	fx = float(line.split()[0])
	x = float(line.split()[1])
	if(x != last_x):
		if(last_x != -1):
			graphic_fx_med.append(fx_sum / i)
			graphic_fx_fact_med.append(fx_fact_sum / i)
			graphic_x_med.append(last_x)
			fx_sum = 0
			fx_fact_sum = 0
			i = 0
	last_x = x
	i = i + 1
	fx_sum = fx_sum + fx
	fx_fact_sum = fx_fact_sum + float(fx / (math.factorial(x) / 1))
	graphic_x.append(x)
	graphic_fx.append(fx)
	graphic_fx_fact.append(float(fx / (math.factorial(x) / 1)))
	graphic_fx_divided_nsquare.append(float(fx / math.pow(x, 2)))
	graphic_fx_divided_ncube.append(float(fx / math.pow(x, 3)))

graphic_fx_med.append(fx_sum / i)
graphic_fx_fact_med.append(fx_fact_sum / i)
graphic_x_med.append(last_x)
#plot the data
if(graphictype == 0):
	#plt.plot(graphic_x, graphic_fx, 'b-', graphic_x_for_logx, graphic_fx_over_logx, 'r-', graphic_x, graphic_fx_over_x, 'g-')
	plt.plot(graphic_x, graphic_fx, 'bo', graphic_x_med, graphic_fx_med, 'r-', label="hoal")
	plt.xlabel('x = cantidad de nodos en un grafo completo', fontsize=14)
	plt.ylabel('f(x) = Microsegundos transcurridos', fontsize=14)
	plt.title("(x, f(x))")
	plt.grid()
	plt.show()
#plt.savefig(outputFilePath + "fx.png")
else: 
	if(graphictype == 1):
		plt.plot(graphic_x, graphic_fx_divided_nsquare, 'b-')
		plt.xlabel('x = Dimension de la matriz', fontsize=14)
		plt.ylabel('f(x) = Microsegundos transcurridos', fontsize=14)
		plt.title("(x, f(x)/pow(n, 2))")
		#plt.show()
		plt.savefig(outputFilePath + "over_nsquare.png")
	else: 
		if(graphictype == 2):
			plt.plot(graphic_x, graphic_fx_divided_ncube, 'b-')
			plt.xlabel('x = Dimension de la matriz', fontsize=14)
			plt.ylabel('f(x) = Microsegundos transcurridos', fontsize=14)
			plt.title("(x, f(x)/pow(n, 3))")
			#plt.show()
			plt.savefig(outputFilePath + "over_ncube.png")
		else:
			if(graphictype == 3):
				plt.plot(graphic_x, graphic_fx_fact, 'bo', graphic_x_med, graphic_fx_fact_med, 'r-', label="hoal")
				plt.xlabel('x = cantidad de nodos en un grafo completo', fontsize=14)
				plt.ylabel('f(x)/x!, f(x) = Microsegundos transcurridos', fontsize=14)
				plt.title("(x, f(x)/x!)")
				plt.grid()
				plt.show()
			else:
				if(graphictype == 5):
					xData = ["Mejor caso", "Caso promedio", "Peor caso"]
					fig = plt.figure()
					width = .35
					ind = np.arange(len(graphic_fx))
					plt.bar(ind, graphic_fx)
					plt.xticks(ind + width / 2, xData)

					plt.xlabel('Casos', fontsize=14)
					plt.ylabel('Microsegundos transcurridos', fontsize=14)
					plt.title("Tabla comparativa de casos")

					fig.autofmt_xdate()

					plt.savefig(outputFilePath + "histogram.png")
				
