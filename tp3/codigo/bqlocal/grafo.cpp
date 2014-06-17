#include "grafo.h"
// -------------- Arista ---------------------------------

Arista::Arista(){
	this->presente = false;
	this->costo_w1 = 0;
	this->costo_w2 = 0;
}

Arista::Arista(bool pres, costo_t w1, costo_t w2){
	this->presente = pres;
	this->costo_w1 = w1;
	this->costo_w2 = w2;
}

Arista::~Arista(){

}

bool Arista::esta_presente(){
	return this->presente;
}

costo_t Arista::obtener_costo_w1(){
	return this->costo_w1;	
}

costo_t Arista::obtener_costo_w2(){
	return this->costo_w2;	
}

void Arista::marcar_presente(costo_t w1, costo_t w2){
	this->presente = true;
	this->costo_w1 = w1;
	this->costo_w2 = w2;
}

void Arista::desmarcar_presente(){
	this->presente = false;
	this->costo_w1 = 0;
	this->costo_w2 = 0;
}

// -------------- Camino ---------------------------------

Camino::Camino(matriz_adyacencia_t& mat_adyacencia){
	this->mat_adyacencia = mat_adyacencia;
	this->costo_camino_w1 = 0;
	this->costo_camino_w2 = 0;
}

Camino::~Camino(){

}

void Camino::agregar_nodo(nodo_t target){
	costo_t costo_agregado_w1 = 0;
	costo_t costo_agregado_w2 = 0;
	if(this->camino.size()>0){
		nodo_t penultimo_nodo = camino.back();
		costo_agregado_w1 = obtener_costo_w1_entre_nodos(penultimo_nodo, target);
		costo_agregado_w2 = obtener_costo_w2_entre_nodos(penultimo_nodo, target);
		this->costo_camino_w1 += costo_agregado_w1;	
		this->costo_camino_w2 += costo_agregado_w2;	
	}
	this->camino.push_back(target);
}

void Camino::agregar_nodo_adelante(nodo_t target){
	costo_t costo_agregado_w1 = 0;
	costo_t costo_agregado_w2 = 0;
	if(this->camino.size()>0){
		nodo_t penultimo_nodo = camino.back();
		costo_agregado_w1 = obtener_costo_w1_entre_nodos(penultimo_nodo, target);
		costo_agregado_w2 = obtener_costo_w2_entre_nodos(penultimo_nodo, target);
		this->costo_camino_w1 += costo_agregado_w1;	
		this->costo_camino_w2 += costo_agregado_w2;	
	}
	this->camino.push_front(target);
}

costo_t Camino::obtener_costo_total_w1_camino(){
	return this->costo_camino_w1;
}

costo_t Camino::obtener_costo_total_w2_camino(){
	return this->costo_camino_w2;
}

//pre: 0 <= i <= j < cantidad_nodos y que i,j sean adyacentes
costo_t Camino::obtener_costo_w1_entre_nodos(nodo_t i, nodo_t j){
	if(this->mat_adyacencia[i][j].esta_presente()){
		return this->mat_adyacencia[i][j].obtener_costo_w1();
	}else{
		cerr << "Costo infinito(no son adyacentes) entre nodos (" << i << " y (" << j << ")" << endl;
		return costo_infinito;
	}	
}

//pre: 0 <= i <= j < cantidad_nodos y que i,j sean adyacentes
costo_t Camino::obtener_costo_w2_entre_nodos(nodo_t i, nodo_t j){
	if(this->mat_adyacencia[i][j].esta_presente()){
		return this->mat_adyacencia[i][j].obtener_costo_w2();
	}else{
		cerr << "Costo infinito(no son adyacentes) entre nodos (" << i << " y (" << j << ")" << endl;
		return costo_infinito;
	}	
}

void Camino::imprimir_camino(ostream& out){
	if(this->camino.size() < 2){
		cerr << "[Imprimir camino] Error - Camino Invalido (size < 2)" << endl;
		return;
	}

	list<nodo_t>::iterator it = this->camino.begin();
	list<nodo_t>::iterator runner_it = this->camino.begin();
	runner_it++;	
	while(runner_it != this->camino.end()){
		out << "(" << *it << ")" << "--[" << this->obtener_costo_w1_entre_nodos(*it, *runner_it) << ", " << this->obtener_costo_w2_entre_nodos(*it, *runner_it) << "]-->";
		++it;
		++runner_it;
	}	
	out << "(" << this->camino.back() << ")" << endl;
}

list<nodo_t>::const_iterator Camino::obtener_iterador_begin(){
	return this->camino.begin();
}

