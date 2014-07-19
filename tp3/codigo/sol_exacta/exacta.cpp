#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include "grafo.h"
#include "parser.h"
#include "timing.h"

#define CANT_ITERS_MEDICION 1

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

void dijkstra(Grafo<peso> *g, int nodo, float **_distancia_w1, float **_distancia_w2)
{
	int cantidad_nodos, nodo_minimo, cantidad_adyacentes, *adyacentes, cantidad_vistos;
	int i;
	float *distancia_w1, *distancia_w2;
	float peso_minimo;
	bool *nodos_vistos;

	if(g == NULL || _distancia_w1 == NULL || _distancia_w2 == NULL)
		return;

	cantidad_nodos = g->CantidadNodos();
	if(nodo < 1 || nodo > cantidad_nodos)
		return;

	nodos_vistos = new bool[cantidad_nodos + 1];
	*_distancia_w1 = new float[cantidad_nodos + 1];
	*_distancia_w2 = new float[cantidad_nodos + 1];
	distancia_w1 = *_distancia_w1;
	distancia_w2 = *_distancia_w2;
	
	for(i = 1; i <= cantidad_nodos; i++){
		distancia_w1[i] = INFINITY;
		distancia_w2[i] = INFINITY;
		nodos_vistos[i] = false;
	}
	distancia_w1[nodo] = 0;
	distancia_w2[nodo] = 0;
	cantidad_vistos = 0;

	while(cantidad_vistos < cantidad_nodos){
		peso_minimo = INFINITY;
		nodo_minimo = 1;
		for(i = 1; i <= cantidad_nodos; i++){
			if(!nodos_vistos[i] && distancia_w1[i] < peso_minimo){
				nodo_minimo = i;
				peso_minimo = distancia_w1[i];
			}
		}
		nodos_vistos[nodo_minimo] = true;
		cantidad_vistos++;
		adyacentes = g->Adyacentes(nodo_minimo, &cantidad_adyacentes);
		for(i = 0; i < cantidad_adyacentes; i++){
			if(!nodos_vistos[adyacentes[i]] && distancia_w1[adyacentes[i]] > distancia_w1[nodo_minimo] + g->Peso(nodo_minimo, adyacentes[i])->w1){
				distancia_w1[adyacentes[i]] = distancia_w1[nodo_minimo] + g->Peso(nodo_minimo, adyacentes[i])->w1;
			}
		}
		free(adyacentes);
	}

	for(i = 1; i <= cantidad_nodos; i++){
		nodos_vistos[i] = false;
	}
	cantidad_vistos = 0;

	while(cantidad_vistos < cantidad_nodos){
		peso_minimo = INFINITY;
		nodo_minimo = 1;
		for(i = 1; i <= cantidad_nodos; i++){
			if(!nodos_vistos[i] && distancia_w2[i] < peso_minimo){
				nodo_minimo = i;
				peso_minimo = distancia_w2[i];
			}
		}
		nodos_vistos[nodo_minimo] = true;
		cantidad_vistos++;
		adyacentes = g->Adyacentes(nodo_minimo, &cantidad_adyacentes);
		for(i = 0; i < cantidad_adyacentes; i++){
			if(!nodos_vistos[adyacentes[i]] && distancia_w2[adyacentes[i]] > distancia_w2[nodo_minimo] + g->Peso(nodo_minimo, adyacentes[i])->w2){
				distancia_w2[adyacentes[i]] = distancia_w2[nodo_minimo] + g->Peso(nodo_minimo, adyacentes[i])->w2;
			}
		}
		free(adyacentes);
	}

	delete [] nodos_vistos;

}

int main(int argc, char **argv)
{
	int medir_tiempo = 0;
	GrafoAdyacencia<peso> *grafo;
	peso **pesos;
	int u, v, K, nodos, aristas, i;
	Solucion solucion;

	//if(argc != 1)
	medir_tiempo = 1;//dejalo asi, lo necesito para los scripts, idem la cantidad de iteraciones(necesito que imprima la salida una sola vez). Firma: Silvio.
	grafo = new GrafoAdyacencia<peso>(0);
	while(Parsear<peso>(*grafo, stdin, &pesos, setW1, setW2, &u, &v, &K, &nodos, &aristas)){
		if(medir_tiempo){
			double promedio_medicion = 0.0;
			MEDIR_TIEMPO_PROMEDIO(
				float *distancia_w1;
				float *distancia_w2;
				solucion.W1 = 0;
				solucion.W2 = 0;
				solucion.k = 1;
				solucion.v = (int *)calloc(2 * aristas, sizeof(int));
				solucion.v[0] = u;
				dijkstra(grafo, u, &distancia_w1, &distancia_w2);
				if(distancia_w1[v] > K){
					printf("no");
				}
				else if(resolver(*grafo, u, v, K, &solucion)){
					printf("%.0f %.0f %d", solucion.W1, solucion.W2, solucion.k);
					for(i = 0; i < solucion.k; i++){
						printf(" %d", solucion.v[i]);
					}
					printf("\n");
				}
				else{
					printf("no");
				}
				free(solucion.v);
				delete [] distancia_w1;
				delete [] distancia_w2;
			, CANT_ITERS_MEDICION, &promedio_medicion);
			//cerr << promedio << " " << nodos << " " << aristas << endl;
			cerr << nodos << " " << aristas << " " << CANT_ITERS_MEDICION << " " << promedio_medicion;
		}
		else{
			float *distancia_w1;
			float *distancia_w2;
			solucion.W1 = 0;
			solucion.W2 = 0;
			solucion.k = 1;
			solucion.v = (int *)calloc(2 * aristas, sizeof(int));
			solucion.v[0] = u;
			dijkstra(grafo, u, &distancia_w1, &distancia_w2);
			if(distancia_w1[v] > K){
				printf("no");
			}
			else if(resolver(*grafo, u, v, K, &solucion)){
				printf("%.0f %.0f %d", solucion.W1, solucion.W2, solucion.k);
				for(i = 0; i < solucion.k; i++){
					printf(" %d", solucion.v[i]);
				}
				printf("\n");
			}
			else{
				printf("no");
			}
			free(solucion.v);
			delete [] distancia_w1;
			delete [] distancia_w2;
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

