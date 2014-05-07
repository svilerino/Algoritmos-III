#ifndef __TP1_EJ3_H__
#define __TP1_EJ3_H__

#ifndef NULL
#define NULL (void *)
#endif

#define REPETICIONES_CALCULAR_TIEMPO (10)
typedef enum Como_Imprimir{
	COMO_PIDE_TP,
	COMO_UN_TABLERO
} Como_Imprimir;

typedef struct Ficha {
	int numero; //-1 = espacio vacio
	int arriba;
	int abajo;
	int izquierda;
	int derecha;
} Ficha;

struct Lista {
	void *nodo;
	struct Lista *anterior;
	struct Lista *siguiente;
};

typedef struct Lista Lista;

#endif

