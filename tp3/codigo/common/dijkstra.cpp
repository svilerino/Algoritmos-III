#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <limits>
#include <set>
#include <utility>
#include <algorithm>
#include <iterator>
#define SIN_PREDECESOR -1
 
using namespace std;

typedef int nodo_t;
typedef double peso_t;
 
const peso_t peso_infinito = numeric_limits<double>::infinity();
 
struct vecino {
    //atributos
    nodo_t nodo_target;
    peso_t peso;
    //constructor por defecto
    vecino(nodo_t arg_nodo_target, peso_t arg_peso) : nodo_target(arg_nodo_target), peso(arg_peso) { }
};

//renombre con typedef para mayor legibilidad 
typedef vector<vector<vecino> > lista_adyacencia_t;


void DijkstraCaminosMinimos(nodo_t origen, const lista_adyacencia_t &lista_adyacentes,
                          vector<peso_t> &distancias,
                          vector<nodo_t> &predecesores){
    //n es la cantidad de nodos en el grafo G=(V, E), es decir, el cardinal de V en esta definicion de G
    int n = lista_adyacentes.size();
    //inicializo el vector distancias y lo redimensiono a n elementos, todos inicializados en peso_infinito
    distancias.clear();
    distancias.resize(n, peso_infinito);
    //como dist(origen, origen) = 0, inicializamos esto
    distancias[origen] = 0;

    //inicializo el vector de predecesores, redimensiono a n elementos, todos inicializados a -SIN_PREDECESOR es decir, -1
    predecesores.clear();
    predecesores.resize(n, SIN_PREDECESOR);
    
    //declaro un conjunto de nodos
    set<pair<peso_t, nodo_t> > conjunto_nodos;
    //inserto el nodo origen
    conjunto_nodos.insert(make_pair(distancias[origen], origen));
 
    while (!conjunto_nodos.empty())
    {
        //obtengo los atributos del siguiente nodo a procesar
        peso_t dist = conjunto_nodos.begin()->first;
        nodo_t u = conjunto_nodos.begin()->second;

        //lo elimino del conjunto
        conjunto_nodos.erase(conjunto_nodos.begin());
 
        // Visito todos los vecinos del nodo u
        const vector<vecino> &vecinos = lista_adyacentes[u];
        for (vector<vecino>::const_iterator vecino_iter = vecinos.begin();vecino_iter != vecinos.end();vecino_iter++){
            //obtengo los atributos del vecino v
            nodo_t v = vecino_iter->nodo_target;
            peso_t peso = vecino_iter->peso;

            //calculo cual seria la distancia de src a v pasando por u
            peso_t distancia_pasando_por_u = dist + peso;
    	    if (distancia_pasando_por_u < distancias[v]) {
                //si la distancia nueva mejora, sobreescribo la distancia actual y seteo el mejor camino obtenido como camino entre src y v
    	        conjunto_nodos.erase(make_pair(distancias[v], v)); 
    	        distancias[v] = distancia_pasando_por_u;
    	        predecesores[v] = u;
    	        conjunto_nodos.insert(make_pair(distancias[v], v)); 
    	    }
        }
    }
}

list<nodo_t> DijkstraConstruirCamino(nodo_t nodo, const vector<nodo_t> &predecesores){
    list<nodo_t> camino;
    for (; nodo != SIN_PREDECESOR; nodo = predecesores[nodo]){
        camino.push_front(nodo);
    }
    return camino;
}
 
 
int main()
{
    //Sea G = (V, E) inicializar lista_adyacentes(n), donde n = #(V), para cada
    //i natural tal que 0<=i<n es el i-esimo nodo del grafo G en el conjunto V,
    //se agrega a la lista de nodos en lista_adyacentes[i](recordar es vector<vector<nodo>>)
    //cada vecino de ese nodo.
    // Recordar insertar dos veces las aristas(ambas direcciones) en un grafo no dirigido
    // pensar en arista entre u y v, v pertenece a vecinos (u) y u pertenece a vecinos(v)

    lista_adyacencia_t lista_adyacentes(6);
    // 0 = a
    lista_adyacentes[0].push_back(vecino(1, 7));
    lista_adyacentes[0].push_back(vecino(2, 9));
    lista_adyacentes[0].push_back(vecino(5, 14));
    // 1 = b
    lista_adyacentes[1].push_back(vecino(0, 7));
    lista_adyacentes[1].push_back(vecino(2, 10));
    lista_adyacentes[1].push_back(vecino(3, 15));
    // 2 = c
    lista_adyacentes[2].push_back(vecino(0, 9));
    lista_adyacentes[2].push_back(vecino(1, 10));
    lista_adyacentes[2].push_back(vecino(3, 11));
    lista_adyacentes[2].push_back(vecino(5, 2));
    // 3 = d
    lista_adyacentes[3].push_back(vecino(1, 15));
    lista_adyacentes[3].push_back(vecino(2, 11));
    lista_adyacentes[3].push_back(vecino(4, 6));
    // 4 = e
    lista_adyacentes[4].push_back(vecino(3, 6));
    lista_adyacentes[4].push_back(vecino(5, 9));
    // 5 = f
    lista_adyacentes[5].push_back(vecino(0, 14));
    lista_adyacentes[5].push_back(vecino(2, 2));
    lista_adyacentes[5].push_back(vecino(4, 9));
 
    vector<peso_t> distancias;
    vector<nodo_t> predecesores;
    DijkstraCaminosMinimos(0, lista_adyacentes, distancias, predecesores);
    cout << "Distancia minima de 0 a 4: " << distancias[4] << endl;
    list<nodo_t> camino = DijkstraConstruirCamino(4, predecesores);
    cout << "Camino Minimo: ";
    
    list<nodo_t>::const_iterator iterador;
    for (iterador = camino.begin(); iterador != camino.end(); ++iterador) {
        cout << (*iterador) << " -> ";
    }
    cout << "Nil" << endl;
    //copy(camino.begin(), camino.end(), ostream_iterator<nodo_t>(cout, " "));
    //cout << endl; 
    return 0;
}