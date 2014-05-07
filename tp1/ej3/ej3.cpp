#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../common/timing/timing.h"
#include "ej3.h"

using namespace std;

void vaciar_lista(Lista *lista)
{
	Lista *l, *l2;
	l = lista;
	while(l){
		l2 = l;
		l = l->siguiente;
		free(l2);
	}
}

Lista *agregar_a_lista_final(Lista *lista, void *nodo)
{
	Lista *l;
	
	l = lista;
	if(!l){
		l = (Lista *)calloc(1, sizeof(Lista));
		l->nodo = nodo;
		return l;
	}

	while(l->siguiente){
		l = l->siguiente;
	}
	l->siguiente = (Lista *)calloc(1, sizeof(Lista));
	l->siguiente->anterior = l;
	l->siguiente->nodo = nodo;
	return lista;
}

Lista *agregar_a_lista(Lista *l, Lista *nuevo)
{
	if(!l){
		return nuevo;
	}

	nuevo->anterior = l;
	nuevo->siguiente = l->siguiente;
	if(l->siguiente){
		l->siguiente->anterior = nuevo;
	}
	l->siguiente = nuevo;
	return nuevo;
}

Lista *agregar_a_lista_atras(Lista *l, Lista *nuevo)
{
	if(!l){
		return nuevo;
	}

	nuevo->anterior = l->anterior;
	nuevo->siguiente = l;
	if(l->anterior){
		l->anterior->siguiente = nuevo;
	}
	l->anterior = nuevo;
	return nuevo;
}

Lista *obtener_siguiente(Lista *l)
{
	if(!l)
		return NULL;

	return l->siguiente;
}

Lista *obtener_anterior(Lista *l)
{
	if(!l)
		return NULL;

	return l->anterior;
}

Lista *sacar_elemento_de_lista(Lista *l)
{
	if(!l)
		return NULL;
	if(l->anterior){
		l->anterior->siguiente = l->siguiente;
	}
	if(l->siguiente){
		l->siguiente->anterior = l->anterior;
	}
	return l;
}

void *obtener_nodo(Lista *l)
{
	if(!l)
		return NULL;
	return l->nodo;
}

void imprimir_tablero(Ficha ***t, int n, int m, Como_Imprimir como_imprimo)
{
	int i, j, a;
	if(como_imprimo == COMO_UN_TABLERO){
		for(i = 0; i < n; i++){
			for(a = 0; a < 3; a++){
				for(j = 0; j < m; j++){
					if(t[i][j]->numero < 0){
						printf("   |");
					}
					else{
						switch(a){
							case 0:
								printf(" %d |", t[i][j]->arriba);
								break;
							case 1:
								printf("%d %d|", t[i][j]->izquierda, t[i][j]->derecha);
								break;
							case 2:
								printf(" %d |", t[i][j]->abajo);
								break;
						}
					}
				}
				printf("\n");
			}
			printf("-----------------------------\n");
		}
	}
	else if(como_imprimo == COMO_PIDE_TP){
		for(i = 0; i < n; i++){
			for(j = 0; j < m; j++){
				printf("%d ", t[i][j]->numero < 0 ? 0 : t[i][j]->numero);
			}
			printf("\n");
		}
	}
}

void eliminar_tablero(Ficha ***tablero, int n, int m)
{
	int i;
	for(i = 0; i < n; i++){
		free(tablero[i]);
	}
	free(tablero);
}

Ficha ***crear_tablero(int n, int m)
{
	Ficha ***t;
	int i;

	t = (Ficha ***)malloc(sizeof(Ficha **) * n);
	for(i = 0; i < n; i++){
		t[i] = (Ficha **)calloc(1, sizeof(Ficha *) * m);
	}
	return t;
}

void copiar(Ficha ***out, Ficha ***in, int n, int m)
{
	int i, j;
	for(j = 0; j < m; j++){
		for(i = 0; i < n; i++){
			out[i][j] = in[i][j];
		}
	}
}

#define cargar_ficha(n,a,i,d,b) \
	f[n].numero = (n) + 1;\
	f[n].arriba = (a);\
	f[n].izquierda = (i);\
	f[n].derecha = (d);\
	f[n].abajo = (b)


