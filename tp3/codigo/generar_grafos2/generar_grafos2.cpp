#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

struct argumento {
	const char *nombre;
	int *seteado;
	char **valor;
	int *entero;
};

int conexo = 0, k_n = 0, min_w1 = -1, max_w1 = -1, min_w2 = -1, max_w2 = -1, nodos = -1, vertices = -1;

int parsear_args(argumento *argumentos, int n, int argc, char **argv)
{
	int i, j, r = 0;
	char *c, *nombre, *valor;

	if(!argumentos || !argc)
		return -1;

	for(i = 0; i < argc; i++){
		valor = 0;
		nombre = argv[i];
		c = strchr(argv[i], '=');
		if(c){
			valor = c + 1;
			*c = 0;
		}
		for(j = 0; j < n; j++){
			if(argumentos[j].nombre && !strcmp(argumentos[j].nombre, nombre)){
				if(argumentos[j].seteado != NULL)
					*argumentos[j].seteado = 1;
				if(argumentos[j].valor != NULL)
					*argumentos[j].valor = valor;
				if(argumentos[j].entero != NULL)
					*argumentos[j].entero = atoi(valor);
				break;
			}
		}
		if(j >= n){
			cerr << "No se reconoce " << nombre << endl;
			r = -1;
		}
	}
	return r;
}

argumento argumentos[8] =
{
	{
		.nombre = "--conexo", //fuerza a que sea conexo
		.seteado = &conexo,
		.valor = NULL,
		.entero = NULL
	},
	{
		.nombre = "--K", //fuerza un K
		.seteado = NULL,
		.valor = NULL,
		.entero = &k_n
	},
	{
		.nombre = "--min-w1",
		.seteado = NULL,
		.valor = NULL,
		.entero = &min_w1
	},
	{
		.nombre = "--max-w1",
		.seteado = NULL,
		.valor = NULL,
		.entero = &max_w1
	},
	{
		.nombre = "--min-w2",
		.seteado = NULL,
		.valor = NULL,
		.entero = &min_w2
	},
	{
		.nombre = "--max-w2",
		.seteado = NULL,
		.valor = NULL,
		.entero = &max_w2
	},
	{
		.nombre = "--vertices",
		.seteado = NULL,
		.valor = NULL,
		.entero = &vertices
	},
	{
		.nombre = "--nodos",
		.seteado = NULL,
		.valor = NULL,
		.entero = &nodos
	}
};

int main(int argc, char **argv)
{
	if(parsear_args(argumentos, 8, argc - 1, argv + 1)){
		return -1;
	}

	if(conexo){
		if(nodos <= 0)
			return -1;
		if(vertices < nodos - 1)
			vertices = nodos - 1;
	}
	if(k_n > 0){
		nodos = k_n;
		vertices = (nodos * (nodos - 1)) / 2;
	}
	else{
		if(nodos <= 0 || vertices < 0)
			return -1;
		if(vertices > (nodos * (nodos - 1)) / 2)
			vertices = (nodos * (nodos - 1)) / 2;
	}

	return 0;
}

