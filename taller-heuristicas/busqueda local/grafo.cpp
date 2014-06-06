#include <cassert>
#include <algorithm>
#include "grafo.h"

Grafo::Grafo() {
  _adyacencias = vector<Vecinos>();
  _n_aristas = 0;
}

Grafo::Grafo(int cantNodos) {
  _n_aristas = 0;
  for (int i = 0; i < cantNodos; i++) {
    this->agregarNodo();
  }
}

int Grafo::cantNodos() const {
  return _adyacencias.size();
}

int Grafo::cantAristas() const {
  return _n_aristas;
}

int Grafo::agregarNodo() {
  _adyacencias.push_back(Vecinos());
  return cantNodos() - 1;
}

void Grafo::agregarArista(int id1, int id2) {
  assert(cantNodos() > std::max(id1, id2));
  _adyacencias[id1].push_back(id2);
  _adyacencias[id2].push_back(id1);
  _n_aristas++;
}

const Grafo::Vecinos Grafo::vecinos(int nodo) const {
  return _adyacencias[nodo];
}

Grafo::Nodos Grafo::nodos() const {
  vector<Nodo> v(cantNodos());
  iota(v.begin(), v.end(), 0);
  return v;
}

ostream& operator<<(ostream& os, const Grafo& g) {
  if ( std::max(g.cantNodos(), g.cantAristas()) < 20) {
    os << "G: {" << std::endl;
    for (int i = 0; i < g.cantNodos(); i++) {
      os << "nodo: " << i << " => vecinos: ["; 
      const Grafo::Vecinos vs = g.vecinos(i);
      for (Grafo::IteradorVecinos it = vs.begin(); it != vs.end(); ++it) {
        os << " " << *it;
      }
      os << " ]" << std::endl;
    }
    os << "}" << std::endl;
  } else {
    os << "G: <#V(G): " << g.cantNodos() << ", #E(G): " << g.cantAristas() << ">" << std::endl;
  }
}