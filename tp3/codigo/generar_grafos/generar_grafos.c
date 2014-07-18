#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(int argc, char **argv)
{
	
	int cantidad_nodos;
	int cantidad_aristas;
	int maximo_random_w1;
	int maximo_random_w2;
	int K;
	int v;
	int w;
	int **aristas;
	int i, j, a, b;
	
	struct timeval tv;

	if(argc != 6){
		fprintf(stderr, "%s cantidad_nodos cantidad_aristas maximo_random_w1 maximo_random_w2 limite_w1\nK va a estar aleatoriamente entre 0 y cantidad_aristas * maximo_random_w1\n", argv[0]);
		return -1;
	}
	cantidad_nodos = atoi(argv[1]);
	cantidad_aristas = atoi(argv[2]);
	maximo_random_w1 = atoi(argv[3]);
	maximo_random_w2 = atoi(argv[4]);
	K = atoi(argv[5]);

	v = random() % cantidad_nodos;
	v++;
	w = random() % cantidad_nodos;
	w++;
	if(cantidad_aristas > (cantidad_nodos * (cantidad_nodos - 1)) / 2)
		cantidad_aristas = (cantidad_nodos * (cantidad_nodos - 1)) / 2;

	gettimeofday(&tv, NULL);

	srandom(tv.tv_usec);
	//K = random() % ((cantidad_aristas * maximo_random_w1) + 1);
	
	printf("%d %d %d %d %d\n", cantidad_nodos, cantidad_aristas, v, w, K);
	aristas = (int **)calloc(cantidad_nodos + 1, sizeof(int *));
	for(i = 0; i <= cantidad_nodos; i++){
		aristas[i] = (int *)calloc(cantidad_nodos + 1, sizeof(int));
	}
	while(cantidad_aristas > 0){
		i = random() % cantidad_nodos;
		i++;
		j = random() % cantidad_nodos;
		j++;
		if(i == j || aristas[i][j] || aristas[j][i]){
			a = (i + j + 1) % (cantidad_nodos * cantidad_nodos);
			b = 0;
			for(b = 0; a > 0; b = (b + 1) % (cantidad_nodos * cantidad_nodos)){
				i = b + 1;
				j = 1;
				if(i > cantidad_nodos){
					j = i - cantidad_nodos;
					i = cantidad_nodos;
				}
				while(i > 0 && j <= cantidad_nodos){
					if(i != j && !aristas[i][j] && !aristas[j][i]){
						a--;
						if(a <= 0)
							break;
					}
					i--;
					j++;
				}
			}
		}
		aristas[i][j] = 1;
		aristas[j][i] = 1;
		printf("%d %d %ld.0 %ld.0\n", i, j, random() % maximo_random_w1, random() % maximo_random_w2);
		cantidad_aristas--;
	}
	printf("0");
	return 0;
}

