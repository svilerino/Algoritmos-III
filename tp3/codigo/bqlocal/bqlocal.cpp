#include "grafo.h"
// -------------- Main ---------------------------------

int main(int argc, char **argv){
    //---------------------------- Obtengo el grafo y los parametros ----------------
    Grafo g(0);
    g.unserialize(cin);
    costo_t limit_w1 = g.obtener_limite_w1();
	nodo_t nodo_src = g.obtener_nodo_origen();
	nodo_t nodo_dst = g.obtener_nodo_destino();
    g.imprimir_matriz_adyacencia(cout);
	
	//--------------------------------- Busco solucion inicial ----------------------
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
        cout << ". Costo minimo obtenido: " << c.obtener_costo_total_w1_camino();
        cout << "...Ok!!" << endl;
    }

    //imprimir camino
    c.imprimir_camino(cout);

    //--------------------------------- Comienzo la busqueda local -------------------    
    g.busqueda_local_entre_pares_insertando();

    cout << "Salida del algoritmo: " << endl;
    g.serialize(cout);
    cout << endl;
    return 0;
}