list<nodo_t>::const_iterator Camino::obtener_iterador_end(){
	return this->camino.end();
}

// -------------- Vecino ---------------------------------

Vecino::Vecino(nodo_t i, nodo_t j, nodo_t comun, Arista desde_i, Arista desde_j){
	this->i = i;
	this->j = j;
	this->en_comun = comun;
	this->desde_i_a_comun = desde_i;
	this->desde_j_a_comun = desde_j;
}

Vecino::~Vecino(){

}

nodo_t Vecino::obtener_nodo_i(){
	return this->i;
}

nodo_t Vecino::obtener_nodo_j(){
	return this->j;
}

nodo_t Vecino::obtener_nodo_comun(){
	return this->en_comun;
}

Arista Vecino::obtener_arista_i_comun(){
	return this->desde_i_a_comun;
}

Arista Vecino::obtener_arista_j_comun(){
	return this->desde_j_a_comun;
}

// -------------- Grafo ---------------------------------

Grafo::Grafo(int cant_inicial_nodos){
	this->cantidad_nodos=cant_inicial_nodos;
	this->cantidad_aristas = 0;
	nodo_src = 0;
	nodo_dst = 0;
	cota_w1 = 0;
	costo_obtenido_w1 = 0;
	costo_obtenido_w2 = 0;
	
	//camino_obtenido se inicializa por defecto;
	
	//inicializo matriz de adyacencia
	vector<Arista> vec_fila(cantidad_nodos, Arista(false, 0, 0));
	this->mat_adyacencia.resize(cantidad_nodos, vec_fila);
}

Grafo::~Grafo(){

}

//modificadores
void Grafo::agregar_nodos(int cantidad_nodos_nuevos){
	//agrego cantidad_nodos_nuevos columnas en todas las filas existentes
	for(int i=0;i < this->cantidad_nodos;i++){
		this->mat_adyacencia[i].resize(this->cantidad_nodos + cantidad_nodos_nuevos, Arista(false, 0, 0));
	}
	
	//agrego cantidad_nodos_nuevos filas al final de la matriz de adyacencia
	vector<Arista> vec_fila(this->cantidad_nodos + cantidad_nodos_nuevos, Arista(false, 0, 0));
	this->mat_adyacencia.resize(this->cantidad_nodos + cantidad_nodos_nuevos, vec_fila);
	
	//actualizo cantidad_nodos total del grafo
	this->cantidad_nodos+=cantidad_nodos_nuevos;
}

void Grafo::agregar_arista(nodo_t i, nodo_t j, costo_t w1, costo_t w2){
	Arista arista = obtener_arista(i, j);
	//marco doble, no es un digrafo
	this->mat_adyacencia[i][j].marcar_presente(w1, w2);
	this->mat_adyacencia[j][i].marcar_presente(w1, w2);
	if(!arista.esta_presente()){
		this->cantidad_aristas++;
	}
}

void Grafo::quitar_arista(nodo_t i, nodo_t j){
	Arista arista = obtener_arista(i, j);
	//desmarco doble, no es un digrafo
	this->mat_adyacencia[i][j].desmarcar_presente();
	this->mat_adyacencia[j][i].desmarcar_presente();
	if(arista.esta_presente()){
		this->cantidad_aristas--;
	}	
}

//consultas
Arista Grafo::obtener_arista(nodo_t i, nodo_t j){
	return this->mat_adyacencia[i][j];
}

void Grafo::imprimir_matriz_adyacencia(ostream& out){	
	out << "Cantidad nodos: " << this->cantidad_nodos << endl;
	out << "Cantidad aristas: " << this->cantidad_aristas << endl;
	out << "Matriz adyacencia:" << endl;
	for (int i = 0;i<this->cantidad_nodos;i++){
		for (int j = 0;j<this->cantidad_nodos;j++){
			out << "|_(" << this->mat_adyacencia[i][j].esta_presente() << ")_|";
			//if(this->mat_adyacencia[i][j].esta_presente()){
			//	out << "|_(" << this->mat_adyacencia[i][j].obtener_costo_w1() << ", " << this->mat_adyacencia[i][j].obtener_costo_w2() << ")_|";
			//}else{
			//	out << "|_(-1, -1)_|";
			//}
		}
		out << endl;
	}
	out << endl;
	out << endl;
}

void Grafo::serialize(ostream& out){
	out << this->costo_obtenido_w1 << " ";
	out << this->costo_obtenido_w2 << " ";
	out << this->camino_obtenido.size() << " ";
	list<nodo_t>::iterator it = this->camino_obtenido.begin();
	while(it != camino_obtenido.end()){
		out << *it << " ";
		++it;
	}
}

