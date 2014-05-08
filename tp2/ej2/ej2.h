#ifndef __EJ2_H__
#define __EJ2_H__

#define REPETICIONES_CALCULAR_TIEMPO (200)

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

typedef int Nodo;

struct GrafoMatrizAdyacencia{
	int nodos; //cantidad de nodos
	int *componente_conexa_nodos; //array para asignar componentes conexas
	int componentes_conexas;
	int **adyacencia; //la matriz ed adyacencia
};

typedef struct GrafoMatrizAdyacencia Grafo;

//nodos = cantidad de nodos del grafo. crea el grafo con nodos que van de 0 a nodos - 1
Grafo *crear_grafo(int nodos);
void liberar_grafo(Grafo *g);
//nodo1 y nodo2 tienen que ir de 0 a nodos - 1
int agregar_arista(Grafo *g, Nodo nodo1, Nodo nodo2);
int cantidad_componentes_conexas(Grafo *g);
//retorna un vector con tantos nodos en el grafo como elementos elementos, en cada iesimo elemento, hay un nodo correspondiente a la componente iesima. si esa componente conexa ya no existe, se pone el valor -1. liberar el resultado con free()
Nodo *nodos_de_componentes(Grafo *g);

int resolver(void);

#endif //__EJ2_H__

