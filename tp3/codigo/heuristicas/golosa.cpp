#include "grafo.h"
// -------------- Main ---------------------------------

int main(int argc, char **argv){
    //---------------------------- Obtengo el grafo y los parametros ----------------
    list<Grafo> instancias = Grafo::parsear_varias_instancias();

    //TODO POR AHORA SE PROCESA LA PRIMERA INSTANCIA PARSEADA NOMAS!!

    Grafo g = instancias.front();
    //g.imprimir_matriz_adyacencia(cout);
	
    double parametro_beta=1;
    //typedef enum tipo_ejecucion_golosa_t {RCL_DETERMINISTICO, RCL_POR_VALOR, RCL_POR_CANTIDAD} tipo_ejecucion_golosa_t;
    Camino camino = g.obtener_solucion_golosa(RCL_DETERMINISTICO, parametro_beta);
    g.establecer_camino_solucion(camino);

    cout << endl << "Camino obtenido con golosa" << endl;
    camino.imprimir_camino(cout);

    cout << "Salida del algoritmo: " << endl;
    g.serialize(cout);
    cout << endl;
    return 0;
}