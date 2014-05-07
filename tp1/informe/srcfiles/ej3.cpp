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

void copiar(Ficha ***out, Ficha ***in, int n, int m)
{
	int i, j;
	for(j = 0; j < m; j++){
		for(i = 0; i < n; i++){
			out[i][j] = in[i][j];
		}
	}
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

