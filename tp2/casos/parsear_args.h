#ifndef __PARSEAR_ARGS_H__
#define __PARSEAR_ARGS_H__

/*
 * en nombre poner el nombre del argumento, en seteado poner un puntero a una variable que se va a poner en 1 si el parametro fue puesto (o en nulo si no se quiere saber si fue seteado o no) y en valor poner un puntero a una variable donde poner el valor colocado al parametro
 */
struct argumento {
	char *nombre;
	int *seteado;
	char **valor;
};

/*
 * pasarle como argc y argv lo que recibe el main() sin el primer argumento, y en n la cantidad de argumentos del primer parametro (argumentos)
 * retorna 0 si fue ok o < 0 si algun argumento no fue encontrado, en tal caso se continuo parseando y completando los argumentos que encontro
 * el campo valor que le setea en la estructura, es un puntero dentro del argv, si el parametro no tiene '=', valor se pone en 0
 */
int parsear_args(struct argumento *argumentos, int n, int argc, char **argv);

#endif

