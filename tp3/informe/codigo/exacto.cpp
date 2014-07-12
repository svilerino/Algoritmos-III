#include <stdio.h>
#include <iostream>
#include <string.h>
#include "grafo.h"
#include "parser.h"
#include "timing.h"

using namespace std;

typedef struct peso{
	int w1;
	int w2;
	int otra_cosa;
} peso;

typedef struct Solucion{
	float W1;
	float W2;
	int k;
	int *v;
} Solucion;

void setW1(void *p, float w)
{
	((peso *)p)->w1 = w;
}
void setW2(void *p, float w)
{
	((peso *)p)->w2 = w;
}

bool resolver(Grafo<peso> &g, int u, int w, int K, Solucion *solucion)
{
	Solucion solucion_mejor, solucion_nueva;
	int cantidad_aristas, *adyacentes, i;
	peso *p;
	float w2, w1;
	bool solucionado = false;

	solucion_mejor.W1 = -1;
	solucion_mejor.W2 = -1;
	solucion_mejor.k = 0;
	solucion_mejor.v = (int *)calloc(2 * g.CantidadAristas(), sizeof(int));
	solucion_nueva.W1 = 0;
	solucion_nueva.W2 = 0;
	solucion_nueva.k = 0;
	solucion_nueva.v = (int *)calloc(2 * g.CantidadAristas(), sizeof(int));

	if(u == w){
		free(solucion_mejor.v);
		free(solucion_nueva.v);
		return true;
	}
	g.MarcarNodo(u, true);
	adyacentes = g.Adyacentes(u, &cantidad_aristas);
	for(i = 0; i < cantidad_aristas; i++){
		if(g.VerMarcaDeNodo(adyacentes[i]) == false){
			p = g.Peso(u, adyacentes[i]);
			w1 = p->w1;
			w2 = p->w2;
			if(w1 <= K && (solucion_mejor.k == 0 || w2 < solucion_mejor.W2)){
				g.QuitarArista(u, adyacentes[i]);
				solucion_nueva.W1 = w1;
				solucion_nueva.W2 = w2;
				solucion_nueva.v[0] = adyacentes[i];
				solucion_nueva.k = 1;
				if(resolver(g, adyacentes[i], w, K - w1, &solucion_nueva)){
					if(solucion_mejor.W1 < 0 || solucion_mejor.W2 > solucion_nueva.W2){
						solucionado = true;
						solucion_mejor.W1 = solucion_nueva.W1;
						solucion_mejor.W2 = solucion_nueva.W2;
						solucion_mejor.k = solucion_nueva.k;
						memcpy(solucion_mejor.v, solucion_nueva.v, 2 * g.CantidadAristas() * sizeof(int));
					}
				}
				g.AgregarArista(u, adyacentes[i], p);
			}
		}
	}
	g.MarcarNodo(u, false);
	if(solucionado){
		solucion->W1 += solucion_mejor.W1;
		solucion->W2 += solucion_mejor.W2;
		memcpy(solucion->v + solucion->k, solucion_mejor.v, solucion_mejor.k * sizeof(int));
		solucion->k += solucion_mejor.k;
	}
	free(adyacentes);
	free(solucion_mejor.v);
	free(solucion_nueva.v);
	return solucionado;
}

