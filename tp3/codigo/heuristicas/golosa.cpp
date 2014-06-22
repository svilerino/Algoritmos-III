#include "grafo.h"
// -------------- Main ---------------------------------

int main(int argc, char **argv){
    //---------------------------- Obtengo el grafo y los parametros ----------------
    list<Grafo> instancias = Grafo::parsear_varias_instancias();

    //TODO POR AHORA SE PROCESA LA PRIMERA INSTANCIA PARSEADA NOMAS!!

    Grafo g = instancias.front();
    //g.imprimir_matriz_adyacencia(cout);
	
    Camino camino = g.obtener_solucion_golosa(DETERMINISTICO);
    g.establecer_camino_solucion(camino);

    cout << endl << "Camino obtenido con golosa" << endl;
    camino.imprimir_camino(cout);

    cout << "Salida del algoritmo: " << endl;
    g.serialize(cout);
    cout << endl;
    return 0;
}