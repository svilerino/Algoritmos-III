#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include "grafo.h"

typedef void (*setW)(void *peso, float w);

/**
 * hay que tener ya un grafo creado (preferentemente sin nodos) y llamar a Parsear sucesivamente hasta que retorne false, en cada retorno de true devuelve una instancia.
 * setW1 y setW2 son 2 funciones que hay que pasar que van a colocar en W el peso1 y peso2 
 *
 * y en **pesos, retorna un array (que hay que liberar con free), con todos los pesos que se crearon
 *
 * ejemplo:
 *
 *	typedef struct Peso{
 *		float w1;
 *		float w2;
 *		int otra_cosa;
 *	} Peso;
 *
 *	void setW1(Peso *p, float w)
 *	{
 *		p->w1 = w;
 *	}
 *	void setW2(Peso *p, float w)
 *	{
 *		p->w2 = w;
 *	}
 *
 *	int main()
 *	{
 *		GrafoAdyacencia<peso> *grafo;
 *		peso **pesos;
 *		int u,v, K, nodos, aristas;
 *		grafo = new GrafoAdyacencia<peso>(0);
 *		while(Parsear<peso>(*grafo, stdin, &pesos, setW1, setW2, &u, &v, &K, &nodos, &aristas)){
 *		//hacer lo que haya que hacer con "grafo"
 *			int i;
 *			for(i = 0; i < aristas; i++){
 *				delete pesos[i];
 *			}
 *			free(pesos);
 *			delete grafo;
 *			grafo = new GrafoAdyacencia<peso>(0);
 *		}
 *		delete grafo;
 *	}
 */
template<class W>
bool Parsear(Grafo<W> &g, FILE *f, W ***pesos, setW setW1, setW setW2, int *u, int *v, int *K, int *nodos, int *aristas)
{
	int i, v1, v2;
	float w1, w2;
	W *peso;

	if(!f || !pesos || !u || !v || !K || !nodos || !aristas)
		return false;
	
	*nodos = 0;
	*aristas = 0;
	fscanf(f, "%d %d %d %d %d", nodos, aristas, u, v, K);
	if(*nodos <= 0 || aristas <= 0) //no mas instancias
		return false;
	*pesos = (W **)calloc(*aristas, sizeof(W *));
	g.AgregarNodos(*nodos);
	for(i = 0; i < *aristas; i++){
		fscanf(f, "%d %d %f %f", &v1, &v2, &w1, &w2);
		peso = new W;
		setW1(peso, w1);
		setW2(peso, w2);
		(*pesos)[i] = peso;
		g.AgregarArista(v1, v2, peso);
	}
	return true;
}

#endif

