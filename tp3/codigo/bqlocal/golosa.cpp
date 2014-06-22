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
	
    //---------------------------- Inicializo los vectores y datos ----------------

    vector<int> costos;
    vector<int> predecesores;
    g.dijkstra(nodo_dst, COSTO_W1, costos, predecesores);

    vector<int> distancias;
    g.breadth_first_search(nodo_dst, distancias);

    Camino camino = g.crear_camino_vacio();
    camino.agregar_nodo(nodo_src);
    int costoCamino = 0;
    int distanciaLlegada = distancias[nodo_src];

    nodo_t actual = nodo_src;

     //--------------------------Comienza la busqueda golosa ----------------

    while(actual != nodo_dst)
    {
        list<pair<nodo_t, Arista> > vecinos = obtener_lista_vecinos(target);
        if(vecinos.empty())
            break;
        list<pair<nodo_t, Arista> >::iterator incidentes_i_it = vecinos.begin();
        list<pair<nodo_t, Arista> >::iterator final_it = vecinos.end();

        pair<nodo_t, Arista> minimo = *incidentes_i_it;

        while(incidentes_i_it != final_it)
        {
                if( (costos(incidentes_i_it->first) + costoCamino <= limit_w1) && (obtener_costo_w1(incidentes_i_it->second) <= minimo.second) && (distancias[incidentes_i_it] < distanciaLlegada))
                {
                    minimo = *incidentes_i_it;
                }

                camino.agregar_nodo(incidentes_i_it->first);
                costoCamino += obtener_costo_w1(incidentes_i_it->second);
                actual = incidentes_i_it->first;
                distanciaLlegada--;

        }

    }

    camino.agregar_nodo(final_it->first);

    cout << "Salida del algoritmo: " << endl;
    camino.imprimir_camino(cout);
    cout << endl;
    return 0;
}
