#include "crear_casos.h"

char *ejercicio = NULL, *salida = NULL, *variable_1 = NULL, *variable_2 = NULL, *variable_3 = NULL, *instancias = NULL, *maximo_random_v1 = NULL, *maximo_random_v2 = NULL, *maximo_random_v3 = NULL, *maximo_random = NULL, *tp = NULL;
int maximo_random_v1_i = 0, maximo_random_v2_i = 0, maximo_random_v3_i = 0, maximo_random_i;

#define cantidad_argumentos (11)
struct argumento mis_argumentos[cantidad_argumentos] =
{
	{
		.nombre = "--tp",
		.seteado = NULL,
		.valor = &tp
	},
	{
		.nombre = "--ejercicio",
		.seteado = NULL,
		.valor = &ejercicio
	},
	{
		.nombre = "--salida",
		.seteado = NULL,
		.valor = &salida
	},
	{
		.nombre = "--variable-1",
		.seteado = NULL,
		.valor = &variable_1
	},
	{
		.nombre = "--variable-2",
		.seteado = NULL,
		.valor = &variable_2
	},
	{
		.nombre = "--variable-3",
		.seteado = NULL,
		.valor = &variable_3
	},
	{
		.nombre = "--instancias",
		.seteado = NULL,
		.valor = &instancias
	},
	{
		.nombre = "--maximo-random",
		.seteado = NULL,
		.valor = &maximo_random
	},
	{
		.nombre = "--maximo-random-v1",
		.seteado = NULL,
		.valor = &maximo_random_v1
	},
	{
		.nombre = "--maximo-random-v2",
		.seteado = NULL,
		.valor = &maximo_random_v2
	},
	{
		.nombre = "--maximo-random-v3",
		.seteado = NULL,
		.valor = &maximo_random_v3
	}
};

#define cantidad_ejercicios (3)
#define tps (3)
generar_caso funciones_casos[tps][cantidad_ejercicios] = {
{generar_caso_tp_1_ej_1, generar_caso_tp_1_ej_2, generar_caso_tp_1_ej_3},
{NULL, generar_caso_tp_2_ej_2, NULL},
{NULL, NULL, NULL}
};

int random_maximo(int max)
{
	if(max > 0)
		return random() % max + 1;//fix +1 para que no de 0!
	return random();
}

void usage(char *s)
{
	if(!s)
		s = "crear_casos";
	printf("\nuso: %s --tp=numero_tp --ejercicio=numero_ejercicio --salida=archivo_salida --variable-1=variable_1 [--variable-2=variable_2 [--variable-3=variable_3]] [--instancias=cantidad_de_instancias] [--maximo-random-v1=maximo_random_v1 [--maximo-random-v2=maximo_random_v2 [--maximo-random-v3=maximo_random_v3]]] [--maximo-random=maximo_random]\n\n"
			"\t--ejercicio=numero_ejercicio : el numero del ejercicio del cual crear el caso\n"
			"\t--salida=archivo_salida : el archivo donde escribir la salida\n"
			"\t--variable-{1,2,3}=variable_{1,2,3} : el valor de la variable para el ejercicio. por ejemplo si el ejercicio solo tiene de variable n, solo uasr --variable-1, si tiene n y m, usar --variable-1 y --variable-2. se puede poner \'random\' para que elija la variable aleatoriamente\n"
			"\t--instancias=cantidad_de_instancias : para los ejercicios que lo soporten, va a ser la cantidad de instancias en el caso. se puede opner \'random\' para que elija aleatoriamente, si no se especifica, se pone 1 instancia\n"
			"\t--maximo-random-v{1,2,3}=maximo_random_v{1,2,3} : el valor maximo que puedan tomar los valores de las variables 1, 2 y 3 que se toman como aleatorio, si no se especifica no tiene limite\n"
			"\t--maximo-random=maximo_random : el valor maximo de las otras variables que no se pasan por parametro\n"
			//"\n\tpara el ejercicio 1: se toman 2 variables, la primera corresponde a la D y la segunda a la n\n"
			"\n\tpara el tp2, ejercicio 2: se toma 2 variables, que corresponde a la cantidad de ciudades y de centrales (n, k), con --maximo-random se controla el valor de la posicion de las centrales\n"
			//"\n\tpara el ejercicio 3: se toman 3 variables, que corresponde a n, m y la cantidad de colores\n"
			"\n", s);
}

