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

Grafo *crear_grafo(int nodos)
{
	int i;

	Grafo *g = NULL;
	if(nodos <= 0)
		return NULL;

	g = (Grafo *)malloc(sizeof(Grafo));
	g->nodos = nodos;
	g->componente_conexa_nodos = (int *)calloc(nodos, sizeof(int));
	g->componentes_conexas = nodos;
	g->aristas = 0;
	g->adyacencia = (int **)malloc(sizeof(int *) * nodos);

	for(i = 0; i < nodos; i++) {
		g->adyacencia[i] = (int *)calloc(nodos, sizeof(int));
	}
	for(i = 0; i < nodos; i++) {
		g->componente_conexa_nodos[i] = i;
	}
	return g;
}

void liberar_grafo(Grafo *g)
{
	int i;

	if(!g)
		return;

	free(g->componente_conexa_nodos);
	for(i = 0; i < g->nodos; i++) {
		free(g->adyacencia[i]);
	}
	free(g->adyacencia);
	free(g);
}

int agregar_arista(Grafo *g, Nodo nodo1, Nodo nodo2)
{
	int i;
	int nueva_componente, vieja_componente;

	if(!g)
		return -1;

	if(nodo1 >= g->nodos || nodo2 >= g->nodos)
		return -1;

	g->adyacencia[nodo1][nodo2] = 1;
	g->adyacencia[nodo2][nodo1] = 1;
	g->aristas++;

	if(g->componente_conexa_nodos[nodo1] != g->componente_conexa_nodos[nodo2]){
		if(g->componente_conexa_nodos[nodo1] <= g->componente_conexa_nodos[nodo2]){
			nueva_componente = g->componente_conexa_nodos[nodo1];
			vieja_componente = g->componente_conexa_nodos[nodo2];
		}
		else{
			nueva_componente = g->componente_conexa_nodos[nodo2];
			vieja_componente = g->componente_conexa_nodos[nodo1];
		}

		for(i = 0; i < g->nodos; i++){ //actualizo la componente conexa de todos los nodos que pertenecian a esa componente
			if(g->componente_conexa_nodos[i] == vieja_componente){
				g->componente_conexa_nodos[i] = nueva_componente;
			}
		}
		g->componentes_conexas--;
	}
	return 0;
}

int cantidad_componentes_conexas(Grafo *g)
{
	if(!g)
		return -1;
	return g->componentes_conexas;
}

int cantidad_aristas(Grafo *g)
{
	if(!g)
		return -1;
	return g->aristas;
}

int cantidad_nodos(Grafo *g)
{
	if(!g)
		return -1;
	return g->nodos;
}

int son_adyacentes(Grafo *g, Nodo nodo1, Nodo nodo2)
{
	if(!g)
		return 0;

	return g->adyacencia[nodo1][nodo2];
}

//retorna un vector con cantidad_componentes_conexas() elementos, en cada iesimo elemento, hay un nodo correspondiente a la componente iesima. liberar el resultado con free()
Nodo *nodos_de_componentes(Grafo *g)
{
	Nodo *nodos;
	int i;

	if(!g)
		return NULL;

	nodos = (Nodo *)malloc(sizeof(Nodo) * g->nodos);
	for(i = 0; i < g->nodos; i++){
		nodos[i] = -1;
	}
	for(i = 0; i < g->nodos; i++){
		nodos[g->componente_conexa_nodos[i]] = i;
	}
	return nodos;
}

int distancia(Ciudad *c1, Ciudad *c2)
{
	if(!c1 || !c2)
		return 0;

	return (c1->x - c2->x) * (c1->x - c2->x) + (c1->y - c2->y) * (c1->y - c2->y); //el cuadrado de la distancia euclidiana
}

void ordenar_aristas(Arista *aristas, int n)
{
	int m, i, j, k;
	Arista *aux;

	if(n <= 1)
		return;
	m = n / 2;
	ordenar_aristas(aristas, m);
	ordenar_aristas(aristas + m, n - m);
	aux = (Arista *)malloc(sizeof(Arista) * n);
	i = 0;
	j = 0;
	k = 0;
	while(i < m || j < (n - m)){
		if(j >= (n - m)){
			memcpy(&(aux[k]), &(aristas[i]), sizeof(Arista));
			i++;
			k++;
			continue;
		}
		if(i >= m){
			memcpy(&(aux[k]), &(aristas[m + j]), sizeof(Arista));
			j++;
			k++;
			continue;
		}
		if(aristas[i].distancia < aristas[m + j].distancia){
			memcpy(&(aux[k]), &(aristas[i]), sizeof(Arista));
			i++;
			k++;
			continue;
		}
		else{
			memcpy(&(aux[k]), &(aristas[m + j]), sizeof(Arista));
			j++;
			k++;
			continue;
		}
	}
	memcpy(aristas, aux, n * sizeof(Arista));
	free(aux);
}

Grafo *resolver(int k_centrales, Ciudad *ciudades, int n_ciudades, Nodo **centrales)
{
	Grafo *g = NULL;
	NodoDistancia *nodos = NULL;
	Arista *aristas = NULL;
	int i, agregados, distancia_minima = -1, nodo_minimo = -1, componentes;

	if(k_centrales <= 0 || ciudades == NULL || n_ciudades <= 0 || centrales == NULL){
		return NULL;
	}

	g = crear_grafo(n_ciudades);
	nodos = (NodoDistancia *)malloc(sizeof(NodoDistancia) * n_ciudades);
	aristas = (Arista *)malloc(sizeof(Arista) * (n_ciudades - 1));

	nodos[0].agregado = 1;
	nodos[0].distancia = 0;
	nodos[0].nodo = 0;
	for(i = 1; i < n_ciudades; i++){
		nodos[i].agregado = 0;
		nodos[i].distancia = distancia(&(ciudades[i]), &(ciudades[0]));
		nodos[i].nodo = 0;
	}

	for(agregados = 0; agregados < n_ciudades - 1; agregados++){
		distancia_minima = -1;
		nodo_minimo = 0;
		for(i = 0; i < n_ciudades; i++){
			if(!nodos[i].agregado){
				if(distancia_minima == -1 || nodos[i].distancia < distancia_minima){
					distancia_minima = nodos[i].distancia;
					nodo_minimo = i;
				}
			}
		}

		nodos[nodo_minimo].agregado = 1;
		aristas[agregados].nodo1 = nodo_minimo;
		aristas[agregados].nodo2 = nodos[nodo_minimo].nodo;
		aristas[agregados].distancia = distancia_minima;

		for(i = 0; i < n_ciudades; i++){
			if(!nodos[i].agregado){
				if(nodos[i].distancia > distancia(&(ciudades[i]), &(ciudades[nodo_minimo]))){
					nodos[i].distancia = distancia(&(ciudades[i]), &(ciudades[nodo_minimo]));
					nodos[i].nodo = nodo_minimo;
				}
			}
		}
	}

	ordenar_aristas(aristas, n_ciudades - 1);

	for(componentes = n_ciudades; componentes > k_centrales; componentes--){
		agregar_arista(g, aristas[n_ciudades - componentes].nodo1, aristas[n_ciudades - componentes].nodo2);
	}

	*centrales = nodos_de_componentes(g);

	free(nodos);
	free(aristas);

	return g;
}

