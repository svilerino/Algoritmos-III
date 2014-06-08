#ifndef __GRAFO_H__
#define __GRAFO_H__

#include <stdlib.h>

/**
 * Esta es una clase "abstracta", hay que usar las otra implementaciones de Grafo, como GrafoAdyacencia
 */
template <class W> class Grafo {
	public:
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
};

template <class W> class GrafoAdyacencia : Grafo<W> {
	private:
		typedef struct Adyacente{
			bool adyacente;
			W *peso;
		}Adyacente;

		Adyacente **adyacencia;
		int nodos;

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
		}

		/**
		 * O(nodos)
		 */
		~GrafoAdyacencia()
		{
			int i;
			for(i = 0; i < nodos; i++){
				free(adyacencia[i]);
			}
			free(adyacencia);
		}

		/**
		 * Agrega una arista entre i y j, 1 <= i <= n, 1 <= j <= n, n cantidad de nodos.
		 * O(1)
		 */
		bool AgregarArista(int i, int j, W *peso){
			if(i <= 0 || i > nodos || j <= 0 || j > nodos)
				return false;
			
			adyacencia[i - 1][j - 1].adyacente = true;
			adyacencia[i - 1][j - 1].peso = peso;
			return true;
		}

		/**
		 * Quita la arista entre i y j, 1 <= i <= n, 1 <= j <= n, n cantidad de nodos.
		 * O(1)
		 */
		bool QuitarArista(int i, int j){
			if(i <= 0 || i > nodos || j <= 0 || j > nodos)
				return false;
			
			adyacencia[i - 1][j - 1].adyacente = false;
			adyacencia[i - 1][j - 1].peso = NULL;
			return true;

		}

		/**
		 * O(1)
		 */
		bool EsAdyacente(int i, int j){
			if(i <= 0 || i > nodos || j <= 0 || j > nodos)
				return false;

			return adyacencia[i - 1][j - 1].adyacente;
		}

		/**
		 * i y j tienen que ser adyacentes, sino retorna NULL.
		 * O(1)
		 */
		W *Peso(int i, int j) {
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
		int *Adyacentes(int i, int *m){
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
};

template <class W> class GrafoLista : Grafo<W> {
	private:
		typedef struct Lista {
			struct Lista *siguiente;
			W *peso;
			int nodo;
		} Lista;
		Lista **lista_nodos;
		int nodos;

	public:
		/**
		 * O(1)
		 */
		GrafoLista(int n){
			nodos = n;
			lista_nodos = (Lista **)calloc(n, sizeof(Lista *));
		}

		/**
		 * O(nodos + aristas)
		 */
		~GrafoLista(){
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
		}
		/**
		 * Agrega una arista entre i y j, 1 <= i <= n, 1 <= j <= n, n cantidad de nodos.
		 * O(1)
		 */
		bool AgregarArista(int i, int j, W *peso){
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

			return true;
		}
		/**
		 * Quita la arista entre i y j, 1 <= i <= n, 1 <= j <= n, n cantidad de nodos.
		 * O(aristas)
		 */
		bool QuitarArista(int i, int j){
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
			return true;
		}

		/**
		 * O(aristas)
		 */
		bool EsAdyacente(int i, int j){
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
		W *Peso(int i, int j){
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
		int *Adyacentes(int i, int *m){
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
};

#endif

