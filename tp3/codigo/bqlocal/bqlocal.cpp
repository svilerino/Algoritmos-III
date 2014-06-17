#include "grafo.h"

void busquedaLocal(Grafo& g, Camino& camino){
	list<nodo_t>::const_iterator it = camino.obtener_iterador_begin();
	list<nodo_t>::const_iterator runner_it = camino.obtener_iterador_begin();
	runner_it++;
	list<nodo_t>::const_iterator final_camino = camino.obtener_iterador_end();
	while(runner_it != final_camino){
        nodo_t nodo_i = *it;
        nodo_t nodo_j = *runner_it;
        costo_t costo_ij_w1 = camino.obtener_costo_w1_entre_nodos(nodo_i, nodo_j);
        costo_t costo_ij_w2 = camino.obtener_costo_w2_entre_nodos(nodo_i, nodo_j);
        cout << "Buscando mejorar la conexion (" << nodo_i << ", " << nodo_j << ") agregando un nodo intermedio. Costos actuales    W1: " << costo_ij_w1 << "     W2: " << costo_ij_w2 << endl;
        //cout << "Costo actual w1 de la arista (" << nodo_i << ", " << nodo_j << "): " << costo_ij_w1 << endl;
        //cout << "Costo actual w2 de la arista (" << nodo_i << ", " << nodo_j << "): " << costo_ij_w2 << endl;
		
        //busco alguna conexion de 2 aristas entre vecinos en comun tal que la suma de esas 2 aristas
		//sea menor al peso de la arista directa

        list<Vecino> vecinosEnComun = g.obtener_adyacentes_en_comun(nodo_i, nodo_j);
		list<Vecino>::iterator vecinos_it = vecinosEnComun.begin();
		list<Vecino>::iterator final_vecinos = vecinosEnComun.end();
		
        cout << "Caminos alternativos agregando un nodo en comun entre (" << nodo_i << ") y (" << nodo_j << ")" << endl;

		while(vecinos_it != final_vecinos){
            nodo_t nodo_comun = vecinos_it->obtener_nodo_comun();

            costo_t i_comun_w1 = vecinos_it->obtener_arista_i_comun().obtener_costo_w1();
            costo_t i_comun_w2 = vecinos_it->obtener_arista_i_comun().obtener_costo_w2();
            
            costo_t j_comun_w1 = vecinos_it->obtener_arista_j_comun().obtener_costo_w1();
            costo_t j_comun_w2 = vecinos_it->obtener_arista_j_comun().obtener_costo_w2();

            costo_t costo_i_comun_j_w1 = i_comun_w1 + j_comun_w1;
            costo_t costo_i_comun_j_w2 = i_comun_w2 + j_comun_w2;
            
            cout << "\tCamino (" << nodo_i << ") --> (" << nodo_comun << ") --> (" << nodo_j << ") Costos Asociados a esta modificacion:    W1: " << costo_i_comun_j_w1 << "     W2: " << costo_i_comun_j_w2 << endl;

			++vecinos_it;
		}
        cout << endl << endl;
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
