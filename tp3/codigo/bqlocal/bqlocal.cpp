#include "grafo.h"
// -------------- Main ---------------------------------

int main(int argc, char **argv){
    //---------------------------- Obtengo el grafo y los parametros ----------------
    Grafo g(0);
    g.unserialize(cin);
    costo_t limit_w1 = g.obtener_limite_w1();
	nodo_t nodo_src = g.obtener_nodo_origen();
	nodo_t nodo_dst = g.obtener_nodo_destino();
    //g.imprimir_matriz_adyacencia(cout);
    //g.imprimir_lista_adyacencia(cout);
	
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

    cout << "Camino inicial: ";
    c.imprimir_camino(cout);
    cout << endl;

    //--------------------------------- Comienzo la busqueda local -------------------    
    //hago iteraciones de busqueda local hasta que no haya mejora(la funcion devuelve true si hubo mejora, false sino)
//    while(g.busqueda_local_entre_pares_insertando());
//    cout << "Finalizo la busqueda local insertando entre pares porque no se obtuvo ninguna mejora." << endl;
//    
//    cout << endl << endl << endl << endl << endl;
//    
//    while(g.busqueda_local_entre_triplas_reemplazando_intermedio());
//    cout << "Finalizo la busqueda local reemplazando entre triplas porque no se obtuvo ninguna mejora." << endl;

    while(g.busqueda_local_entre_triplas_salteando());
    cout << "Finalizo la busqueda local salteando entre triplas porque no se obtuvo ninguna mejora." << endl;

//    bool res_local_pares = false;
//    bool res_local_triplas_intermedio = false;
//    bool res_local_triplas_salteando = false;
//    do{
//        res_local_pares = g.busqueda_local_entre_pares_insertando();
//        res_local_triplas_intermedio = g.busqueda_local_entre_triplas_reemplazando_intermedio();
//        res_local_triplas_salteando = g.g.busqueda_local_entre_triplas_salteando();
//    }while(res_local_pares || res_local_triplas_intermedio || res_local_triplas_salteando);

    cout << "Salida del algoritmo: " << endl;
    g.serialize(cout);
    cout << endl;

    ////Ejemplo BFS para obtener caminos minimos en "cantidad de aristas a origen"
    //vector<distancia_t> distancias_en_pasos;
    //g.breadth_first_search(nodo_src, distancias_en_pasos);
    //for(int i=0;i<g.obtener_cantidad_nodos();i++){
    //    cout << "Distancia de ("<< nodo_src << ") a (" << i << "): " << distancias_en_pasos[i] << " aristas" << endl;
    //}
    return 0;
}
