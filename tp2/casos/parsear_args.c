#include <string.h>
#include <stdio.h>
#include "parsear_args.h"

int parsear_args(struct argumento *argumentos, int n, int argc, char **argv)
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
				if(argumentos[j].seteado)
					*argumentos[j].seteado = 1;
				if(argumentos[j].valor)
					*argumentos[j].valor = valor;
				break;
			}
		}
		if(j >= n){
			fprintf(stderr, "No se reconoce %s\n", nombre);
			r = -1;
		}
	}
	return r;
}