void Grafo::unserialize(istream& in){
	//primera linea:
	//n m u v K
	//cant nodos, cant aristas, src, dst, K(cota w1)
	int cant_nodos_nuevos = 0, cantidad_aristas_nuevas = 0;
	in >> cant_nodos_nuevos;
	in >> cantidad_aristas_nuevas;
	in >> this->nodo_src;
	in >> this->nodo_dst;
	in >> this->cota_w1;

	this->agregar_nodos(cant_nodos_nuevos);

	int count = 0;
	while(count < cantidad_aristas_nuevas){
		//las lineas de las aristas vienen asi:
		// v1 v2 w1 w2
		nodo_t nodo_a = 0, nodo_b = 0; costo_t costo_w1 = 0, costo_w2 = 0;
		in >> nodo_a;
		in >> nodo_b;
		in >> costo_w1;
		in >> costo_w2;
		this->agregar_arista(nodo_a, nodo_b, costo_w1, costo_w2);
		count++;
	}
}

//Devuelve el camino minimo entre origen y destino(calcula el arbol, pero reconstruye solo el camino de origen a destino)
Camino Grafo::dijkstra(nodo_t origen, nodo_t destino, tipo_costo_t target_a_minimizar){
	vector<costo_t> costo_minimo;
	vector<nodo_t> predecesor;

	//inicializacion
	int n = this->cantidad_nodos;
	costo_minimo.clear();
	costo_minimo.resize(n, costo_infinito);
	costo_minimo[origen] = costo_nulo;

	predecesor.clear();
	predecesor.resize(n, predecesor_nulo);

	set<pair<costo_t, nodo_t> > cola;
	cola.insert(make_pair(costo_minimo[origen], origen));

	while(!cola.empty()){
		costo_t cost_to_u = cola.begin()->first;
		nodo_t nodo_u = cola.begin()->second;
		//marcar como visto en este caso es eliminarlo de la cola
		cola.erase(cola.begin());

		//Para cada vecino de u, obtengo el vector fila de la matriz de adyacencia
		//de dicho nodo e itero sobre todos, quedandome con las aristas presentes			
		int vecino_candidato = 0;
		vector<Arista> vecinos = this->mat_adyacencia[nodo_u];
		while(vecino_candidato < this->cantidad_nodos){
			if(vecinos[vecino_candidato].esta_presente()){
				//hacer algo con este vecino
				nodo_t nodo_v = vecino_candidato;
				costo_t cost_v = (target_a_minimizar == COSTO_W1) ? vecinos[nodo_v].obtener_costo_w1() : vecinos[nodo_v].obtener_costo_w2();
				int costo_a_v_pasando_por_u = cost_to_u + cost_v;
				//si mejora, sobreescribo el camino y su costo
				if(costo_a_v_pasando_por_u < costo_minimo[nodo_v]){
					//elimino de la cola de nodos la distancia vieja
					cola.erase(make_pair(costo_minimo[nodo_v], nodo_v));
					
					//actualizo estructuras
					costo_minimo[nodo_v] = costo_a_v_pasando_por_u;
					predecesor[nodo_v] = nodo_u;

					//sobreescribo la distancia en la cola de nodos la distancia nueva
					cola.insert(make_pair(costo_minimo[nodo_v], nodo_v));
				}
			}
			vecino_candidato++;
		}
	}

	//if(target_a_minimizar == COSTO_W1){
	//	cout << "Distancia minima de origen a destino respecto a W1: " << costo_minimo[destino] << endl;	
	//}else if(target_a_minimizar == COSTO_W2){		
	//	cout << "Distancia minima de origen a destino respecto a W2: " << costo_minimo[destino] << endl;
	//}	
	
	//armo camino
	Camino c(this->mat_adyacencia);
	
	nodo_t nodo = destino;	
	do{
		c.agregar_nodo_adelante(nodo);
		nodo = predecesor[nodo];
	}while(nodo != predecesor_nulo);
	return c;
}

list<Vecino> Grafo::obtener_adyacentes_en_comun(nodo_t i, nodo_t j){
	list<Vecino> res;
	vector<Arista> adyacentesFila_i = this->mat_adyacencia[i];
	vector<Arista> adyacentesFila_j = this->mat_adyacencia[j];
	for(int idx=0;idx<this->cantidad_nodos;idx++){
		if(adyacentesFila_i[idx].esta_presente() && adyacentesFila_j[idx].esta_presente()){
			//el nodo idx es adyacente de i y j.
			res.push_back(Vecino(i, j, idx, adyacentesFila_i[idx], adyacentesFila_j[idx]));
		}
	}	
	return res;
}