int main(int argc, char **argv)
{
	int ej, n_tp;
	FILE *f;
	struct timeval tv;

	if(parsear_args(mis_argumentos, cantidad_argumentos, argc - 1, argv + 1) < 0){
		usage(argv[0]);
		return -1;
	}

	if(!tp || !ejercicio || !salida || !variable_1){
		usage(argv[0]);
		return -1;
	}
	
	n_tp = atoi(tp);
	ej = atoi(ejercicio);
	if(ej <= 0 || tp <= 0 || ej > cantidad_ejercicios || n_tp > tps){
		fprintf(stderr, "No existe el ejercicio %d\n", ej);
		return -1;
	}

	if(maximo_random_v1){
		maximo_random_v1_i = atoi(maximo_random_v1);
		if(maximo_random_v1_i <= 0){
			fprintf(stderr, "maximo_random_v1 (%s) tiene que ser > 0\n", maximo_random_v1);
			return -1;
		}
	}
	if(maximo_random_v2){
		maximo_random_v2_i = atoi(maximo_random_v2);
		if(maximo_random_v2_i <= 0){
			fprintf(stderr, "maximo_random_v2 (%s) tiene que ser > 0\n", maximo_random_v2);
			return -1;
		}
	}
	if(maximo_random_v3){
		maximo_random_v3_i = atoi(maximo_random_v3);
		if(maximo_random_v3_i <= 0){
			fprintf(stderr, "maximo_random_v3 (%s) tiene que ser > 0\n", maximo_random_v3);
			return -1;
		}
	}
	if(maximo_random){
		maximo_random_i = atoi(maximo_random);
		if(maximo_random_i <= 0){
			fprintf(stderr, "maximo_random (%s) tiene que ser > 0\n", maximo_random);
			return -1;
		}
	}

	f = fopen(salida, "w");
	if(!f){
		fprintf(stderr, "No se pudo abrir el archivo %s para escribirlo\n", salida);
		return -1;
	}
	gettimeofday(&tv, 0);
	srandom(tv.tv_usec + tv.tv_sec);
	if(!funciones_casos[n_tp - 1][ej - 1]){
		fprintf(stderr, "Aun no esta implementado el tp %d ejercicio %d\n", n_tp, ej);
		return -1;
	}
	if(funciones_casos[n_tp - 1][ej - 1](f, variable_1, variable_2, variable_3, instancias) < 0){
		fprintf(stderr, "Error al crear el caso del tp %d ejercicio %s\n", n_tp, ejercicio);
		fclose(f);
		return -1;
	}
	fclose(f);
	return 0;
}

int generar_caso_tp_1_ej_1(FILE *f, char *var_1, char *var_2, char *var_3, char *instancias)
{
	int cantidad = 1;
	int n = 0, D = 0, dn = 0, i;

	if(!f || !var_1 || !var_2)
		return -1;

	if(instancias){
		if(!strcmp(instancias, "random")){
			cantidad = random_maximo(maximo_random_v1_i);
		}
		else{
			cantidad = atoi(instancias);
		}
	}

	if(strcmp(var_1, "random")){
		D = atoi(var_1);
		if(D <= 0){
			fprintf(stderr, "Mala variable-1\n");
			return -1;
		}
	}
	if(strcmp(var_2, "random")){
		n = atoi(var_2);
		if(n <= 0){
			fprintf(stderr, "Mala variable-2\n");
			return -1;
		}
	}

	while(cantidad > 0){
		if(!strcmp(var_1, "random")){
			D = random_maximo(maximo_random_v1_i) + 1;
		}
		if(!strcmp(var_2, "random")){
			n = random_maximo(maximo_random_v2_i) + 1;
		}
		fprintf(f, "%d %d", D, n);
		for(i = 0; i < n; i++){
			dn = random_maximo(maximo_random_i);
			fprintf(f, " %d", dn);
		}
		fprintf(f, "\n");
		cantidad--;
	}
	fprintf(f, "0");
	return 0;
}