void eliminar_fichas(Ficha *f){
	free(f);
}

Ficha *cargar_fichas(int n)
{
	Ficha *f;
	int i, a, z, d, b;

	f = (Ficha *)malloc(sizeof(Ficha) * (n + 1));
	for(i = 0; i < n; i++){
		if(scanf("%d %d %d %d", &a, &z, &d, &b) < 4){
			printf("Entrada erronea\n");
			return 0;
		}
		if(a <= 0 || z <= 0 || d <= 0 || b <= 0){
			printf("Entrada erronea\n");
			return 0;
		}
		cargar_ficha(i,a,z,d,b);
	}
	f[n].numero = -1;
	return f;
}

int valida_colocar(Ficha *ficha, Ficha ***tablero, int i, int j)
{
	if(ficha->numero >= 0){ //si es < 0 significa que se quiere dejar el casillero vacio, por lo que siempre es valido de colocar
		if(j > 0){ //si no estamos en la primer columna, entonces chequeo el color izquierdo de la ficha
			if(tablero[i][j - 1]->numero >= 0){
				if(tablero[i][j - 1]->derecha != ficha->izquierda){
					return 0;
				}
			}
		}
		if(i > 0){ //si no estamos en la primera fila, chequeo el color de arriba de la ficha
			if(tablero[i - 1][j]->numero >= 0){
				if(tablero[i - 1][j]->abajo != ficha->arriba){
					return 0;
				}
			}
		}
	}
	return 1;
}

void resolver(Ficha ***tablero_optimo, int *fichas_maxima, Ficha ***tablero, int n_tablero, int m_tablero, Ficha *fichas, Lista *fichas_disponibles, int i_t, int j_t, int casilleros_tablero, int fichas_en_tablero)
{
	int proximo_i, proximo_j;
	int total_casilleros;
	total_casilleros = n_tablero * m_tablero;
	Lista *lista, *lista_anterior = NULL, *lista_siguiente = NULL;
	Ficha *ficha;

	if(casilleros_tablero == total_casilleros){
		if(fichas_en_tablero > *fichas_maxima){ //solo si supera la cantidad maxima de fichas encontradas entonces reemplazo el tablero viejo con el nuevo
			*fichas_maxima = fichas_en_tablero;
			copiar(tablero_optimo, tablero, n_tablero, m_tablero); //O(n_tablero * m_tablero)
		}
		return;
	}
	if(total_casilleros - casilleros_tablero + fichas_en_tablero <= *fichas_maxima){ //la poda para cortar con la recursion si ya se calcula que no podra superar al tablero_optimo
		return;
	}
	proximo_i = i_t; //calcula cual es la siguiente posicion que llama a la recursion
	proximo_j = j_t;
	if(j_t == m_tablero - 1){
		proximo_j = 0;
		proximo_i++;
	}
	else{
		proximo_j++;
	}
	for(lista = fichas_disponibles; lista != NULL; lista = obtener_siguiente(lista)){ //(O(longitud de la lista)
		ficha = (Ficha *)obtener_nodo(lista); //O(1)
		if(valida_colocar(ficha, tablero, i_t, j_t)){ //O(1)
			tablero[i_t][j_t] = ficha;
			if(ficha->numero >= 0){ //si el numero es >= 0, es una ficha con colores, por lo que la tengo que quitar de la listaa de fichas disponibles al llamar a la recursion
				lista_siguiente = obtener_siguiente(lista); //O(1)
				lista_anterior = obtener_anterior(lista); //O(1)
				sacar_elemento_de_lista(lista); //O(1)
				fichas_en_tablero++;
			}
			else //si el numero es < 0, significa que se llego a la ultima `ficha', que representa dejar el casillero vacio, la cual siempre esta disponible y no hay que quitarla de disponibilidad
				lista_anterior = fichas_disponibles; //le paso la lista entera de fichas, desde el comienzo
			resolver(tablero_optimo, fichas_maxima, tablero, n_tablero, m_tablero, fichas, !lista_anterior ? lista_siguiente : fichas_disponibles, proximo_i, proximo_j, casilleros_tablero + 1, fichas_en_tablero);
			if(ficha->numero >= 0){ //si se saco la ficha de la lista de disponible, entonces la tengo que volver a agregar en el mismo lugar donde estaba
				if(lista_anterior){ //si el elemento que saque tenia un anterior, se lo agrego despues del anterior
					lista = agregar_a_lista(lista_anterior, lista); //O(1)
				}
				else if(lista_siguiente){ //sino (si saque el primer elemento), lo agrego atras del siguiente elemento del que saque
					lista = agregar_a_lista_atras(lista_siguiente, lista); //O(1)
				}
				fichas_en_tablero--;
			}
		}
	}
}

