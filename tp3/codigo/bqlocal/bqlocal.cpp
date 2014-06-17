#include "grafo.h"

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

void busquedaLocal(Grafo& g, Camino& camino){
	list<nodo_t>::const_iterator it = camino.obtener_iterador_begin();
	list<nodo_t>::const_iterator runner_it = camino.obtener_iterador_begin();
	runner_it++;
	list<nodo_t>::const_iterator final_camino = camino.obtener_iterador_end();
	while(runner_it != final_camino){
        nodo_t nodo_i = *it;
        nodo_t nodo_j = *runner_it;
        costo_t costo_ij = camino.obtener_costo_w1_entre_nodos(nodo_i, nodo_j);
		cout << "Buscando mejorar la conexion (" << nodo_i << ", " << nodo_j << ")" << endl;
        cout << "Costo actual w1 de la arista (" << nodo_i << ", " << nodo_j << "): " << costo_ij << endl;
		//busco alguna conexion de 2 aristas entre vecinos en comun tal que la suma de esas 2 aristas
		//sea menor al peso de la arista directa

        list<Vecino> vecinosEnComun = g.obtener_adyacentes_en_comun(nodo_i, nodo_j);
		list<Vecino>::iterator vecinos_it = vecinosEnComun.begin();
		list<Vecino>::iterator final_vecinos = vecinosEnComun.end();
		cout << "Vecinos en comun de (" << nodo_i << ") y (" << nodo_j << ")" << endl;
		while(vecinos_it != final_vecinos){
			cout << vecinos_it->obtener_nodo_comun() << endl;

			++vecinos_it;
		}
		++it;
		++runner_it;
	}
}

// -------------- Main ---------------------------------

int main(int argc, char **argv){	
	//--------------------------------- Parametros del algoritmo -------------------------
	nodo_t nodo_src = 0;
	nodo_t nodo_dst = 2;
	costo_t limit_w1 = 21;

	//--------------------------------- Obtengo el grafo ---------------------------------
    Grafo g(0);
    g.unserialize(cin);
    g.imprimir_matriz_adyacencia(cout);
	
	//--------------------------------- Busco solucion inicial ---------------------------
    Camino c = g.dijkstra(nodo_src, nodo_dst, COSTO_W1);    

    //--------------------------------- Valido la factibilidad de la solucion----------------
    cout << "Se requiere un camino entre (" << nodo_src << ") y (" << nodo_dst<< ") que no exceda el costo " << limit_w1;
    if(c.obtener_costo_total_w1_camino() == costo_infinito){
        cout << "...Fail!!" << endl;
        cerr << "No existe solucion factible. No existe camino entre origen(" << nodo_src << ") y destino(" << nodo_dst << ") " << endl;
        exit(1);
    }else if(c.obtener_costo_total_w1_camino() > limit_w1){
        cout << "...Fail!!" << endl;
        cerr << "No existe solucion factible. El camino minimo respecto a w1 de origen(" << nodo_src << ") a destino(" << nodo_dst << ") es de costo " << c.obtener_costo_total_w1_camino() << endl;
        exit(1);
    }else{
        cout << "...Ok!!" << endl;
    }

    cout << "Camino de costo minimo sobre w1 desde (" << nodo_src << ") hasta (" << nodo_dst << "): " << c.obtener_costo_total_w1_camino() << endl;
    //imprimir camino
    c.imprimir_camino(cout);

    //--------------------------------- Comienzo la busqueda local --------------------------    
    cout << endl << endl;
    busquedaLocal(g, c);
    return 0;
}
