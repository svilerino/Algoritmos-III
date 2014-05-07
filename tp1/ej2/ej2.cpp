#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string.h>
#include "../common/timing/timing.h"

#define NUM_REPEAT_INSTANCE_AVERAGE 100

using namespace std;

bool compare_function (tuple<int, int, int>, tuple<int, int, int>);
void parsearInstancia(vector<tuple<int, int, int> >&, uint);
void serializarSalida(vector<tuple<int, int, int> >&, uint, uint);
void solveInstance(vector<tuple<int, int, int> >&, uint, uint&);
bool esSolucionValida(vector<tuple<int, int, int> >&);

int main(int argc, char** argv){
	bool medirTiempo = ( (argc>1) && (strcmp(argv[1], "--measure-time") == 0) );

	uint cantidadPiezas;
	cin >> cantidadPiezas;
	while(cantidadPiezas != 0)// si llego a la linea de la entrada que comienza con 0 se termina la entrada de instancias
	{
		//ya tengo la cantidad de piezas, y se que es no nula. parseo el resto de los datos

		// --- Parseo entrada ---
			vector<tuple<int, int, int> > piezas; // tupla < depreciacion, tiempo, posicion en la entrada >
			parsearInstancia(piezas, cantidadPiezas);
		// --- Fin Parseo entrada ---

		// --- Resolucion del problema ---
			uint totalPerdido = 0;			
			if(medirTiempo){
				double promedio = 0.0;
				//macro en timing.h
				MEDIR_TIEMPO_PROMEDIO(solveInstance(piezas, cantidadPiezas, totalPerdido);, NUM_REPEAT_INSTANCE_AVERAGE, &promedio)
				//se envia por stderr el resultado del timing(otra opcion era abrir el archivo desde aca, pero manejo las redirecciones desde los scripts de testing)
				//formato de salida de timing : <cantidad piezas> <tiempo promedio en microsegundos> <cantidad de corridas>
				cerr << cantidadPiezas << " " << promedio << " " << NUM_REPEAT_INSTANCE_AVERAGE << endl;
			}else{
				solveInstance(piezas, cantidadPiezas, totalPerdido);
			}			
		// --- Fin Resolucion del problema ---

		// --- Validacion de la solucion ---
			if(!esSolucionValida(piezas)){
				cout << "Error - la salida no es optima!" << endl;
				exit(1);
			}
		// --- Fin validacion de la solucion ---

		// --- Serializar salida ---
			serializarSalida(piezas, cantidadPiezas, totalPerdido);
		// --- Fin Serializar salida ---

		//leo la proxima linea para procesar la proxima linea(hace falta por la guarda del while)
		cin >> cantidadPiezas;
	}
	return 0;
}

bool esSolucionValida(vector<tuple<int, int, int> >& resultado){
	//tupla < depreciacion, tiempo, posicion en la entrada >
	//La solucion devuelve i1,i2,i3,i4,...in con las piezas en orden de forma tal que son optimas
	//Sea f(ik) = ti_k / d_ik 
	//El resultado deberia estar ordenado de forma tal que i <= j => f(i_k) <= f(i_j) que por el teorema del informe sabemos que es la sol. optima.
	return is_sorted(resultado.begin(), resultado.end(), compare_function);
}

bool compare_function (tuple<int, int, int> e1, tuple<int, int, int> e2){
	//tupla < depreciacion, tiempo, posicion en la entrada >
	return (get<1>(e1)*get<0>(e2)) > (get<0>(e1)*get<1>(e2));
}

void parsearInstancia(vector<tuple<int, int, int> >& piezas, uint cantidadPiezas){
	int di, ti;
	for(uint i = 0; i < cantidadPiezas; i++)
	{
		cin >> di;
		cin >> ti;
		piezas.push_back(make_tuple(di, ti, i+1)); // i+1 porque pide que las piezas se numeren desde 0
	}
}
void serializarSalida(vector<tuple<int, int, int> >& piezas, uint cantidadPiezas, uint totalPerdido){
	for (uint j = 0; j < cantidadPiezas; ++j)
	{
		cout << get<2>(piezas[j]) << " "; //imprime el orden de las piezas
	}

	// imprimir el total perdido
	cout << totalPerdido << endl; 
}

void solveInstance(vector<tuple<int, int, int> >& piezas, uint cantidadPiezas, uint& totalPerdido){
	sort(piezas.begin(), piezas.end(), compare_function); // ordenar segun ti/di	
	int b = cantidadPiezas - 1; // recorro el vector de adelante para atras para hacer la acumulacion de perdidas
	uint depreciaciones = 0; //suma de las depreciaciones a sumar
	/*
	Para un calculo de 3 piezas seria = t1*(d1+d2+d3) + t2*(d2+d3) +t3*(d3).   en depreciaciones voy acumulando los "d"
	*/
	while(b >= 0)
	{
		depreciaciones = depreciaciones + get<0>(piezas[b]);
		totalPerdido = totalPerdido + get<1>(piezas[b])*depreciaciones;
		b--;
	}
}
