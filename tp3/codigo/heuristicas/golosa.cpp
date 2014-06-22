#include "grafo.h"
// -------------- Main ---------------------------------

int main(int argc, char **argv){
    //---------------------------- Obtengo el grafo y los parametros ----------------
    list<Grafo> instancias = Grafo::parsear_varias_instancias();

    //TODO POR AHORA SE PROCESA LA PRIMERA INSTANCIA PARSEADA NOMAS!!

    Grafo g = instancias.front();
    costo_t limit_w1 = g.obtener_limite_w1();
	nodo_t nodo_src = g.obtener_nodo_origen();
	nodo_t nodo_dst = g.obtener_nodo_destino();
    g.imprimir_matriz_adyacencia(cout);
	
    //---------------------------- Inicializo los vectores y datos ----------------

    vector<costo_t> costos;
    vector<nodo_t> predecesores;
    g.dijkstra(nodo_dst, COSTO_W1, costos, predecesores);

    vector<distancia_t> distancias;
    g.breadth_first_search(nodo_dst, distancias);

    Camino camino = g.crear_camino_vacio();
    camino.agregar_nodo(nodo_src);
    costo_t costoCamino = 0;
    distancia_t distanciaLlegada = distancias[nodo_src];

    nodo_t actual = nodo_src;

    //--------------------------Comienza la busqueda golosa ----------------

    cout << "Nodo inicial: " << actual << endl;

    while(actual != nodo_dst){
        list<pair<nodo_t, Arista> > vecinos = g.obtener_lista_vecinos(actual);        
        if(vecinos.empty()){
            break;
        }

        cout << "Obteniendo mejor vecino del nodo (" << actual << ") segun decision greedy..." << endl;
        list<pair<nodo_t, Arista> >::iterator incidentes_i_it = vecinos.begin();
        list<pair<nodo_t, Arista> >::iterator final_it = vecinos.end();
        pair<nodo_t, Arista> minimo = vecinos.front();
        while(incidentes_i_it != final_it){
            if( (costos[incidentes_i_it->first] + costoCamino <= limit_w1) && 
                ((incidentes_i_it->second).obtener_costo_w2() <= (minimo.second).obtener_costo_w2()) &&
                 (distancias[incidentes_i_it->first] < distanciaLlegada)){
                minimo = *incidentes_i_it;
            }
            incidentes_i_it++;
        }

        cout << "\tEl mejor vecino segun decision greedy para el nodo (" << actual << ") es (";
        cout << minimo.first << ") con un costo (w1: " << (minimo.second).obtener_costo_w1() << ", w2: " << (minimo.second).obtener_costo_w2() << ")" << endl;
        cout << endl;

        camino.agregar_nodo(minimo.first);
        costoCamino += (minimo.second).obtener_costo_w2();
        actual = minimo.first;
        distanciaLlegada--;
    }    

    cout << "Salida del algoritmo: " << endl;
    camino.imprimir_camino(cout);
    cout << endl;
    return 0;
}
