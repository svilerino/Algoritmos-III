#include "grafo.h"
#include "timing.h"
#define CANT_ITERS_MEDICION 1//ojo que como es randomized la greedy no tiene sentido repetirlo mas de una vez!

void ejecutar_grasp(Grafo &g);

// -------------- Main ---------------------------------
int main(int argc, char **argv){
    list<Grafo> instancias = Grafo::parsear_varias_instancias(FORMATO_1_N_CLOSED);
    uint64_t instance_number = 1;
    for(Grafo &g : instancias){
        cout << endl << endl << "Aplicando metaheuristica GRASP a la " << instance_number << "-esima instancia de input..." << endl;
        ejecutar_grasp(g);
        instance_number++;
    }
    return 0;
}

void ejecutar_grasp(Grafo &g){
    //g.imprimir_lista_adyacencia(cout);
    //g.imprimir_matriz_adyacencia(cout);

    //----- Configuracion del criterio de terminacion de GRASP -----

    //typedef enum criterio_terminacion_grasp_t {CRT_K_ITERS_SIN_MEJORA, CRT_K_ITERS_LIMIT_REACHED, CRT_SOLUTION_GOOD_ENOUGH} criterio_terminacion_grasp_t;
    criterio_terminacion_grasp_t criterio_terminacion = CRT_K_ITERS_SIN_MEJORA;
    //este parametro denota la cantidad de iteraciones maxima, dependiendo del tipo de criterio, de cantidad fija de iteraciones o cantidad de iters
    //consecutivas sin mejora
    uint64_t ITERS_LIMIT = 100;
    //este parametro denota el valor aceptable de la funcion objetivo w2 a partir del cual, dejamos de mejorar la solucion y consideramos que es lo suficientemente buena
    //NOTA: ESTO SOLO TIENE SENTIDO SI PARA LA INSTANCIA SE CONOCE UNA COTA CONOCIDA!
    //USAR CON CUIDADO, O IGNORAR. PUEDE NO TERMINAR NUNCA, LO DEJO IMPLEMENTADO POR CUESTIONES DE EXPERIMENTACION NOMAS. ACLARAR EN EL INFORME!
    costo_t W2_VALUE_TARGET = 5;

    //----- Configuracion de los modos de la busqueda local y golosa -----

    //typedef enum tipo_ejecucion_bqlocal_t {BQL_SUBDIVIDIR_PARES, BQL_CONTRAER_TRIPLAS_A_PARES, BQL_MEJORAR_CONEXION_TRIPLAS} tipo_ejecucion_bqlocal_t;
    //typedef enum tipo_ejecucion_golosa_t {RCL_DETERMINISTICO, RCL_POR_VALOR, RCL_POR_CANTIDAD} tipo_ejecucion_golosa_t;    
    tipo_ejecucion_bqlocal_t modo_busqueda_local = BQL_CONTRAER_TRIPLAS_A_PARES;
    tipo_ejecucion_golosa_t modo_golosa = RCL_POR_CANTIDAD;
    //si el tipo de golosa es RCL_POR_VALOR, este parametro indica el porcentaje de alejamiento del minimo de los candidatos de la lista
    //mas formalmente filtra todos los candidatos factibles locales que no cumplan candidato->costo_w2 <= valor_limite
    //donde valor limite es  (parametro_beta + 1) * minimo.second.obtener_costo_w2();
    //si el tipo de golosa es RCL_POR_CANTIDAD, este parametro indica la cantidad min{cant_candidatos, parametro_beta} de soluciones a considerar en la lista
    //si el tipo es RCL_DETERMINISTICO, este parametro es ignorado por el metodo.
    //double parametro_beta = 1.5;
    double parametro_beta = 2;

    //-------------------------------------------------------

    bool condicion_terminacion = false;
    Camino mejor_solucion = g.obtener_camino_vacio();
    costo_t costo_mejor_solucion = costo_infinito;
    uint64_t cant_iters = 0;
    uint64_t cant_iters_sin_mejora = 0;
    double promedio_parcial = 0;
    double promedio = 0;
    Camino camino = g.obtener_camino_vacio();
    do{
        MEDIR_TIEMPO_PROMEDIO(
            camino = g.obtener_solucion_golosa(modo_golosa, parametro_beta);
            //cout << "Solucion inicial de la greedy:" << endl;
            //camino.imprimir_camino(cout);
            g.establecer_camino_solucion(camino);

            //hago iteraciones de busqueda local hasta que no haya mejora(la funcion devuelve true si hubo mejora, false sino)   
            while(g.busqueda_local(modo_busqueda_local));
        , CANT_ITERS_MEDICION, &promedio_parcial);
        promedio += promedio_parcial;
        //en este punto la bqlocal mejoro todo lo que pudo la sol. inicial obtenida con la randomized greedy

        //me fijo si esta solucion es mejor que la que tenia guardada, de ser asi actualizo el maximo y guardo la sol actual como la nueva mejor.
        costo_t costo_solucion_actual = camino.obtener_costo_total_w2_camino();
        if(costo_solucion_actual < costo_mejor_solucion){
            costo_mejor_solucion = costo_solucion_actual;
            mejor_solucion = g.obtener_camino_solucion();
            //reseteo el contador
            cant_iters_sin_mejora = 0;
        }else{
            //una iteracion consecutiva mas sin mejora
            cant_iters_sin_mejora++;
        }

        cant_iters++;

        if(criterio_terminacion == CRT_K_ITERS_LIMIT_REACHED){
            condicion_terminacion = (cant_iters < ITERS_LIMIT);            
        }else if(criterio_terminacion == CRT_K_ITERS_SIN_MEJORA){
            condicion_terminacion = (cant_iters_sin_mejora < ITERS_LIMIT);
        }else if(criterio_terminacion == CRT_SOLUTION_GOOD_ENOUGH){
            condicion_terminacion = (costo_mejor_solucion < W2_VALUE_TARGET);
        }
    }while(!condicion_terminacion);
    promedio = promedio / (double) cant_iters;

    //imprimo mediciones en stderr
    cerr << g.obtener_cantidad_nodos() << " " << g.obtener_cantidad_aristas() << " " << cant_iters << " " << promedio << endl;

    cout << endl << "Se cumplio el criterio de terminacion elegido: ";
    if(criterio_terminacion == CRT_K_ITERS_LIMIT_REACHED){
        cout << "CRT_K_ITERS_LIMIT_REACHED";
    }else if(criterio_terminacion == CRT_K_ITERS_SIN_MEJORA){
        cout << "CRT_K_ITERS_SIN_MEJORA";
    }else if(criterio_terminacion == CRT_SOLUTION_GOOD_ENOUGH){
        cout << "CRT_SOLUTION_GOOD_ENOUGH";
    }
    cout << endl;

    g.establecer_se_encontro_solucion(true);

    cout << "Salida del algoritmo: " << endl;
    g.serialize(cout, FORMATO_1_N_CLOSED);
    cout << endl;
}