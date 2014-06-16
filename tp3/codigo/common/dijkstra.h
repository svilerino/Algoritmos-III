#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <limits>
#include <utility>
#include <algorithm>
#include <set>
#include <iterator>
#define SIN_PREDECESOR -1
//#define VECINOS_CONTAINER vector
//#define CONTAINER_ADD_METHOD push_back
#define VECINOS_CONTAINER multiset
#define CONTAINER_ADD_METHOD insert

using namespace std;

typedef int nodo_index_t;
typedef double costo_t;
 
const costo_t costo_infinito = numeric_limits<double>::infinity();
 
struct nodo_t {
    //atributos
    nodo_index_t nodo_index;
    costo_t costo_w1;
    costo_t costo_w2;
    //constructor por defecto
    nodo_t(nodo_index_t arg_nodo_index, costo_t arg_costo_w1, costo_t arg_costo_w2) : nodo_index(arg_nodo_index), costo_w1(arg_costo_w1), costo_w2(arg_costo_w2) { }
    
    //operador de comparacion sobre w2
    bool operator<(const struct nodo_t &other) const
    {
        return costo_w2 < other.costo_w2;
    }

    //operador de igualdad sobre nodo_index
    bool operator==(const struct nodo_t &other) const
    {
        return nodo_index == other.nodo_index;
    }
};

//renombre con typedef para mayor legibilidad 
typedef vector<VECINOS_CONTAINER<nodo_t> > lista_adyacencia_t;

void dijkstraCaminosMinimos(nodo_index_t origen, const lista_adyacencia_t &lista_adyacentes,
                          vector<costo_t> &costos,
                          vector<nodo_index_t> &predecesores){
    //n es la cantidad de nodos en el grafo G=(V, E), es decir, el cardinal de V en esta definicion de G
    int n = lista_adyacentes.size();
    //inicializo el vector costos y lo redimensiono a n elementos, todos inicializados en costo_infinito
    costos.clear();
    costos.resize(n, costo_infinito);
    //como cost(origen, origen) = 0, inicializamos esto
    costos[origen] = 0;

    //inicializo el vector de predecesores, redimensiono a n elementos, todos inicializados a -SIN_PREDECESOR es decir, -1
    predecesores.clear();
    predecesores.resize(n, SIN_PREDECESOR);
    
    //declaro un conjunto de nodos
    set<pair<costo_t, nodo_index_t> > conjunto_nodos;
    //inserto el nodo origen
    conjunto_nodos.insert(make_pair(costos[origen], origen));
 
    while (!conjunto_nodos.empty())
    {
        //obtengo los atributos del siguiente nodo a procesar
        nodo_index_t u = conjunto_nodos.begin()->second;
        costo_t cost_u = conjunto_nodos.begin()->first;

        //elimino al nodo u del conjunto
        conjunto_nodos.erase(conjunto_nodos.begin());
 
        // Visito todos los vecinos del nodo u
        const VECINOS_CONTAINER<nodo_t> &vecinos = lista_adyacentes[u];
        for (VECINOS_CONTAINER<nodo_t>::const_iterator vecino_iter = vecinos.begin(); vecino_iter != vecinos.end(); ++vecino_iter){
            //obtengo los atributos del vecino v
            nodo_index_t v = vecino_iter->nodo_index;
            costo_t cost_v = vecino_iter->costo_w1;

            //calculo cual seria la costo de src a v pasando por u
            costo_t costo_pasando_por_u = cost_u + cost_v;
    	    if (costo_pasando_por_u < costos[v]) {
                //si la costo nueva mejora, sobreescribo la costo actual y seteo el mejor camino obtenido como camino entre src y v
    	        conjunto_nodos.erase(make_pair(costos[v], v)); 
    	        costos[v] = costo_pasando_por_u;
    	        predecesores[v] = u;
    	        conjunto_nodos.insert(make_pair(costos[v], v)); 
    	    }
        }
    }
}

list<nodo_index_t> dijkstraConstruirCamino(nodo_index_t nodo, const vector<nodo_index_t> &predecesores){
    list<nodo_index_t> camino;
    for (; nodo != SIN_PREDECESOR; nodo = predecesores[nodo]){
        camino.push_front(nodo);
    }
    return camino;
}