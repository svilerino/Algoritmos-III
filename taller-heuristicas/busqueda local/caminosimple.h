#ifndef CAMINO_SIMPLE_H
#define CAMINO_SIMPLE_H

#include <set>
#include <vector>
#include <iostream>

using std::set;
using std::vector;
using std::ostream;

class CaminoSimple {

public:

 CaminoSimple();
 CaminoSimple(const CaminoSimple& cs);
 bool agregarNodo(int id);
 int iesimo(int i) const;
 bool incluido(int id) const;
 void borrarNodo(int id);
 void borrarUltimo();
 int cantNodos() const;
 ostream& mostrarCamino(ostream& os) const;


private:
 set<int> _nodos;
 vector<int> _camino;
 int _tam;
};

ostream& operator<<(ostream& os, const CaminoSimple& cs);
bool operator==(const CaminoSimple& cs1, const CaminoSimple& cs2);
bool operator!=(const CaminoSimple& cs1, const CaminoSimple& cs2);


#endif