void resolver_limpiando_variables(Ficha ***tablero_optimo, int *fichas_maxima, Ficha ***tablero, int n_tablero, int m_tablero, Ficha *fichas, Lista *fichas_disponibles, int i_t, int j_t, int casilleros_tablero, int fichas_en_tablero)
{
	int j, k, n_fichas;

	n_fichas = n_tablero * m_tablero + 1;
	fichas_disponibles = 0;
	for(j = 0; j < n_fichas; j++){
		fichas_disponibles = agregar_a_lista_final(fichas_disponibles, &fichas[j]);
	}
	for(j = 0; j < n_tablero; j++){
		for(k = 0; k < m_tablero; k++){
			tablero_optimo[j][k] = 0;
		}
	}
	*fichas_maxima = -1;
	resolver(tablero_optimo, fichas_maxima, tablero, n_tablero, m_tablero, fichas, fichas_disponibles, i_t, j_t, casilleros_tablero, fichas_en_tablero);
	vaciar_lista(fichas_disponibles);
}

int main(int argc, char **argv)
{
	Ficha ***tablero;
	Ficha ***tablero_optimo;
	Ficha *fichas;
	Lista *fichas_disponibles = NULL;
	int fichas_maxima = -1;
	int n_tablero = 3, m_tablero = 3, n_fichas, n_colores = 4, i;
	int calcular_tiempo = 0;
	
	if(argc > 1)
		calcular_tiempo = 1;

	if(scanf("%d %d %d", &n_tablero, &m_tablero, &n_colores) < 3){
		printf("Entrada erronea\n");
		return -1;
	}
	if(n_tablero <= 0 || m_tablero <= 0 || n_colores <= 0){
		printf("Entrada erronea\n");
		return -1;
	}
	n_fichas = (n_tablero * m_tablero) + 1;
	tablero = crear_tablero(n_tablero, m_tablero);
	assert(tablero);
	tablero_optimo = crear_tablero(n_tablero, m_tablero);
	assert(tablero_optimo);
	fichas = cargar_fichas(n_tablero * m_tablero);
	assert(fichas);

	if(calcular_tiempo){
		double promedio = 0.0;
		MEDIR_TIEMPO_PROMEDIO(resolver_limpiando_variables(tablero_optimo, &fichas_maxima, tablero, n_tablero, m_tablero, fichas, fichas_disponibles, 0, 0, 0, 0);, REPETICIONES_CALCULAR_TIEMPO, &promedio);
		imprimir_tablero(tablero_optimo, n_tablero, m_tablero, COMO_UN_TABLERO);
		cerr << n_tablero << " " << m_tablero << " " << n_colores << " " << promedio << " " << REPETICIONES_CALCULAR_TIEMPO << endl;
	}
	else{
		for(i = 0; i < n_fichas; i++){
			fichas_disponibles = agregar_a_lista_final(fichas_disponibles, &fichas[i]);
		}
		resolver(tablero_optimo, &fichas_maxima, tablero, n_tablero, m_tablero, fichas, fichas_disponibles, 0, 0, 0, 0);
		imprimir_tablero(tablero_optimo, n_tablero, m_tablero, COMO_PIDE_TP);
		vaciar_lista(fichas_disponibles);
	}
	eliminar_tablero(tablero, n_tablero, m_tablero);
	eliminar_tablero(tablero_optimo, n_tablero, m_tablero);
	eliminar_fichas(fichas);

	return 0;
}

