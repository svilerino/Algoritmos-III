#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "timing.h"
#include "ej2.h"

using namespace std;

int calcular_tiempo = 0;

int resolver(void)
{
	return 0;
}

int main(int argc, char **argv)
{
	int n_ciudades = 0;
	int k_centrales = 0;

	if(argc > 1)
		calcular_tiempo = 1;

	if(calcular_tiempo){
		double promedio = 0.0;
		MEDIR_TIEMPO_PROMEDIO(resolver();, REPETICIONES_CALCULAR_TIEMPO, &promedio);
		cerr << n_ciudades << " " << k_centrales << " " << promedio << " " << REPETICIONES_CALCULAR_TIEMPO << endl;
	}

	return 0;
}

