import matplotlib.pyplot as plt
import fileinput
import sys
from math import log
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import numpy as np

#parameters validation
if(len(sys.argv) < 3):
    print "Use with parameters: plotter.py <outputPNGPath> <inputDataFile of pairs (x, f(x)) separated by space>"
    sys.exit(1); 

#read input data from file
outputFilePath = str(sys.argv[1])
dataFile = open(sys.argv[2], 'r')


if(outputFilePath != "ej3_plot"):
	#transform input data in format <input size> <elapsed time in microseconds> <repetitions_for_average>
	#into 2 arrays representing the points in the cartesian plane.
	graphic_x = []
	graphic_fx = []
	graphic_fx_over_x = []
	graphic_x_for_logx = []
	graphic_fx_over_logx = []
	for line in dataFile:
		x = float(line.split()[0])
		fx = float(line.split()[1])
		graphic_x.append(x)
		graphic_fx.append(fx)
		fx_over_x = x/float(x)
		graphic_fx_over_x.append(fx_over_x)
		if(log(float(x)) != 0):
			fx_over_logx = x/log(float(x))
			graphic_x_for_logx.append(x)
			graphic_fx_over_logx.append(fx_over_logx)

	#plot the data
	plt.plot(graphic_x, graphic_fx, 'b-', graphic_x_for_logx, graphic_fx_over_logx, 'r-', graphic_x, graphic_fx_over_x, 'g-')
	plt.xlabel('Input size', fontsize=16)
	plt.ylabel('Microsegundos tomados', fontsize=16)
	plt.title("Magnitud de la entrada vs tiempo de ejecucion(microseg)")
	#plt.show()
	plt.savefig(outputFilePath + ".png")
else:
	X = []
	Y = []
	Z = []
	for line in dataFile:
		point_x = float(line.split()[0])
		point_y = float(line.split()[1])
		point_z = float(line.split()[2])
		X.append(point_x)
		Y.append(point_y)
		Z.append(point_z)
	print X
	print Y
	print Z
	#3d plotting

	fig = plt.figure()
	ax = fig.gca(projection='3d')
	X, Y = np.meshgrid(X, Y)
	surf = ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap=cm.coolwarm,
	        linewidth=0, antialiased=True)
	ax.zaxis.set_major_locator(LinearLocator(10))
	ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))

	ax.view_init(25, 70)

	fig.tight_layout()

	fig.colorbar(surf, shrink=0.5, aspect=5)

	#plt.show()
	plt.savefig(outputFilePath + ".png")