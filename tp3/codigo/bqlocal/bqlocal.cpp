#include <stdio.h>
#include <iostream>
#include <string.h>
#include "../common/grafo.h"
#include "../common/parser.h"
#include "../common/dijkstra.h"

using namespace std;

void agregar_arista(lista_adyacencia_t& lista_adyacentes, nodo_t a, nodo_t b, costo_t w1, costo_t w2){
    // Recordar agregar dos veces las aristas(ambas direcciones) en un grafo no dirigido
    // pensar en arista entre u y v, v pertenece a vecinos (u) y u pertenece a vecinos(v)
	lista_adyacentes[a].CONTAINER_ADD_METHOD(vecino_t(b, w1, w2));
	lista_adyacentes[b].CONTAINER_ADD_METHOD(vecino_t(a, w1, w2));
}

void armar_grafo(lista_adyacencia_t& lista_adyacentes, int cantNodos){
	//Sea G = (V, E) inicializar lista_adyacentes(n), donde n = #(V), para cada
    //i natural tal que 0<=i<n es el i-esimo nodo del grafo G en el conjunto V,
    //se agrega a la lista de nodos en lista_adyacentes[i](recordar es vector<VECINOS_CONTAINER<nodo>>)
    //cada vecino de ese nodo.

	//agregar_arista(nodo a, nodo b, costo w1, costo w2)
	agregar_arista(0, 1, 2, 3);
	agregar_arista(0, 2, 2, 3);


    //for(int i=0;i<cantNodos;i++){
    //	cout << "Vecinos de " << i << endl;
    //	const VECINOS_CONTAINER<vecino_t> &vecinos = lista_adyacentes[i];
    //    for (VECINOS_CONTAINER<vecino_t>::const_iterator vecino_iter = vecinos.begin(); vecino_iter != vecinos.end(); ++vecino_iter){
    //    	cout << vecino_iter->nodo_index << endl;
    //    }
    //}
}

//void custom_set_intersection(VECINOS_CONTAINER<vecino_t>::const_iterator first1, VECINOS_CONTAINER<vecino_t>::const_iterator last1,
//                          VECINOS_CONTAINER<vecino_t>::const_iterator first2, VECINOS_CONTAINER<vecino_t>::const_iterator last2,
//                          VECINOS_CONTAINER<vecino_t>& vecinosEnComun)
//{
//    while (first1 != last1 && first2 != last2) {
//        if (*first1 < *first2) {
//            ++first1;
//        } else  {
//            if (!(*first2 < *first1)) {
//                vecinosEnComun.CONTAINER_ADD_METHOD(*first1++);
//            }
//            ++first2;
//        }
//    }
//}

//VECINOS_CONTAINER<vecino_t> obtenerVecinosEnComun(nodo_t k, nodo_t k_sig, lista_adyacencia_t& lista_adyacentes){
//	const VECINOS_CONTAINER<vecino_t> &vecinos_k = lista_adyacentes[k];
//	const VECINOS_CONTAINER<vecino_t> &vecinos_k_sig = lista_adyacentes[k_sig];
//	VECINOS_CONTAINER<vecino_t> vecinosEnComun;
//	custom_set_intersection(vecinos_k.begin(), vecinos_k.end(),
//							 vecinos_k_sig.begin(), vecinos_k_sig.end(), 
//							 vecinosEnComun);
//	return vecinosEnComun;
//}

