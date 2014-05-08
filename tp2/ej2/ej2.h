#ifndef __EJ2_H__
#define __EJ2_H__

#define REPETICIONES_CALCULAR_TIEMPO (100)

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
	int aristas; //aristas colocadas
};

typedef struct GrafoMatrizAdyacencia Grafo;

typedef struct Ciudad {
	Nodo nodo;
	int x;
	int y;
} Ciudad;

typedef struct NodoDistancia {
	int agregado;
	int distancia;
	Nodo nodo;
} NodoDistancia;

typedef struct Arista{
	Nodo nodo1;
	Nodo nodo2;
	int distancia;
} Arista;

//nodos = cantidad de nodos del grafo. crea el grafo con nodos que van de 0 a nodos - 1
Grafo *crear_grafo(int nodos);
void liberar_grafo(Grafo *g);
//nodo1 y nodo2 tienen que ir de 0 a nodos - 1
int agregar_arista(Grafo *g, Nodo nodo1, Nodo nodo2);
int cantidad_componentes_conexas(Grafo *g);
int cantidad_aristas(Grafo *g);
int cantidad_nodos(Grafo *g);
//retorna 0 si no son adyacentes, 1 si son adyacentes
int son_adyacentes(Grafo *g, Nodo nodo1, Nodo nodo2);
//retorna un vector con tantos nodos en el grafo como elementos elementos, en cada iesimo elemento, hay un nodo correspondiente a la componente iesima. si esa componente conexa ya no existe, se pone el valor -1. liberar el resultado con free()
Nodo *nodos_de_componentes(Grafo *g);

int distancia(Ciudad *c1, Ciudad *c2);

//liberar con free()
Ciudad *cargar_datos(FILE *f, int *n_ciudades, int *k_centrales);

void ordenar_aristas(Arista *aristas, int n);
Grafo *resolver(int k_centrales, Ciudad *ciudades, int n_ciudades, Nodo **centrales);

#endif //__EJ2_H__