int main(int argc, char **argv)
{
	int medir_tiempo = 0;
	GrafoAdyacencia<peso> *grafo;
	peso **pesos;
	int u, v, K, nodos, aristas, i;
	Solucion solucion;

	if(argc != 1)
		medir_tiempo = 1;
	grafo = new GrafoAdyacencia<peso>(0);
	while(Parsear<peso>(*grafo, stdin, &pesos, setW1, setW2, &u, &v, &K, &nodos, &aristas)){
		if(medir_tiempo){
			double promedio = 0.0;
			MEDIR_TIEMPO_PROMEDIO(
				solucion.W1 = 0;
				solucion.W2 = 0;
				solucion.k = 1;
				solucion.v = (int *)calloc(2 * aristas, sizeof(int));
				solucion.v[0] = u;
				if(resolver(*grafo, u, v, K, &solucion)){
					printf("%f %f %d", solucion.W1, solucion.W2, solucion.k);
					for(i = 0; i < solucion.k; i++){
						printf(" %d", solucion.v[i]);
					}
					printf("\n");
				}
				else{
					printf("no\n");
				}
				free(solucion.v);
			, 5, &promedio);
			cerr << promedio << " " << nodos << " " << aristas << endl;
		}
		else{
			solucion.W1 = 0;
			solucion.W2 = 0;
			solucion.k = 1;
			solucion.v = (int *)calloc(2 * aristas, sizeof(int));
			solucion.v[0] = u;
			if(resolver(*grafo, u, v, K, &solucion)){
				printf("%f %f %d", solucion.W1, solucion.W2, solucion.k);
				for(i = 0; i < solucion.k; i++){
					printf(" %d", solucion.v[i]);
				}
				printf("\n");
			}
			else{
				printf("no\n");
			}
			free(solucion.v);
		}
		int i;
		for(i = 0; i < aristas; i++){
			delete pesos[i];
		}
		free(pesos);
		delete grafo;
		grafo = new GrafoAdyacencia<peso>(0);
	}
	delete grafo;
	return 0;
/*
	{
		peso p1, *p;
		GrafoAdyacencia<peso> grafo(10);

		p1.w1 = 42;
		p1.w2 = 30;
		grafo.AgregarArista(2,3, &p1);

		cout << grafo.EsAdyacente(2,3) << " " << grafo.EsAdyacente(9,8) << endl;
		p = grafo.Peso(2, 3);
		cout << p->w1 << " " << p->w2 << " " << endl;
	}
	{
		peso p1, *p;
		GrafoLista<peso> grafo(10);

		p1.w1 = 32;
		p1.w2 = 20;
		grafo.AgregarArista(5,3, &p1);

		cout << grafo.EsAdyacente(5,3) << " " << grafo.EsAdyacente(2,3) << endl;
		p = grafo.Peso(5, 3);
		cout << p->w1 << " " << p->w2 << " " << endl;
	}

	GrafoAdyacencia<peso> *grafo;
	peso **pesos;
	int u,v, K, nodos, aristas;
	grafo = new GrafoAdyacencia<peso>(0);
	while(Parsear<peso>(*grafo, stdin, &pesos, setW1, setW2, &u, &v, &K, &nodos, &aristas)){
		cout << "tiene " << grafo->CantidadNodos() << " nodos" << endl;
		int i;
		for(i = 0; i < aristas; i++){
			delete pesos[i];
		}
		free(pesos);
		delete grafo;
		grafo = new GrafoAdyacencia<peso>(0);
	}
	delete grafo;

	return 0;
	*/
}




#ifndef __GRAFO_H__
#define __GRAFO_H__

#include <stdlib.h>

/**
 * Esta es una clase "abstracta", hay que usar las otra implementaciones de Grafo, como GrafoAdyacencia
 */
template <class W> class Grafo {
	public:
		virtual ~Grafo(void){}
		/**
		 * agrega n nodos
		 */
		virtual bool AgregarNodos(int n) = 0;
		/**
		 * Agrega una arista entre i y j, 1 <= i <= n, 1 <= j <= n, n cantidad de nodos.
		 */
		virtual bool AgregarArista(int i, int j, W *peso) = 0;
		/**
		 * Quita la arista entre i y j, 1 <= i <= n, 1 <= j <= n, n cantidad de nodos.
		 */
		virtual bool QuitarArista(int i, int j) = 0;
		virtual bool EsAdyacente(int i, int j) = 0;
		/**
		 * i y j tienen que ser adyacentes, sino retorna NULL.
		 */
		virtual W *Peso(int i, int j) = 0;
		/**
		 * retorna un array de nodos adyacentes a i, en m retorna la cantidad de elementos en el array que se retorna. liberar con free()
		 */
		virtual int *Adyacentes(int i, int *m) = 0;

		virtual int CantidadNodos(void) = 0;
		virtual int CantidadAristas(void) = 0;

		/**
		 * permite poner una marca de true o false en un nodo
		 */
		virtual void MarcarNodo(int i, bool marca) = 0;
		/**
		 * permite ver como fue marcado el nodo, al crearse el grafo todos arranca en false
		 */
		virtual bool VerMarcaDeNodo(int i) = 0;
};