int generar_caso_tp_1_ej_2(FILE *f, char *var_1, char *var_2, char *var_3, char *instancias)
{
	int cantidad = 1, n = 0, voy = 0, d, t;

	if(!f || !var_1)
		return -1;

	if(instancias){
		if(!strcmp(instancias, "random")){
			cantidad = random_maximo(maximo_random_v1_i) + 1;
		}
		else{
			cantidad = atoi(instancias);
		}
	}
	if(strcmp(var_1, "random")){
		n = atoi(var_1);
		if(n <= 0){
			fprintf(stderr, "Mala variable-1\n");
			return -1;
		}
	}
	while(cantidad > 0){
		if(!strcmp(var_1, "random")){
			n = random_maximo(maximo_random_v1_i) + 1;
		}
		fprintf(f, "%d\n", n);
		for(voy = 0; voy < n; voy++){
			d = random_maximo(maximo_random_i);
			t = random_maximo(maximo_random_i);
			fprintf(f, "%d %d\n", d, t);
		}
		cantidad--;
	}
	fprintf(f, "0");
	return 0;
}

int generar_caso_tp_1_ej_3(FILE *f, char *var_1, char *var_2, char *var_3, char *instancias)
{
	int n, m, c, i;

	if(!f || !var_1 || !var_2 || !var_3)
		return -1;

	if(strcmp(var_1, "random")){
		n = atoi(var_1);
		if(n <= 0){
			fprintf(stderr, "Mala variable-1\n");
			return -1;
		}
	}
	else{
		n = random_maximo(maximo_random_v1_i) + 1;
	}
	if(strcmp(var_2, "random")){
		m = atoi(var_2);
		if(m <= 0){
			fprintf(stderr, "Mala variable-2\n");
			return -1;
		}
	}
	else{
		m = random_maximo(maximo_random_v2_i) + 1;
	}
	if(strcmp(var_3, "random")){
		c = atoi(var_3);
		if(c <= 0){
			fprintf(stderr, "Mala variable-3\n");
			return -1;
		}
	}
	else{
		c = random_maximo(maximo_random_v3_i) + 1;
	}
	fprintf(f, "%d %d %d\n", n, m, c);
	for(i = 0; i < n * m; i++){
		fprintf(f, "%d %d %d %d\n", random_maximo(c) + 1, random_maximo(c) + 1, random_maximo(c) + 1, random_maximo(c) + 1);
	}
	return 0;
}

int generar_caso_tp_2_ej_2(FILE *f, char *var_1, char *var_2, char *var_3, char *instancias)
{
	int voy = 0, ciudades = 0, centrales = 0, x, y;

	if(!f || !var_1)
		return -1;

	if(strcmp(var_1, "random")){
		ciudades = atoi(var_1);
		if(ciudades <= 0){
			fprintf(stderr, "Mala variable-1\n");
			return -1;
		}
	}
	else{
		ciudades = random_maximo(maximo_random_v1_i) + 1;
	}
	if(strcmp(var_2, "random")){
		centrales = atoi(var_2);
		if(centrales <= 0){
			fprintf(stderr, "Mala variable-1\n");
			return -1;
		}
	}
	else{
		centrales = random_maximo(maximo_random_v2_i) + 1;
	}
	fprintf(f, "%d %d", ciudades, centrales);
	for(voy = 0; voy < ciudades; voy++){
		x = random_maximo(maximo_random_i);
		y = random_maximo(maximo_random_i);
		fprintf(f, "\n%d %d", x, y);
	}
	return 0;
}

