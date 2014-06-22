#include "grafo.h"
// -------------- Main ---------------------------------

int main(int argc, char **argv){
    //---------------------------- Obtengo el grafo y los parametros ----------------
    list<Grafo> instancias = Grafo::parsear_varias_instancias();

    //TODO POR AHORA SE PROCESA LA PRIMERA INSTANCIA PARSEADA NOMAS!!
    Grafo g = instancias.front();
	
    double parametro_beta = 1.5;
    bool hay_mejora = false;
    do{
    	//typedef enum tipo_ejecucion_golosa_t {DETERMINISTICO, RCL_POR_VALOR, RCL_POR_CANTIDAD} tipo_ejecucion_golosa_t;
    	Camino camino = g.obtener_solucion_golosa(RCL_POR_VALOR, parametro_beta);
    	cout << "Solucion inicial de la greedy:" << endl;
    	camino.imprimir_camino(cout);
		g.establecer_camino_solucion(camino);
        //hay_mejora = g.busqueda_local_entre_pares_insertando();
        //hay_mejora = g.busqueda_local_entre_triplas_reemplazando_intermedio();
        hay_mejora = g.busqueda_local_entre_triplas_salteando();
    }while(hay_mejora);

    cout << "Salida del algoritmo: " << endl;
    g.serialize(cout);
    cout << endl;
    return 0;
}