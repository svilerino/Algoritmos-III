#include <stdio.h>
#include <iostream>
#include <string.h>
#include "../common/grafo.h"
#include "../common/parser.h"

using namespace std;

int main(int argc, char **argv)
{
	//Sea G=(V,E) un grafo,
	//Sean f,g  dos funciones de pesos en las aristas del grafo y sea K en R un limite para la funcion g
	//Quiero un camino minimo sobre f pero tal que g<K entre dos nodos A y B
	//Dado un grafo G, primero voy a buscar un camino inicial factible.
	//Para esto voy a hacer dijkstra desde A, y me voy a quedar con el camino de A hasta B que minimice g
	//si dicho camino se pasa de la cota K entonces no existe solucion.
	//Caso contrario, este camino sera la solucion inicial factible para comenzar la busqueda local
	return 0;
}