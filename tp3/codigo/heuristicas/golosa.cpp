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

    while(actual != nodo_dst)
    {
        list<pair<nodo_t, Arista> > vecinos = g.obtener_lista_vecinos(actual);
        if(vecinos.empty()){
            break;
        }

        list<pair<nodo_t, Arista> >::iterator incidentes_i_it = vecinos.begin();
        list<pair<nodo_t, Arista> >::iterator final_it = vecinos.end();

        pair<nodo_t, Arista> minimo = *incidentes_i_it;

        while(incidentes_i_it != final_it)
        {
                if( (costos[incidentes_i_it->first] + costoCamino <= limit_w1) && 
                    ((incidentes_i_it->second).obtener_costo_w1() <= (minimo.second).obtener_costo_w1()) &&
                     (distancias[incidentes_i_it->first] < distanciaLlegada))
                {
                    minimo = *incidentes_i_it;
                }

                camino.agregar_nodo(incidentes_i_it->first);
                costoCamino += (incidentes_i_it->second).obtener_costo_w1();
                actual = incidentes_i_it->first;
                distanciaLlegada--;
        }
    }

   // camino.agregar_nodo(final_it->first);

    cout << "Salida del algoritmo: " << endl;
    camino.imprimir_camino(cout);
    cout << endl;
    return 0;
}
