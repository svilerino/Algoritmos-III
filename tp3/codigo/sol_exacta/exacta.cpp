#include <stdio.h>
#include <iostream>
#include "../common/grafo.h"

using namespace std;

typedef struct peso{
	int w1;
	int w2;
} peso;

int main(int argc, char **argv)
{
	peso p1;
	GrafoAdyacencia<peso> grafo(10);

	p1.w1 = 42;
	p1.w2 = 30;
	grafo.AgregarArista(2,3, &p1);

	cout << grafo.EsAdyacente(2,3) << " " << grafo.EsAdyacente(9,8);
	return 0;
}

