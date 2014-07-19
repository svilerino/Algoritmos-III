#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <random>

using namespace std;

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
	
	if(argc != 6){
		fprintf(stderr, "%s cantidad_nodos cantidad_aristas maximo_random_w1 maximo_random_w2 limite_w1\nK va a estar aleatoriamente entre 0 y cantidad_aristas * maximo_random_w1\n", argv[0]);
		return -1;
	}
	cantidad_nodos = atoi(argv[1]);
	cantidad_aristas = atoi(argv[2]);
	maximo_random_w1 = atoi(argv[3]);
	maximo_random_w2 = atoi(argv[4]);
	K = atoi(argv[5]);

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, cantidad_nodos-1);
	//dis(gen) genera el numerito

	v = dis(gen);
	v++;
	w = dis(gen);
	w++;
	if(cantidad_aristas > (cantidad_nodos * (cantidad_nodos - 1)) / 2)
		cantidad_aristas = (cantidad_nodos * (cantidad_nodos - 1)) / 2;

	printf("%d %d %d %d %d\n", cantidad_nodos, cantidad_aristas, v, w, K);
	aristas = (int **)calloc(cantidad_nodos + 1, sizeof(int *));
	for(i = 0; i <= cantidad_nodos; i++){
		aristas[i] = (int *)calloc(cantidad_nodos + 1, sizeof(int));
	}
	while(cantidad_aristas > 0){
		i = dis(gen);
		i++;
		j = dis(gen);
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

		//uniform_int_distribution<> disrw2(0, maximo_random_w2);
		//uniform_int_distribution<> disrw1(0, maximo_random_w1);
		uniform_int_distribution<> disrw2(0, maximo_random_w1);
		uniform_int_distribution<> disrw1(maximo_random_w1, maximo_random_w2);
		
		uniform_int_distribution<> dice(0, 1);
		if(dice(gen) == 0){
			printf("%d %d %ld.0 %ld.0\n", i, j, disrw1(gen), disrw2(gen));	
		}else{
			printf("%d %d %ld.0 %ld.0\n", i, j, disrw2(gen), disrw1(gen));			
		}

		cantidad_aristas--;
	}
	printf("0");
	return 0;
}

