#ifndef __TIMING_H__
#define __TIMING_H__
#include <chrono>

/*
    Se le pasa por execute_target la porcion de codigo a medir
    y en cant_repeticiones la cantidad de veces para tomar el promedio
    queda definida una variable promedio escrita como contenido del puntero pasado en out_double_variable_ptr
*/
#define MEDIR_TIEMPO_PROMEDIO(execute_target, cant_repeticiones, out_double_variable_ptr)\
    chrono::time_point<chrono::high_resolution_clock> start, end; \
    start = chrono::high_resolution_clock::now(); \
    for(int j=0;j<cant_repeticiones;j++){ \
        execute_target \
    } \
    end = chrono::high_resolution_clock::now(); \
    int elapsed = chrono::duration_cast<chrono::microseconds>(end-start).count(); \
    *out_double_variable_ptr = (double) ( elapsed/(double)cant_repeticiones );


#endif  /* !__TIMING_H__ */
