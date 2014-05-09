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
graphic_fx = []
graphic_fx_divided_ncube = []
graphic_fx_divided_nsquare = []
for line in dataFile:
	x = float(line.split()[0])
	fx = float(line.split()[1])
	graphic_x.append(x)
	graphic_fx.append(fx)
	graphic_fx_divided_nsquare.append(float(fx / math.pow(x, 2)))
	graphic_fx_divided_ncube.append(float(fx / math.pow(x, 3)))

#plot the data
if(graphictype == 0):
	#plt.plot(graphic_x, graphic_fx, 'b-', graphic_x_for_logx, graphic_fx_over_logx, 'r-', graphic_x, graphic_fx_over_x, 'g-')
	plt.plot(graphic_x, graphic_fx, 'b-', label="hoal")
	plt.xlabel('x = Cantidad de cartas', fontsize=14)
	plt.ylabel('f(x) = Microsegundos transcurridos', fontsize=14)
	plt.title("(x, f(x))")
	#plt.show()
	plt.savefig(outputFilePath + ".png")
else: 
	if(graphictype == 1):
		plt.plot(graphic_x, graphic_fx_divided_nsquare, 'b-')
		plt.xlabel('x = Cantidad de cartas', fontsize=14)
		plt.ylabel('f(x) = Microsegundos transcurridos', fontsize=14)
		plt.title("(x, f(x)/pow(n, 2))")
		#plt.show()
		plt.savefig(outputFilePath + "over_nsquare.png")
	else: 
		if(graphictype == 2):
			plt.plot(graphic_x, graphic_fx_divided_ncube, 'b-')
			plt.xlabel('x = Cantidad de cartas', fontsize=14)
			plt.ylabel('f(x) = Microsegundos transcurridos', fontsize=14)
			plt.title("(x, f(x)/pow(n, 3))")
			#plt.show()
			plt.savefig(outputFilePath + "over_ncube.png")

#X = []
#Y = []
#Z = []
#for line in dataFile:
#	point_x = float(line.split()[0])
#	point_y = float(line.split()[1])
#	point_z = float(line.split()[2])
#	X.append(point_x)
#	Y.append(point_y)
#	Z.append(point_z)
#print X
#print Y
#print Z
##3d plotting
#
#fig = plt.figure()
#ax = fig.gca(projection='3d')
#X, Y = np.meshgrid(X, Y)
#surf = ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap=cm.coolwarm,
#        linewidth=0, antialiased=True)
#ax.zaxis.set_major_locator(LinearLocator(10))
#ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))
#
#ax.view_init(25, 70)
#
#fig.tight_layout()
#
#fig.colorbar(surf, shrink=0.5, aspect=5)
#
##plt.show()
#plt.savefig(outputFilePath)