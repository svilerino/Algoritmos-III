#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void merge_sort(int *a, int n)
{
	int m, i, j, k;
	int *aux;

	if(n <= 1)
		return;
	m = n / 2;
	merge_sort(a, m);
	merge_sort(a + m, n - m);
	aux = malloc(sizeof(int) * n);
	i = 0;
	j = 0;
	k = 0;
	while(i < m || j < (n - m)){
		if(j >= (n - m)){
			aux[k]= a[i];
			i++;
			k++;
			continue;
		}
		if(i >= m){
			aux[k]= a[m + j];
			j++;
			k++;
			continue;
		}
		if(a[i] < a[m + j]){
			aux[k]= a[i];
			i++;
			k++;
			continue;
		}
		else{
			aux[k]= a[m + j];
			j++;
			k++;
			continue;
		}
	}
	memcpy(a, aux, n * sizeof(int));
	free(aux);
}

int main(int argc, char **argv)
{
	int D, n, *dn, i, j, max_c = -1, max_i = -1;//, max_j = -1;

	if(scanf("%d %d", &D, &n) < 2)
		return -1;
	if(n <= 0)
		return -1;
	dn = malloc(sizeof(int) * n);
	for(i = 0; i < n; i++){
		if(scanf(" %d", dn + i) < 1)
			return -1;
	}
	merge_sort(dn, n);
	for(i = 0; i < n; i++){
		for(j = i; j < n && (dn[j] - dn[i]) < D; j++);
		if(j - i > max_c){
			max_c = j - i;
			max_i = i;
			//max_j = j - 1;
		}
	}
	//printf("maximo: cantidad %d, inicio %d, fin %d\n", max_c, max_i, max_j);
	printf("%d %d", dn[max_i], max_c);
	/*for(i = max_i; i <= max_j; i++){
		printf("%d ", dn[i]);
	}*/
	printf("\n");
	free(dn);
	return 0;
}