template <class W> class GrafoAdyacencia : public Grafo<W> {
	private:
		typedef struct Adyacente{
			bool adyacente;
			W *peso;
		}Adyacente;

		Adyacente **adyacencia;
		bool *marcas;
		int nodos;
		int aristas;

	public:
		/**
		 * crea un grafo con n nodos
		 * O(n^2)
		 */
		GrafoAdyacencia(int n)
		{
			int i, j;
			nodos = n;
			adyacencia = (Adyacente **)calloc(n, sizeof(Adyacente *));
			for(i = 0; i < n; i++){
				adyacencia[i] = (Adyacente *)calloc(n, sizeof(Adyacente));
				for(j = 0; j < n; j++){
					adyacencia[i][j].adyacente = false;
					adyacencia[i][j].peso = NULL;
				}
			}
			marcas = (bool *)calloc(n, sizeof(bool));
			for(i = 0; i < n; i++){
				marcas[i] = false;
			}
			aristas = 0;
		}

		/**
		 * O(nodos)
		 */
		~GrafoAdyacencia(void)
		{
			int i;
			for(i = 0; i < nodos; i++){
				free(adyacencia[i]);
			}
			free(marcas);
			free(adyacencia);
		}

		/**
		 * agrega n nodos
		 * O(n*(nodos + n) + nodos*(n + nodos))
		 */
		bool AgregarNodos(int n)
		{
			int i, j;

			if(n <= 0)
				return false;

			adyacencia = (Adyacente **)realloc(adyacencia, (nodos + n) * sizeof(Adyacente *));

			for(i = nodos; i < nodos + n; i++){
				adyacencia[i] = (Adyacente *)calloc(n, sizeof(Adyacente));
				for(j = 0; j < nodos + n; j++){
					adyacencia[i][j].adyacente = false;
					adyacencia[i][j].peso = NULL;
				}
			}
			for(i = 0; i < nodos; i++){
				for(j = nodos; j < nodos + n; j++){
					adyacencia[i][j].adyacente = false;
					adyacencia[i][j].peso = NULL;
				}
			}

			marcas = (bool *)realloc(marcas, (n + nodos) * sizeof(bool));
			for(i = nodos; i < nodos + n; i++){
				marcas[i] = false;
			}

			nodos += n;
			return true;
		}

		/**
		 * Agrega una arista entre i y j, 1 <= i <= n, 1 <= j <= n, n cantidad de nodos.
		 * O(1)
		 */
		bool AgregarArista(int i, int j, W *peso)
		{
			if(i <= 0 || i > nodos || j <= 0 || j > nodos)
				return false;
			
			adyacencia[i - 1][j - 1].adyacente = true;
			adyacencia[i - 1][j - 1].peso = peso;
			adyacencia[j - 1][i - 1].adyacente = true;
			adyacencia[j - 1][i - 1].peso = peso;
			aristas++;
			return true;
		}

		/**
		 * Quita la arista entre i y j, 1 <= i <= n, 1 <= j <= n, n cantidad de nodos.
		 * O(1)
		 */
		bool QuitarArista(int i, int j)
		{
			if(i <= 0 || i > nodos || j <= 0 || j > nodos)
				return false;
			
			adyacencia[i - 1][j - 1].adyacente = false;
			adyacencia[i - 1][j - 1].peso = NULL;
			adyacencia[j - 1][i - 1].adyacente = false;
			adyacencia[j - 1][i - 1].peso = NULL;
			aristas--;
			return true;

		}

		/**
		 * O(1)
		 */
		bool EsAdyacente(int i, int j)
		{
			if(i <= 0 || i > nodos || j <= 0 || j > nodos)
				return false;

			return adyacencia[i - 1][j - 1].adyacente;
		}

		/**
		 * i y j tienen que ser adyacentes, sino retorna NULL.
		 * O(1)
		 */
		W *Peso(int i, int j)
		{
			if(i <= 0 || i > nodos || j <= 0 || j > nodos)
				return NULL;

			if(!adyacencia[i - 1][j - 1].adyacente)
				return NULL;

			return adyacencia[i - 1][j - 1].peso;
		}

		/**
		 * retorna un array de nodos adyacentes a i, en m retorna la cantidad de elementos en el array que se retorna. liberar con free()
		 * O(nodos)
		 */
		int *Adyacentes(int i, int *m)
		{
			int *ret;
			int j;

			if(!m || i <= 0 || i > nodos)
				return NULL;
			*m = 0;
			ret = (int *)calloc(nodos, sizeof(int));
			for(j = 0; j < nodos; j++){
				if(adyacencia[i - 1][j].adyacente){
					ret[*m] = j + 1;
					(*m)++;
				}
			}
			return ret;
		}

		int CantidadNodos(void)
		{
			return nodos;
		}

		int CantidadAristas(void)
		{
			return aristas;
		}

		void MarcarNodo(int i, bool marca)
		{
			if(i <= 0 || i > nodos)
				return;
			this->marcas[i - 1] = marca;
		}

		bool VerMarcaDeNodo(int i)
		{
			if(i <= 0 || i > nodos)
				return false;
			return marcas[i - 1];
		}
};

