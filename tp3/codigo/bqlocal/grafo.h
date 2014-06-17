#ifndef __GRAFO_H__
#define __GRAFO_H__

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <string>
#include <limits>

using namespace std;

typedef int nodo_t;
typedef double costo_t;
typedef enum tipo_costo_t {COSTO_W1, COSTO_W2} tipo_costo_t;

const costo_t costo_infinito = numeric_limits<double>::infinity();
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
};

typedef vector<vector<Arista> > matriz_adyacencia_t;

class Camino{
private:
	list<nodo_t> camino;
	costo_t costo_camino_w1;
	costo_t costo_camino_w2;
	matriz_adyacencia_t mat_adyacencia;
public:
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

	list<nodo_t>::const_iterator obtener_iterador_begin();
	list<nodo_t>::const_iterator obtener_iterador_end();
};

class Vecino{
private:
	nodo_t i;
	nodo_t j;
	nodo_t en_comun;
	Arista desde_i_a_comun;
	Arista desde_j_a_comun;
public:
	Vecino(nodo_t i, nodo_t j, nodo_t comun, Arista desde_i, Arista desde_j);
	~Vecino();
	nodo_t obtener_nodo_i();
	nodo_t obtener_nodo_j();
	nodo_t obtener_nodo_comun();
	Arista obtener_arista_i_comun();
	Arista obtener_arista_j_comun();
};

class Grafo{
private:
	//atributos
	matriz_adyacencia_t mat_adyacencia;
	int cantidad_nodos;
	int cantidad_aristas;
	
	//atributos propios del problema
	nodo_t nodo_src;
	nodo_t nodo_dst;
	costo_t cota_w1;
	costo_t costo_obtenido_w1;
	costo_t costo_obtenido_w2;	
	list<nodo_t> camino_obtenido;
	//dado el camino, podemos obtener los pesos de cada "salto" indexando en la matriz de adyacencia el costo de cada salto
	//tanto para w1 como w2

public:
	//constructor y destructor
	Grafo(int cant_inicial_nodos);
	~Grafo();
	
	//Modificadores
	void agregar_nodos(int cantidad_nodos);
	void agregar_arista(nodo_t i, nodo_t j, costo_t w1, costo_t w2);
	void quitar_arista(nodo_t i, nodo_t j);

	//Consultas
	Arista obtener_arista(nodo_t i, nodo_t j);	
	//pre: 0 <= i <= j < cantidad_nodos y que i,j sean adyacentes
	list<Vecino> obtener_adyacentes_en_comun(nodo_t i, nodo_t j);

	//Entrada - Salida
	void imprimir_matriz_adyacencia(ostream& out);
	void serialize(ostream& out);
	void unserialize(istream& in);

	//Algoritmos
	Camino dijkstra(nodo_t origen, nodo_t destino, tipo_costo_t target_a_minimizar);
};

#endif
