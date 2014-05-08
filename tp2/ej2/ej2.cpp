#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "timing.h"
#include "ej2.h"

using namespace std;

int calcular_tiempo = 0;

Grafo *crear_grafo(int nodos)
{
	int i;

	Grafo *g = NULL;
	if(nodos <= 0)
		return NULL;

	g = (Grafo *)malloc(sizeof(Grafo));
	g->nodos = nodos;
	g->componente_conexa_nodos = (int *)calloc(nodos, sizeof(int));
	g->componentes_conexas = nodos;
	g->adyacencia = (int **)malloc(sizeof(int *) * nodos);

	for(i = 0; i < nodos; i++) {
		g->adyacencia[i] = (int *)calloc(nodos, sizeof(int));
	}
	for(i = 0; i < nodos; i++) {
		g->componente_conexa_nodos[i] = i;
	}
	return g;
}

void liberar_grafo(Grafo *g)
{
	int i;

	if(!g)
		return;

	free(g->componente_conexa_nodos);
	for(i = 0; i < g->nodos; i++) {
		free(g->adyacencia[i]);
	}
	free(g->adyacencia);
	free(g);
}

int agregar_arista(Grafo *g, Nodo nodo1, Nodo nodo2)
{
	int i;
	int nueva_componente, vieja_componente;

	if(!g)
		return -1;

	if(nodo1 >= g->nodos || nodo2 >= g->nodos)
		return -1;

	g->adyacencia[nodo1][nodo2] = 1;
	g->adyacencia[nodo2][nodo1] = 1;

	if(g->componente_conexa_nodos[nodo1] != g->componente_conexa_nodos[nodo2]){
		if(g->componente_conexa_nodos[nodo1] <= g->componente_conexa_nodos[nodo2]){
			nueva_componente = g->componente_conexa_nodos[nodo1];
			vieja_componente = g->componente_conexa_nodos[nodo2];
		}
		else{
			nueva_componente = g->componente_conexa_nodos[nodo2];
			vieja_componente = g->componente_conexa_nodos[nodo1];
		}

		for(i = 0; i < g->nodos; i++){ //actualizo la componente conexa de todos los nodos que pertenecian a esa componente
			if(g->componente_conexa_nodos[i] == vieja_componente){
				g->componente_conexa_nodos[i] = nueva_componente;
			}
		}
		g->componentes_conexas--;
	}
	return 0;
}

int cantidad_componentes_conexas(Grafo *g)
{
	if(!g)
		return -1;
	return g->componentes_conexas;
}

//retorna un vector con cantidad_componentes_conexas() elementos, en cada iesimo elemento, hay un nodo correspondiente a la componente iesima. liberar el resultado con free()
Nodo *nodos_de_componentes(Grafo *g)
{
	Nodo *nodos;
	int i;

	if(!g)
		return NULL;

	nodos = (Nodo *)malloc(sizeof(Nodo) * g->nodos);
	for(i = 0; i < g->nodos; i++){
		nodos[i] = -1;
	}
	for(i = 0; i < g->componentes_conexas; i++){
		nodos[g->componente_conexa_nodos[i]] = i;
	}
	return nodos;
}

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

