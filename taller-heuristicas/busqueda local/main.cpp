#include <iostream>
#include "grafo.h"
#include "caminosimple.h"


int main() {


  int n, m, n1, n2;

  // Leo el grafo desde stdin
  std::cin >> n;
  Grafo g(n);
  std::cin >> m;

  while (m--) {
    std::cin >> n1 >> n2;
    g.agregarArista(n1, n2);
  }


  std::cout << g;

  // Genero solucion inicial
  /** COMPLETAR **/


  bool hayMejora = true;
  while (hayMejora) {

    // Busco en la vecindad si existe una mejor solucion
    /** COMPLETAR **/

    // Actualizo hayMejora
    /** COMPLETAR **/

  }

  // Por ultimo muestro la solucion encontrada
  /** COMPLETAR **/

  return 0;
}
