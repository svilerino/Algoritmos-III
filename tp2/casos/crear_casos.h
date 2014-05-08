#ifndef __CREAR_CASOS_H__
#define __CREAR_CASOS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
 #include <sys/time.h>
#include "parsear_args.h"

#ifndef NULL
	#define NULL ((void *)0)
#endif

typedef int (*generar_caso)(FILE *f, char *var_1, char *var_2, char *var_3, char *instancias);

int generar_caso_tp_1_ej_1(FILE *f, char *var_1, char *var_2, char *var_3, char *instancias);
int generar_caso_tp_1_ej_2(FILE *f, char *var_1, char *var_2, char *var_3, char *instancias);
int generar_caso_tp_1_ej_3(FILE *f, char *var_1, char *var_2, char *var_3, char *instancias);
int generar_caso_tp_2_ej_2(FILE *f, char *var_1, char *var_2, char *var_3, char *instancias);

int random_maximo(int max);

void usage(char *s);

#endif

