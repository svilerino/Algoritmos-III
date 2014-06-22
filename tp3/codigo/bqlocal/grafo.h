#ifndef __GRAFO_H__
#define __GRAFO_H__

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <queue>
#include <string>
#include <limits>

using namespace std;

typedef int nodo_t;
typedef int longuitud_t;
typedef double distancia_t;
typedef double costo_t;
typedef enum tipo_costo_t {COSTO_W1, COSTO_W2} tipo_costo_t;

const costo_t costo_infinito = numeric_limits<double>::infinity();
const distancia_t distancia_infinita = numeric_limits<double>::infinity();
const costo_t costo_nulo = 0;
const nodo_t predecesor_nulo = -1;

class Arista{
private:
	bool presente;
	costo_t costo_w1;
	costo_t costo_w2;
public:
	Arista();
	Arista(bool pres, costo_t w1, costo_t w2);
	~Arista();

	bool esta_presente();
	void desmarcar_presente();
	void marcar_presente(costo_t w1, costo_t w2);
	costo_t obtener_costo_w1();
	costo_t obtener_costo_w2();

	bool operator== (const Arista &other) const
	{
	    return (other.presente == this->presente &&
	            other.costo_w1 == this->costo_w1 &&
	            other.costo_w2 == this->costo_w2);
	}
};

typedef vector<vector<Arista> > matriz_adyacencia_t;
typedef vector<list<pair<nodo_t, Arista> > > lista_adyacencia_t;

class Vecino{
private:
	nodo_t i;
	nodo_t j;
	nodo_t en_comun;
	Arista desde_i_a_comun;
	Arista desde_j_a_comun;
public:
	Vecino(nodo_t i, nodo_t j, nodo_t comun, Arista desde_i, Arista desde_j);
	Vecino();
	~Vecino();
	nodo_t obtener_nodo_i();
	nodo_t obtener_nodo_j();
	nodo_t obtener_nodo_comun();
	Arista obtener_arista_i_comun();
	Arista obtener_arista_j_comun();
};

class Camino{
private:
	list<nodo_t> camino;
	costo_t costo_camino_w1;
	costo_t costo_camino_w2;
	matriz_adyacencia_t mat_adyacencia;
public:
	Camino();
	Camino(matriz_adyacencia_t& mat_adyacencia);	
	~Camino();

	void agregar_nodo(nodo_t target);
	void agregar_nodo_adelante(nodo_t target);
	costo_t obtener_costo_total_w1_camino();
	costo_t obtener_costo_total_w2_camino();
	//pre: 0 <= i <= j < cantidad_nodos y que i,j sean adyacentes
	costo_t obtener_costo_w1_entre_nodos(nodo_t i, nodo_t j);
	//pre: 0 <= i <= j < cantidad_nodos y que i,j sean adyacentes
	costo_t obtener_costo_w2_entre_nodos(nodo_t i, nodo_t j);

	void imprimir_camino(ostream& out);

	list<nodo_t>::iterator obtener_iterador_begin();
	list<nodo_t>::iterator obtener_iterador_end();
	list<nodo_t>::const_iterator obtener_iterador_const_begin();
	list<nodo_t>::const_iterator obtener_iterador_const_end();
	longuitud_t obtener_longuitud_camino();

	//pre: at.obtener_nodo_i() y at.obtener_nodo_i() deben pertenecer al camino
	//Se reemplazara la conexion directa entre i y j por i -> encomun -> j indicado por el Vecino pasado
	//por parametro. Devuelve true si se inserto, false sino.
	bool insertar_nodo(Vecino& at);

	//pre: at.obtener_nodo_i() y at.obtener_nodo_i() deben pertenecer al camino
	//Se reemplazara la conexion i..loquesea..j por i -> encomun -> j indicado por el Vecino pasado
	//por parametro. Devuelve true si se inserto, false sino.
	bool mejorar_tripla(Vecino& at);

	bool realizar_salto_entre_3_nodos(nodo_t punto_de_salto);
};

class Grafo{
private:
	//atributos
	matriz_adyacencia_t mat_adyacencia;
	lista_adyacencia_t lista_adyacencia;
	int cantidad_nodos;
	int cantidad_aristas;
	
	//atributos propios del problema
	nodo_t nodo_src;
	nodo_t nodo_dst;
	costo_t cota_w1;
	Camino camino_obtenido;
	//dado el camino, podemos obtener los pesos de cada "salto" indexando en la matriz de adyacencia el costo de cada salto
	//tanto para w1 como w2

	//metodos auxiliares
	bool mejorar_conexion_entre_pares(nodo_t nodo_i, nodo_t nodo_j, costo_t costo_ij_w1, costo_t costo_ij_w2, costo_t total_w1, costo_t total_w2,
	 Vecino& mejor_vecino);

	bool mejorar_conexion_salteando(nodo_t nodo_i, nodo_t nodo_j, costo_t costo_ij_w1, costo_t costo_ij_w2, costo_t total_w1, costo_t total_w2,
	 Arista& mejor_vecino);

public:
	//constructor y destructor
	Grafo(int cant_inicial_nodos);
	~Grafo();
	
	//Modificadores
	void agregar_nodos(int cantidad_nodos);
	void agregar_arista(nodo_t i, nodo_t j, costo_t w1, costo_t w2);
	void quitar_arista(nodo_t i, nodo_t j);

	//Consultas
	vector<Arista> obtener_vector_fila_vecinos(nodo_t target);
	list<pair<nodo_t, Arista> > obtener_lista_vecinos(nodo_t target);
	int obtener_cantidad_nodos();
	int obtener_cantidad_aristas();
	Arista obtener_arista(nodo_t i, nodo_t j);	
	//pre: 0 <= i <= j < cantidad_nodos y que i,j sean adyacentes
	list<Vecino> obtener_adyacentes_en_comun(nodo_t i, nodo_t j);
	nodo_t obtener_nodo_origen();
	nodo_t obtener_nodo_destino();
	costo_t obtener_limite_w1();
	Camino& obtener_camino_solucion();

	//Entrada - Salida
	void imprimir_matriz_adyacencia(ostream& out);
	void imprimir_lista_adyacencia(ostream& out);
	void serialize(ostream& out);
	void unserialize(istream& in);

	//Algoritmos
	//Realiza la busqueda local sobre una solucion inicial factible creada por dijkstra sobre COSTO_W1 entre src y dst
	bool busqueda_local_entre_pares_insertando();
	bool busqueda_local_entre_triplas_salteando();
	bool busqueda_local_entre_triplas_reemplazando_intermedio();
	//Devuelve el camino minimo entre origen y destino(calcula el arbol, pero reconstruye solo el camino de origen a destino)
	Camino& dijkstra(nodo_t origen, nodo_t destino, tipo_costo_t target_a_minimizar);
	//Devuelve el camino minimo entre origen y destino y quedan modificados por referencia los costos_minimos y los predecesores de todos los nodos de G
	Camino& dijkstra(nodo_t origen, nodo_t destino, tipo_costo_t target_a_minimizar, vector<costo_t>& costo_minimo, vector<nodo_t>& predecesor);
	//Dado un nodo_t origen se calcula para cada nodo, la distancia minima en cantidad de aristas de peso constante 1 de cualquier nodo a origen
	void breadth_first_search(nodo_t origen, vector<distancia_t>& distancias_en_aristas_a_origen);
};

#endif