//void busquedaLocal(list<nodo_t> camino, lista_adyacencia_t& lista_adyacentes){
//	
//	list<nodo_t>::const_iterator it = camino.begin();
//	list<nodo_t>::const_iterator runner_it = camino.begin();
//	runner_it++;
//	list<nodo_t>::const_iterator final_camino = camino.end();
//	while(runner_it != final_camino){
//		cout << "Buscando mejorar la conexion (" << *it << ", " << *runner_it << ")" << endl;
//
//		//busco alguna conexion de 2 aristas entre vecinos en comun tal que la suma de esas 2 aristas
//		//sea menor al peso de la arista directa
//
//		//necesito los costos it->vecinos_it y vecinos_it->runner_it
//		//es decir, los costos de las aristas del camino entre it y runner_it			
//
//		VECINOS_CONTAINER<vecino_t> vecinosEnComun = obtenerVecinosEnComun(*(it), *(runner_it), lista_adyacentes);
//		VECINOS_CONTAINER<vecino_t>::const_iterator vecinos_it = vecinosEnComun.begin();
//		VECINOS_CONTAINER<vecino_t>::const_iterator final_vecinos = vecinosEnComun.end();
//		cout << "Vecinos en comun de (" << *it << ") y (" << *runner_it << ")" << endl;
//		while(vecinos_it != final_vecinos){
//			cout << vecinos_it->nodo_index << endl;
//
//			++vecinos_it;
//		}
//		++it;
//		++runner_it;
//	}
//}

int main(int argc, char **argv){
	//Sea G=(V,E) un grafo,
	//Sean w1,w2  dos funciones de costo en las aristas del grafo y sea K en R un limite para la funcion w1
	//Quiero un camino minimo sobre w2 pero tal que w1<K entre dos nodos A y B
	//Dado un grafo G, primero voy a buscar un camino inicial factible.
	//Para esto voy a hacer dijkstra desde A, y me voy a quedar con el camino de A hasta B que minimice w1
	//si dicho camino se pasa de la cota K entonces no existe solucion.
	//Caso contrario, este camino sera la solucion inicial factible para comenzar la busqueda local
	
	//--------------------------------- Parametros del algoritmo -------------------------
	nodo_t nodo_src = 0;
	nodo_t nodo_dst = 4;
	costo_t limit_w1 = 21;

	//--------------------------------- Obtengo el grafo ---------------------------------
	int cantNodos = 6;
 	lista_adyacencia_t lista_adyacentes(cantNodos);
	armar_grafo(lista_adyacentes, cantNodos);

	//--------------------------------- Busco solucion inicial ---------------------------
    vector<costo_t> distancia;
    vector<vecino_t> predecesores;
    //calcular arbol de caminos minimos desde nodo_src
    dijkstra_caminos_minimos(nodo_src, lista_adyacentes, distancia, predecesores);
    cout << "Distancia minima desde (" << nodo_src << ") hasta (" << nodo_dst << "): " << distancia[nodo_dst] << endl;
    
    //--------------------------------- Valido la factibilidad de la solucion----------------
    cout << "Se requiere un camino entre (" << nodo_src << ") y (" << nodo_dst<< ") que no exceda el costo " << limit_w1;
    if(distancia[nodo_dst] == costo_infinito){
		cout << "...Fail!!" << endl;
    	cerr << "No existe solucion factible. No existe camino entre origen(" << nodo_src << ") y destino(" << nodo_dst << ") " << endl;
    	exit(1);
    }else if(distancia[nodo_dst] > limit_w1){
    	cout << "...Fail!!" << endl;
    	cerr << "No existe solucion factible. El camino minimo respecto a w1 de origen(" << nodo_src << ") a destino(" << nodo_dst << ") es de costo " << distancia[nodo_dst] << endl;
    	exit(1);
    }else{
    	cout << "...Ok!!" << endl;
    }

    //--------------------------------- Construyo solucion inicial --------------------------    
    list<vecino_t> camino = dijkstra_construir_camino(vecino_t(nodo_dst, 0, 0), predecesores);
    cout << "Camino Minimo: ";
    
    //imprimir camino
    list<vecino_t>::const_iterator iterador;
    for (iterador = camino.begin(); iterador != camino.end(); ++iterador) {
        cout << "[idx: "<< iterador->nodo_index << "; peso_w1: " << iterador->costo_w1 << "; peso_w2: " << iterador->costo_w2 << "] -> ";
    }
    cout << "Nil" << endl;

    //--------------------------------- Comienzo la busqueda local --------------------------    
    //busquedaLocal(camino, lista_adyacentes);

    return 0;
}