template <class W> class GrafoLista : public Grafo<W> {
	private:
		typedef struct Lista {
			struct Lista *siguiente;
			W *peso;
			int nodo;
		} Lista;
		Lista **lista_nodos;
		bool *marcas;
		int nodos;
		int aristas;

	public:
		/**
		 * O(n)
		 */
		GrafoLista(int n)
		{
			int i;
			nodos = n;
			lista_nodos = (Lista **)calloc(n, sizeof(Lista *));
			aristas = 0;
			marcas = (bool *)calloc(n, sizeof(bool));
			for(i = 0; i < n; i++){
				marcas[i] = false;
			}
		}

		/**
		 * O(nodos + aristas)
		 */
		~GrafoLista(void)
		{
			Lista *l, *l2;
			int i;
			for(i = 0; i < nodos; i++){
				l = lista_nodos[i];
				while(l != NULL){
					l2 = l;
					l = l->siguiente;
					free(l2);
				}
			}
			free(lista_nodos);
			free(marcas);
		}

		/**
		 * agrega n nodos
		 * O(n)
		 */
		bool AgregarNodos(int n)
		{
			int i;

			if(n <= 0)
				return false;

			lista_nodos = (Lista **)realloc(lista_nodos, (n + nodos) * sizeof(Lista *));
			marcas = (bool *)realloc(marcas, (n + nodos) * sizeof(bool));
			for(i = nodos; i < nodos + n; i++){
				lista_nodos[i] = NULL;
				marcas[i] = false;
			}
			nodos += n;
			return true;
		}

		/**
		 * Agrega una arista entre i y j, 1 <= i <= n, 1 <= j <= n, n cantidad de nodos.
		 * O(1)
		 */
		bool AgregarArista(int i, int j, W *peso)
		{
			Lista *nuevo;

			if(i <= 0 || i > nodos || j <= 0 || j > nodos)
				return false;

			nuevo = (Lista *)malloc(sizeof(Lista));
			nuevo->nodo = j;
			nuevo->peso = peso;
			nuevo->siguiente = lista_nodos[i - 1];
			lista_nodos[i - 1] = nuevo;

			nuevo = (Lista *)malloc(sizeof(Lista));
			nuevo->nodo = i;
			nuevo->peso = peso;
			nuevo->siguiente = lista_nodos[j - 1];
			lista_nodos[j - 1] = nuevo;

			aristas++;
			return true;
		}
		/**
		 * Quita la arista entre i y j, 1 <= i <= n, 1 <= j <= n, n cantidad de nodos.
		 * O(aristas)
		 */
		bool QuitarArista(int i, int j)
		{
			Lista *l, *p;
			if(i <= 0 || i > nodos || j <= 0 || j > nodos)
				return false;

			l = lista_nodos[i - 1];
			p = NULL;
			while(l != NULL){
				if(l->nodo == j){
					if(p != NULL)
						p->siguiente = l->siguiente;
					else
						lista_nodos[i - 1] = l->siguiente;
					free(l);
				}
				p = l;
				l = l->siguiente;
			}

			l = lista_nodos[j - 1];
			p = NULL;
			while(l != NULL){
				if(l->nodo == i){
					if(p != NULL)
						p->siguiente = l->siguiente;
					else
						lista_nodos[i - 1] = l->siguiente;
					free(l);
				}
				p = l;
				l = l->siguiente;
			}
			aristas--;
			return true;
		}

		/**
		 * O(aristas)
		 */
		bool EsAdyacente(int i, int j)
		{
			Lista *l;
			if(i <= 0 || i > nodos || j <= 0 || j > nodos)
				return false;

			l = lista_nodos[i - 1];
			while(l != NULL){
				if(l->nodo == j){
					return true;
				}
				l = l->siguiente;
			}
			return false;
		}
		/**
		 * i y j tienen que ser adyacentes, sino retorna NULL.
		 */
		W *Peso(int i, int j)
		{
			Lista *l;
			if(i <= 0 || i > nodos || j <= 0 || j > nodos)
				return NULL;

			l = lista_nodos[i - 1];
			while(l != NULL){
				if(l->nodo == j){
					return l->peso;
				}
				l = l->siguiente;
			}
			return NULL;

		}
		/**
		 * retorna un array de nodos adyacentes a i, en m retorna la cantidad de elementos en el array que se retorna. liberar con free()
		 * O(aristas)
		 */
		int *Adyacentes(int i, int *m)
		{
			int *ret;
			Lista *l;

			if(!m || i <= 0 || i > nodos)
				return NULL;
			*m = 0;
			ret = (int *)calloc(nodos, sizeof(int));
			l = lista_nodos[i - 1];
			while(l != NULL){
				ret[*m] = l->nodo;
				(*m)++;
				l = l->siguiente;
			}
			return ret;
		}

		int CantidadNodos(void)
		{
			return nodos;
		}

		int CantidadAristas(void)
		{
			return aristas;
		}

		void MarcarNodo(int i, bool marca)
		{
			if(i <= 0 || i > nodos)
				return;
			this->marcas[i - 1] = marca;
		}

		bool VerMarcaDeNodo(int i)
		{
			if(i <= 0 || i > nodos)
				return false;
			return marcas[i - 1];
		}
};

#endif

