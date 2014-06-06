#include <cassert>
#include <iostream>
#include "caminosimple.h"


CaminoSimple::CaminoSimple() {
 _nodos = set<int>();
 _camino = vector<int>();
 _tam = 0;
}

CaminoSimple::CaminoSimple(const CaminoSimple& cs) {
 _nodos = set<int>();
 _camino = vector<int>();
 _tam = 0;
 for (int i = cs.cantNodos()-1; i >= 0; i--) {
 	this->agregarNodo(cs.iesimo(i));
 }
}

bool CaminoSimple::agregarNodo(int id) {
 if (_nodos.insert(id).second) {
 	_camino.push_back(id);
 	_tam++;
 	return true;
 }
 return false;
}

int CaminoSimple::iesimo(int i) const {
 assert(i < _camino.size());
 return _camino[i];
}

bool CaminoSimple::incluido(int id) const {
 return _nodos.find(id) != _nodos.end();
}

void CaminoSimple::borrarNodo(int id) {
 if (incluido(id)) {
 	_nodos.erase(id);
 	vector<int>::iterator it = _camino.begin();
 	while (*it != id) {
     	++it;
 	}
     _camino.erase(it);
 	_tam--;
 }
}

void CaminoSimple::borrarUltimo() {
 if (_tam > 0) {
 	int ult = _camino.back();
 	_camino.pop_back();
 	_nodos.erase(ult);
 	_tam--;
 }
}

int CaminoSimple::cantNodos() const {
 return _tam;
}

ostream& CaminoSimple::mostrarCamino(ostream& os) const {
 for (int i = 0; i < _tam; i++) {
 	os << _camino[i];
 	if (i < _tam-1) {os << " -> ";}
 }
 return os << std::endl;
}

ostream& operator<<(ostream& os, const CaminoSimple& cs) {
    return cs.mostrarCamino(os);
}

bool operator==(const CaminoSimple& cs1, const CaminoSimple& cs2) {
 if (&cs1 == &cs2) return true;
 if (cs1.cantNodos() != cs2.cantNodos()) return false;
 for (int i = 0; i < cs1.cantNodos(); i++) {
 	if (cs1.iesimo(i) != cs2.iesimo(i)) return false;
 }
 return true;
}

bool operator!=(const CaminoSimple& cs1, const CaminoSimple& cs2) {
 return !(cs1 == cs2);
}
