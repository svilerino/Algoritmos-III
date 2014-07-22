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

costo_t Arista::obtener_costo_w1() const {
	return this->costo_w1;	
}

costo_t Arista::obtener_costo_w2() const {
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
	int cant_nodos = mat_adyacencia.size();
	this->esta_en_camino.resize(cant_nodos, false);
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
	this->esta_en_camino[target] = true;
}

void Camino::agregar_nodo_adelante(nodo_t target){
	costo_t costo_agregado_w1 = 0;
	costo_t costo_agregado_w2 = 0;
	if(this->camino.size()>0){
		nodo_t penultimo_nodo = camino.front();
		costo_agregado_w1 = obtener_costo_w1_entre_nodos(penultimo_nodo, target);
		costo_agregado_w2 = obtener_costo_w2_entre_nodos(penultimo_nodo, target);
		this->costo_camino_w1 += costo_agregado_w1;	
		this->costo_camino_w2 += costo_agregado_w2;	
	}
	this->camino.push_front(target);
	this->esta_en_camino[target] = true;
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
		cerr << "Costo w1 infinito(no son adyacentes) entre nodos (" << i << ") y (" << j << ")" << endl;
		return costo_infinito;
	}	
}

//pre: 0 <= i <= j < cantidad_nodos y que i,j sean adyacentes
costo_t Camino::obtener_costo_w2_entre_nodos(nodo_t i, nodo_t j){
	if(this->mat_adyacencia[i][j].esta_presente()){
		return this->mat_adyacencia[i][j].obtener_costo_w2();
	}else{
		cerr << "Costo w2 infinito(no son adyacentes) entre nodos (" << i << ") y (" << j << ")" << endl;
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
		out << "(" << *it << ")" << "----[" << this->obtener_costo_w1_entre_nodos(*it, *runner_it) << ", " << this->obtener_costo_w2_entre_nodos(*it, *runner_it) << "]---->";
		++it;
		++runner_it;
	}	
	out << "(" << this->camino.back() << ")" << endl;
}

list<nodo_t>::const_iterator Camino::obtener_iterador_const_begin(){
	return this->camino.begin();
}

list<nodo_t>::const_iterator Camino::obtener_iterador_const_end(){
	return this->camino.end();
}

list<nodo_t>::iterator Camino::obtener_iterador_begin(){
	return this->camino.begin();
}

list<nodo_t>::iterator Camino::obtener_iterador_end(){
	return this->camino.end();
}

longuitud_t Camino::obtener_longuitud_camino(){
	return this->camino.size();
}

bool Camino::realizar_salto_entre_3_nodos(nodo_t nodo_target){
	list<nodo_t>::iterator runner_it = this->camino.begin();
	list<nodo_t>::iterator final_it = this->camino.end();
	while(runner_it != final_it){
		if(*runner_it == nodo_target){
			break;
		}
		++runner_it;
	}

	if(runner_it == final_it){		
		cerr << "Camino::realizar_salto_entre_3_nodos. No se encontro el nodo target pasado por parametro en el camino" << endl;
		cerr << "Nodo target (" << nodo_target << ")" << endl;
		cerr << "Camino: ";
		this->imprimir_camino(cerr);
		return false;
	}

	//aca vale *runner_it == nodo_target

	nodo_t nodo_i = *runner_it;
	
	runner_it++;	
	list<nodo_t>::iterator deletion_target = runner_it;
	nodo_t nodo_intermedio_viejo = *runner_it;
	
	runner_it++;
	nodo_t nodo_j = *runner_it;

	costo_t costo_i_intermedio_w1 = obtener_costo_w1_entre_nodos(nodo_i, nodo_intermedio_viejo);
	costo_t costo_i_intermedio_w2 = obtener_costo_w2_entre_nodos(nodo_i, nodo_intermedio_viejo);

	costo_t costo_intermedio_j_w1 = obtener_costo_w1_entre_nodos(nodo_intermedio_viejo, nodo_j);
	costo_t costo_intermedio_j_w2 = obtener_costo_w2_entre_nodos(nodo_intermedio_viejo, nodo_j);

	costo_t costo_i_j_w1 = obtener_costo_w1_entre_nodos(nodo_i, nodo_j);
	costo_t costo_i_j_w2 = obtener_costo_w2_entre_nodos(nodo_i, nodo_j);

	//elimino el nodo intermedio entre i y j
	this->camino.erase(deletion_target);
	this->esta_en_camino[nodo_intermedio_viejo] = false;

	//actualizo los costos del camino
	this->costo_camino_w1 = (this->costo_camino_w1 - (costo_i_intermedio_w1 + costo_intermedio_j_w1) + costo_i_j_w1);
	this->costo_camino_w2 = (this->costo_camino_w2 - (costo_i_intermedio_w2 + costo_intermedio_j_w2) + costo_i_j_w2);

	return true;
}

bool Camino::pertenece_a_camino(nodo_t target){
	return this->esta_en_camino[target];
}

//pre: at.obtener_nodo_i() y at.obtener_nodo_i() deben pertenecer al camino
//Se reemplazara la conexion i..loquesea..j por i -> encomun -> j indicado por el Vecino pasado
//por parametro. Devuelve true si se inserto, false sino.
bool Camino::mejorar_tripla(Vecino& at){
	nodo_t nodo_target = at.obtener_nodo_i();
	nodo_t nodo_sig_sig_target = at.obtener_nodo_j();
	nodo_t nodo_intermedio = at.obtener_nodo_comun();
	costo_t i_comun_w1 = at.obtener_arista_i_comun().obtener_costo_w1();
	costo_t i_comun_w2 = at.obtener_arista_i_comun().obtener_costo_w2();
	costo_t j_comun_w1 = at.obtener_arista_j_comun().obtener_costo_w1();
	costo_t j_comun_w2 = at.obtener_arista_j_comun().obtener_costo_w2();
	nodo_t intermedio_viejo = -1;

	list<nodo_t>::iterator insertion_target = this->camino.begin();
	list<nodo_t>::iterator final_it = this->camino.end();
	while(insertion_target != final_it){
		if(*insertion_target == nodo_target){	
			insertion_target++;
			intermedio_viejo = *insertion_target;
			break;
		}
		++insertion_target;
	}

	if(insertion_target == final_it){
		cerr << "Camino::mejorar_tripla. No se encontro el nodo target pasado por parametro en el camino" << endl;
		return false;
	}

	//aca vale que el iterator it apunta a target + 1

	//elimino el intermedio viejo
	nodo_t nodo_intermedio_viejo = *insertion_target;
	insertion_target = this->camino.erase(insertion_target);
	this->esta_en_camino[nodo_intermedio_viejo] = false;

	//inserto el nodo en el medio
	this->camino.insert(insertion_target, nodo_intermedio);
	this->esta_en_camino[nodo_intermedio] = true;

	//actualizo los costos
	//resto el costo entre i y j y sumo las 2 aristas nuevas

	costo_t costo_ij_w1 = obtener_costo_w1_entre_nodos(nodo_target, intermedio_viejo);
	costo_ij_w1 += obtener_costo_w1_entre_nodos(intermedio_viejo, nodo_sig_sig_target);

	costo_t costo_ij_w2 = obtener_costo_w2_entre_nodos(nodo_target, intermedio_viejo);
	costo_ij_w2 += obtener_costo_w2_entre_nodos(intermedio_viejo, nodo_sig_sig_target);
	            
    costo_t costo_i_comun_j_w1 = i_comun_w1 + j_comun_w1;
    costo_t costo_i_comun_j_w2 = i_comun_w2 + j_comun_w2;

	this->costo_camino_w1 = (this->obtener_costo_total_w1_camino() - costo_ij_w1 + costo_i_comun_j_w1);
	this->costo_camino_w2 = (this->obtener_costo_total_w2_camino() - costo_ij_w2 + costo_i_comun_j_w2);
	
	return true;
}

//pre: at.obtener_nodo_i() y at.obtener_nodo_i() deben pertenecer al camino
//Se reemplazara la conexion directa entre i y j por i -> encomun -> j indicado por el Vecino pasado
//por parametro
bool Camino::insertar_nodo(Vecino& at){
	nodo_t nodo_target = at.obtener_nodo_i();
	nodo_t nodo_sig_target = at.obtener_nodo_j();
	nodo_t nodo_intermedio = at.obtener_nodo_comun();
	costo_t i_comun_w1 = at.obtener_arista_i_comun().obtener_costo_w1();
	costo_t i_comun_w2 = at.obtener_arista_i_comun().obtener_costo_w2();
	costo_t j_comun_w1 = at.obtener_arista_j_comun().obtener_costo_w1();
	costo_t j_comun_w2 = at.obtener_arista_j_comun().obtener_costo_w2();

	list<nodo_t>::iterator insertion_target = this->camino.begin();
	list<nodo_t>::iterator final_it = this->camino.end();
	while(insertion_target != final_it){
		if(*insertion_target == nodo_target){
			insertion_target++;
			break;
		}
		++insertion_target;
	}

	if(insertion_target == final_it){
		cerr << "Camino::insertar_nodo. No se encontro el nodo target pasado por parametro en el camino" << endl;
		return false;
	}

	//aca vale que el iterator it apunta a target + 1

	//inserto el nodo en el medio
	this->camino.insert(insertion_target, nodo_intermedio);
	this->esta_en_camino[nodo_intermedio] = true;
	
	//actualizo los costos
	//resto la arista entre i y j y sumo las 2 aristas nuevas
	costo_t costo_ij_w1 = obtener_costo_w1_entre_nodos(nodo_target, nodo_sig_target);
	costo_t costo_ij_w2 = obtener_costo_w2_entre_nodos(nodo_target, nodo_sig_target);
	            
    costo_t costo_i_comun_j_w1 = i_comun_w1 + j_comun_w1;
    costo_t costo_i_comun_j_w2 = i_comun_w2 + j_comun_w2;

	this->costo_camino_w1 = (this->obtener_costo_total_w1_camino() - costo_ij_w1 + costo_i_comun_j_w1);
	this->costo_camino_w2 = (this->obtener_costo_total_w2_camino() - costo_ij_w2 + costo_i_comun_j_w2);
	return true;
}
// -------------- Vecino ---------------------------------

Vecino::Vecino(nodo_t i, nodo_t j, nodo_t comun, Arista desde_i, Arista desde_j){
	this->i = i;
	this->j = j;
	this->en_comun = comun;
	this->desde_i_a_comun = desde_i;
	this->desde_j_a_comun = desde_j;
}

Vecino::Vecino(){
	this->i = 0;
	this->j = 0;
	this->en_comun = 0;
	//aristas constructor por defecto
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

Camino Grafo::obtener_camino_vacio(){
	Camino c(this->mat_adyacencia);
	return c;
}

Grafo::Grafo(int cant_inicial_nodos) : camino_obtenido(mat_adyacencia) {
	this->cantidad_nodos=cant_inicial_nodos;
	this->cantidad_aristas = 0;
	nodo_src = 0;
	nodo_dst = 0;
	cota_w1 = 0;
	sol_valida = false;
		
	//inicializo matriz de adyacencia
	vector<Arista> vec_fila(cantidad_nodos, Arista(false, 0, 0));
	this->mat_adyacencia.resize(cantidad_nodos, vec_fila);
	
	//inicializo lista de adyacencia
	this->lista_adyacencia.resize(cantidad_nodos);

	//inicializo matriz de vecinos comunes
	vector<vector<Vecino> > vec_fila_vecinos(cantidad_nodos);
	this->vecinos_comunes.resize(cantidad_nodos, vec_fila_vecinos);
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
	
	//redimensiono lista adyacencias
	this->lista_adyacencia.resize(cantidad_nodos + cantidad_nodos_nuevos);

	//redimensiono matriz de vecinos comunes
	vector<vector<Vecino> > vec_fila_vecinos(this->cantidad_nodos + cantidad_nodos_nuevos);
	this->vecinos_comunes.resize(this->cantidad_nodos + cantidad_nodos_nuevos, vec_fila_vecinos);

	//actualizo cantidad_nodos total del grafo
	this->cantidad_nodos+=cantidad_nodos_nuevos;
}

void Grafo::agregar_arista(nodo_t i, nodo_t j, costo_t w1, costo_t w2){
	Arista arista = obtener_arista(i, j);
	//marco doble, no es un digrafo
	this->mat_adyacencia[i][j].marcar_presente(w1, w2);
	this->mat_adyacencia[j][i].marcar_presente(w1, w2);

	this->lista_adyacencia[i].push_back(make_pair(j, obtener_arista(i, j)));
	this->lista_adyacencia[j].push_back(make_pair(i, obtener_arista(i, j)));

	if(!arista.esta_presente()){
		this->cantidad_aristas++;
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
		}
		out << endl;
	}
	out << endl;
	out << endl;
}

void Grafo::imprimir_lista_adyacencia(ostream& out){	
	out << "Cantidad nodos: " << this->cantidad_nodos << endl;
	out << "Cantidad aristas: " << this->cantidad_aristas << endl;
	out << "Lista adyacencia:" << endl;
	for (int i = 0;i<this->cantidad_nodos;i++){
		out << "Vecinos de [" << i << "]: ";
		lista_adyacentes::iterator adyacentes_i_it = this->lista_adyacencia[i].begin();
		lista_adyacentes::iterator final_it = this->lista_adyacencia[i].end();
		while(adyacentes_i_it != final_it){
			out << "(" << adyacentes_i_it->first << ") --(" << this->mat_adyacencia[i][adyacentes_i_it->first].obtener_costo_w1() << ", " << this->mat_adyacencia[i][adyacentes_i_it->first].obtener_costo_w2() << ")--> ";
			adyacentes_i_it++;
		}
		out << "Nil" << endl;
	}
	out << endl;
	out << endl;
}

costo_t Grafo::obtener_limite_w1(){
	return this->cota_w1;
}

void Grafo::establecer_camino_solucion(Camino c){
	this->camino_obtenido = c;
}

Camino Grafo::obtener_camino_solucion(){
	return this->camino_obtenido;
}

int Grafo::obtener_cantidad_nodos(){
	return this->cantidad_nodos;
}

int Grafo::obtener_cantidad_aristas(){
	return this->cantidad_aristas;
}

void Grafo::serialize(ostream& out, formato_salida_t formato){
	if(this->sol_valida){
		out << this->camino_obtenido.obtener_costo_total_w1_camino() << " ";
		out << this->camino_obtenido.obtener_costo_total_w2_camino() << " ";
		out << this->camino_obtenido.obtener_longuitud_camino() << " ";
		list<nodo_t>::const_iterator it = this->camino_obtenido.obtener_iterador_const_begin();
		while(it != camino_obtenido.obtener_iterador_const_end()){
			out << ( (formato == FORMATO_1_N_CLOSED) ? ( (*it) + 1 ) : *it ) << " ";
			++it;
		}		
	}else{
		out << "no";
	}
}

bool Grafo::unserialize(istream& in, formato_entrada_t formato){
	//primera linea:
	//n m u v K
	//cant nodos, cant aristas, src, dst, K(cota w1)
	int cant_nodos_nuevos = 0, cantidad_aristas_nuevas = 0;
	in >> cant_nodos_nuevos;

	if(cant_nodos_nuevos == 0){
		return false;//es la ultima linea de la entrada!!
	}

	in >> cantidad_aristas_nuevas;
	in >> this->nodo_src;
	in >> this->nodo_dst;
	in >> this->cota_w1;

	if(formato == FORMATO_1_N_CLOSED){
		this->nodo_src--;
		this->nodo_dst--;
	}

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

		if(formato == FORMATO_1_N_CLOSED){
			nodo_a--;
			nodo_b--;
		}

		this->agregar_arista(nodo_a, nodo_b, costo_w1, costo_w2);
		count++;
	}

	//precalcular vecinos en comun
	for(int i=0;i<cant_nodos_nuevos;i++){
		for(int j=0;j<cant_nodos_nuevos;j++){
			precalcular_adyacentes_en_comun(i, j);			
		}
	}
	return true;
}

//Devuelve el camino minimo entre origen y destino(calcula el arbol, pero reconstruye solo el camino de origen a destino)
Camino Grafo::dijkstra(nodo_t origen, nodo_t destino, tipo_costo_t target_a_minimizar){
	vector<costo_t> costo_minimo;
	vector<nodo_t> predecesor;
	this->dijkstra(origen, target_a_minimizar, costo_minimo, predecesor);
	
	//armar camino entre origen y destino
	Camino c(this->mat_adyacencia);
	nodo_t nodo = destino;
	do{
		//cout << nodo << " " ;
		c.agregar_nodo_adelante(nodo);
		nodo = predecesor[nodo];
	}while(nodo != predecesor_nulo);
	return c;
}

//Aplica dijkstra desde nodo origen y calcula el arbol de caminos minimos por referencia a los vectores por parametro
void Grafo::dijkstra(nodo_t origen, tipo_costo_t target_a_minimizar, vector<costo_t>& costo_minimo, vector<nodo_t>& predecesor){
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
		lista_adyacentes::iterator adyacentes_i_it = this->lista_adyacencia[nodo_u].begin();
		lista_adyacentes::iterator final_it = this->lista_adyacencia[nodo_u].end();
		while(adyacentes_i_it != final_it){
			nodo_t nodo_v = adyacentes_i_it->first;

			costo_t cost_v = (target_a_minimizar == COSTO_W1) ? (adyacentes_i_it->second).obtener_costo_w1() : (adyacentes_i_it->second).obtener_costo_w2();
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
			adyacentes_i_it++;
		}
	}
}

void Grafo::breadth_first_search(nodo_t origen, vector<distancia_t>& distancias){
	distancias.clear();
	distancias.resize(this->cantidad_nodos, distancia_infinita);
	queue<nodo_t> cola;
	vector<bool> visitado(this->cantidad_nodos, false);//inicializo todos los nodos sin visitar
	cola.push(origen);
	visitado[origen] = true;
	distancias[origen] = 0;//dist(src, src) = 0

	while(!cola.empty()){
		nodo_t target = cola.front();//obtengo el primero
		cola.pop();//desencolo el primero

		//para todos los vecinos de target
		lista_adyacentes::iterator adyacentes_i_it = this->lista_adyacencia[target].begin();
		lista_adyacentes::iterator final_it = this->lista_adyacencia[target].end();
		while(adyacentes_i_it != final_it){
			nodo_t vecino_candidato = adyacentes_i_it->first;
			if(!visitado[vecino_candidato]){
				cola.push(vecino_candidato);
				visitado[vecino_candidato] = true;
				distancias[vecino_candidato] = distancias[target] + 1;
			}
			adyacentes_i_it++;
		}
	}
}

//para impl. con listas de adyacencia, es O(n**2) si no estan ordenados haciendo 2 fors
//se puede mejorar, "ordenando" las listas en espacios auxiliares en O(nlogn) y intersecando en O(n)
//dejando complejidad de O(nlogn), de cualquier manera, como se nos pidio que fuera polinomial unicamente
//uso la matriz de adyacencia, y en O(n) recorro los vecinos de ambos, y donde se cumpla vecindad en ambos
//lo selecciono como vecino en comun.
void Grafo::precalcular_adyacentes_en_comun(nodo_t i, nodo_t j){
	vector<Vecino> res;
	vector<Arista> adyacentesFila_i = this->mat_adyacencia[i];
	vector<Arista> adyacentesFila_j = this->mat_adyacencia[j];
	for(int idx=0;idx<this->cantidad_nodos;idx++){
		if(adyacentesFila_i[idx].esta_presente() && adyacentesFila_j[idx].esta_presente()){
			//el nodo idx es adyacente de i y j.
			res.push_back(Vecino(i, j, idx, adyacentesFila_i[idx], adyacentesFila_j[idx]));
		}
	}		
	this->vecinos_comunes[i][j] = res;
}

vector<Vecino> Grafo::obtener_adyacentes_en_comun(nodo_t i, nodo_t j){	
	//cout << "Vecinos comunes de (" << i << ") y (" << j << ") : ";
	#ifdef VECINOS_COMUNES_LAZY
		vector<Vecino> res;
		vector<Arista> adyacentesFila_i = this->mat_adyacencia[i];
		vector<Arista> adyacentesFila_j = this->mat_adyacencia[j];
		for(int idx=0;idx<this->cantidad_nodos;idx++){
			if(adyacentesFila_i[idx].esta_presente() && adyacentesFila_j[idx].esta_presente()){
				//el nodo idx es adyacente de i y j.
				res.push_back(Vecino(i, j, idx, adyacentesFila_i[idx], adyacentesFila_j[idx]));
				//cout << "(" << idx << ") --->";
			}
		}		
		//cout << "Nil" << endl;
		return res;
	#else
		//list<Vecino> tmp = this->vecinos_comunes[i][j];
		//for(Vecino v : tmp){
			//cout << "(" << v.obtener_nodo_comun() << ") ---> ";
		//}
		//cout << "Nil" << endl;
		return this->vecinos_comunes[i][j];
	#endif
}

vector<Arista> Grafo::obtener_vector_fila_vecinos(nodo_t target){
	return this->mat_adyacencia[target];
}

lista_adyacentes Grafo::obtener_lista_vecinos(nodo_t target){
	return this->lista_adyacencia[target];
}

nodo_t Grafo::obtener_nodo_origen(){
	return this->nodo_src;
}

nodo_t Grafo::obtener_nodo_destino(){
	return this->nodo_dst;
}

bool Grafo::mejorar_conexion_salteando(nodo_t nodo_i, nodo_t nodo_j, costo_t costo_ij_w1, costo_t costo_ij_w2, costo_t total_w1, costo_t total_w2,
 Arista& mejor_vecino){
	//es un grafo no dirigido, es simetrico buscar si a i es adyacente a j o viceversa
	//me fijo si i y j son adyacentes
	Arista candidato_a_mejor_camino = this->obtener_arista(nodo_i, nodo_j);
	if(candidato_a_mejor_camino.esta_presente()){
		//FIX. ADEMAS DE EXISTIR LA ARISTA, EL CAMINO DIRECTO TIENE QUE SER MEJOR ESTRICTO QUE LA SUMA DE LAS DOS ARISTAS EXISTENTES

		costo_t hipotetico_w1_total_camino = (total_w1 - (costo_ij_w1) + candidato_a_mejor_camino.obtener_costo_w1());
		bool es_mejora_factible = (candidato_a_mejor_camino.obtener_costo_w2() < costo_ij_w2) /*mejora la arista directa*/&&
			(hipotetico_w1_total_camino <= this->obtener_limite_w1()); /*no se pasa de la cota*/
		if(es_mejora_factible){
			//la arista directa mejora el camino en w2 pero no se pasa de w1 el camino
			
			//OJO en esta linea!: si asigno candidato_a_mejor_camino estoy devolviendo una referencia a una variable de stack y catapunchis!
			mejor_vecino = this->obtener_arista(nodo_i, nodo_j);
			return true;
		}else{
			//o no mejora o se pasa w1 => false
			return false;
		}
	}else{
		//no hay arista directa => false
		return false;		
	}
}

bool Grafo::mejorar_conexion_entre_pares(nodo_t nodo_i, nodo_t nodo_j, costo_t costo_ij_w1, costo_t costo_ij_w2, costo_t total_w1, costo_t total_w2,
 Vecino& mejor_vecino){
	//SEA LA VECINDAD Vc = {Caminos C' tal que difieran de C en tan solo un nodo}

	//DEFINO UNA TABOO LIST COMO UNA LISTA EN LA CUAL VOY A DESCARTAR LAS SOLUCIONES QUE USEN NODOS DE DICHA LISTA
	//ESTO ES NECESARIO PARA EVITAR LA GENERACION DE CICLOS EN EL CASO DE AGREGAR UN NODO AL SUBDIVIDIR UNA ARISTA O REEMPLAZAR UN NODO
	//INTERMEDIO ENTRE OTROS DOS. SI EL NODO ELEGIDO YA PERTENECIA AL CAMINO, SE GENERAN CICLOS Y NO QUEREMOS ESTO YA QUE BUSCAMOS UN CAMINO MINIMO.
	//SI MANTENEMOS DISJUNTOS EL CONJUNTO DE NODOS DEL CAMINO ACTUAL Y LOS NODOS RESTANTES DEL GRAFO, CUALQUIER ELECCION QUE HAGAMOS NO GENERARA CICLOS.

	//OTRA OPCION SERIA NO RESTRINGIR LA ELECCION DE LAS SOLUCIONES DE LA VECINDAD, PERO DEBERIAMOS LUEGO REALIZAR UNA PODA DE CICLOS DEL CAMINO
	//CREEMOS QUE ESTA OPCION SERIA MEJOR, PORQUE AGREGANDO EL NODO SE MEJORA LA SOLUCION, Y ELIMINANDO EL CICLO, SE MEJORA AUN MAS, PERO A NUESTRO ENTENDER
	//DEJA DE SER BUSQUEDA LOCAL, DADO QUE LA SOLUCION QUE SURJA DE ESTO PUEDE NO ESTAR EN LA VECINDAD Vc

    //busco la conexion entre i y j pasando por un nodo intermedio tal que minimice la distancia de w2 sin pasarme de la cota total de w1 para el camino
 	vector<Vecino> vecinosEnComun = this->obtener_adyacentes_en_comun(nodo_i, nodo_j);
	vector<Vecino>::iterator vecinos_it = vecinosEnComun.begin();
	vector<Vecino>::iterator final_vecinos = vecinosEnComun.end();
	
    //me fijo todos los caminos alternativos agregando un nodo entre los nodos ij,
    vector<Vecino>::iterator mejor_vecino_it = vecinosEnComun.end();//inicializamos en algo que indique que no hay mejora
	costo_t mejor_camino_ij_w2 = costo_ij_w2;
	while(vecinos_it != final_vecinos){
        costo_t i_comun_w1 = vecinos_it->obtener_arista_i_comun().obtener_costo_w1();
        costo_t i_comun_w2 = vecinos_it->obtener_arista_i_comun().obtener_costo_w2();
        costo_t j_comun_w1 = vecinos_it->obtener_arista_j_comun().obtener_costo_w1();
        costo_t j_comun_w2 = vecinos_it->obtener_arista_j_comun().obtener_costo_w2();
        costo_t costo_i_comun_j_w1 = i_comun_w1 + j_comun_w1;
        costo_t costo_i_comun_j_w2 = i_comun_w2 + j_comun_w2;
		costo_t hipotetico_w1_total_camino = (total_w1 - costo_ij_w1 + costo_i_comun_j_w1);
		costo_t hipotetico_w2_total_camino = (total_w2 - costo_ij_w2 + costo_i_comun_j_w2);
			
		//veamos si el camino es una solucion factible
		if(hipotetico_w1_total_camino <= this->obtener_limite_w1()){
			//es factible, veamos si mejora al ultimo mejor revisado

			//VEAMOS ADEMAS, QUE NO ESTE EN NUESTRA TABOO LIST(QUE NO PERTENEZCA AL CAMINO DE LA SOL. ACTUAL)			
			nodo_t nodo_comun = vecinos_it->obtener_nodo_comun();
			if(!this->camino_obtenido.pertenece_a_camino(nodo_comun)){//puedo verlo en O(1)
				//el nodo no esta en la taboo list
				if(costo_i_comun_j_w2 < mejor_camino_ij_w2){
					//encontre mejora, actualizo variables
					mejor_camino_ij_w2 = hipotetico_w2_total_camino;
					mejor_vecino_it = vecinos_it;
				}				
			}else{
				//taboo list skipped!
				#ifdef DEBUG_MESSAGES_ON	
					#ifdef CYCLE_PREVENT_MESSAGE_ON
					cout << "Salteamos el nodo (" << nodo_comun << ") entre (" << nodo_i << ") y (" << nodo_j << ") como candidato a mejorar la solucion actual porque al pertenecer al camino generaria un ciclo" << endl;
					#endif
				#endif
			}
		}
		++vecinos_it;
	}
	if(mejor_vecino_it != final_vecinos){
		mejor_vecino = *mejor_vecino_it;
		return true;
	}
	return false;
 }

//Defino la vecindad del camino como Vc = {Todos los caminos c' que difieren en un nodo de c}
//Intento mejorar un camino vk---->vk+1 con otro vk---->vj---->vk+1 tal que mejora w2 y w1 no se pasa en el costo total del camino
//Primero reviso todos los pares de nodos del camino buscando posibles subdivisiones que mejoren la solucion.
//Me voy a quedar unicamente (si existen varias) con la mejor solucion de la vecindad, notemos que revisar toda la vecindad es cuadratico
//La longuitud de un camino simple puede acotarse por la cantidad de nodos n, luego hay n-1 pares de nodos en el camino
//Para cada par, es lineal la obtencion de los vecinos en comun, y al ser el camino una lista enlazada, la modificacion tiene costo
//constante O(1), en total esto nos da un costo cuadratico O(n**2)
int Grafo::busqueda_local_entre_pares_insertando(Camino& solucion_actual, Vecino& conexion_ij_minima_w2){
	//cout << "-------------------------------Comienza iteracion de busqueda local insertando entre pares--------------------------------" << endl;
	list<nodo_t>::const_iterator it = solucion_actual.obtener_iterador_const_begin();
	list<nodo_t>::const_iterator runner_it = solucion_actual.obtener_iterador_const_begin();
	runner_it++;
	list<nodo_t>::const_iterator final_camino = solucion_actual.obtener_iterador_const_end();
	costo_t total_w1 = solucion_actual.obtener_costo_total_w1_camino();
	costo_t total_w2 = solucion_actual.obtener_costo_total_w2_camino();

	costo_t total_inicial_w2 = total_w2;

	//variables para guardar la mejor solucion de la vecindad
	costo_t mejor_costo_w2 = costo_infinito;	
	list<nodo_t>::const_iterator punto_de_insercion_mejora_it = final_camino;
	bool hay_mejoras_para_el_camino = false;

	//busco la mejor solucion en la vecindad
	#ifdef DEBUG_MESSAGES_ON
		cout << "Solucion actual: ";
		solucion_actual.imprimir_camino(cout);
	    cout << "Costo total del camino actual:   W1: " << total_w1 << "    W2: " << total_w2 << endl;
	#endif
	while(runner_it != final_camino){
        nodo_t nodo_i = *it;
        nodo_t nodo_j = *runner_it;
        costo_t costo_ij_w1 = solucion_actual.obtener_costo_w1_entre_nodos(nodo_i, nodo_j);
    	costo_t costo_ij_w2 = solucion_actual.obtener_costo_w2_entre_nodos(nodo_i, nodo_j);

    	Vecino mejor_conexion_ij;
    	//le paso una ref a una var tipo vecino, si devuelve true, se escribe por referencia el mejor camino, sino, no cambia lo que le pasamos.
    	//cout << "Buscando mejorar la conexion (" << nodo_i << ")----[" << costo_ij_w1 << ", " << costo_ij_w2 << "]---->(" << nodo_j << ") agregando un nodo intermedio..." << endl;
        bool encontre_mejora = mejorar_conexion_entre_pares(nodo_i, nodo_j,
        										 costo_ij_w1, costo_ij_w2,
        										 total_w1, total_w2,
        										 mejor_conexion_ij);

		//hay que ver si encontramos una mejora
		if(encontre_mejora){//la funcion asegura que si dio true, me da el vecino por puntero en mejor_conexion_ij
            //nodo_t nodo_comun = mejor_conexion_ij.obtener_nodo_comun();
            costo_t i_comun_w1 = mejor_conexion_ij.obtener_arista_i_comun().obtener_costo_w1();
            costo_t i_comun_w2 = mejor_conexion_ij.obtener_arista_i_comun().obtener_costo_w2();
            costo_t j_comun_w1 = mejor_conexion_ij.obtener_arista_j_comun().obtener_costo_w1();
            costo_t j_comun_w2 = mejor_conexion_ij.obtener_arista_j_comun().obtener_costo_w2();
            costo_t costo_i_comun_j_w1 = i_comun_w1 + j_comun_w1;
            costo_t costo_i_comun_j_w2 = i_comun_w2 + j_comun_w2;
			//cout << "\tSe encontro una posible mejora. El mejor sendero entre los nodos que se encontro en todos los vecinos entre (" << nodo_i << ") y (" << nodo_j << ") es " << endl;
            //cout << "\tCamino (" << nodo_i << ")----[" << i_comun_w1 << ", " << i_comun_w2 <<  "]---->(" << nodo_comun << ")----[" << j_comun_w1 << ", " << j_comun_w2 << "]---->(" << nodo_j;
            //cout <<	") Nuevo costo del sendero entre (" << nodo_i << ") y (" << nodo_j << ") aplicando a esta modificacion:    W1: " << costo_i_comun_j_w1 << "     W2: " << costo_i_comun_j_w2 << endl;

			costo_t hipotetico_w1_total_camino = (total_w1 - costo_ij_w1 + costo_i_comun_j_w1);
			costo_t hipotetico_w2_total_camino = (total_w2 - costo_ij_w2 + costo_i_comun_j_w2);

			if( (hipotetico_w2_total_camino < mejor_costo_w2) && (hipotetico_w1_total_camino <= this->obtener_limite_w1())){
				mejor_costo_w2 = hipotetico_w2_total_camino;
				conexion_ij_minima_w2 = mejor_conexion_ij;
				punto_de_insercion_mejora_it = it;
				hay_mejoras_para_el_camino = true;
			}

			//cout << "\tSi aplicamos este cambio los costos del camino total quedarian:   W1: " << hipotetico_w1_total_camino << "    W2: "  << hipotetico_w2_total_camino << endl;
		}//else{
			//cout << "\tNo se encontro mejora." << endl;
		//}
        //cout << endl;
		++it;
		++runner_it;
	}

	//Si hubo mejoras, tengo guardada la mejor
	if(hay_mejoras_para_el_camino){
		#ifdef DEBUG_MESSAGES_ON
			cout << "Se encontro mejora. La mejor mejora encontrada entre todos los pares de nodos fue:" << endl;
		#endif
		//insertar conexion_ij_minima_w2 en punto_de_insercion_mejora_it
		//y actualizar todos los atributos necesarios.

		nodo_t nodo_i = conexion_ij_minima_w2.obtener_nodo_i();
		nodo_t nodo_j = conexion_ij_minima_w2.obtener_nodo_j();
		nodo_t nodo_comun = conexion_ij_minima_w2.obtener_nodo_comun();
        costo_t i_comun_w1 = conexion_ij_minima_w2.obtener_arista_i_comun().obtener_costo_w1();
        costo_t i_comun_w2 = conexion_ij_minima_w2.obtener_arista_i_comun().obtener_costo_w2();
        costo_t j_comun_w1 = conexion_ij_minima_w2.obtener_arista_j_comun().obtener_costo_w1();
        costo_t j_comun_w2 = conexion_ij_minima_w2.obtener_arista_j_comun().obtener_costo_w2();
        costo_t costo_i_comun_j_w1 = i_comun_w1 + j_comun_w1;
        costo_t costo_i_comun_j_w2 = i_comun_w2 + j_comun_w2;		
        #ifdef DEBUG_MESSAGES_ON
	        cout << "Se agregara un nodo intermedio en: (" << nodo_i << ")----[" << i_comun_w1 << ", " << i_comun_w2 <<  "]---->(" << nodo_comun << ")----[" << j_comun_w1 << ", " << j_comun_w2 << "]---->(" << nodo_j << ")" << endl;
	        cout <<	"Nuevo costo del sendero entre (" << nodo_i << ") y (" << nodo_j << ") aplicando a esta modificacion:    W1: " << costo_i_comun_j_w1 << "     W2: " << costo_i_comun_j_w2 << endl;	    	       	       
		#endif
		return (total_inicial_w2 - mejor_costo_w2);//mejora respecto a w2 en el camino del inicial al actual
	}else{
		#ifdef DEBUG_MESSAGES_ON
			cout << "No se pudo mejorar la solucion." << endl;
		#endif
		return 0;//mejoro en 0 el camino respecto a w2
	}
}

int Grafo::busqueda_local_entre_triplas_reemplazando_intermedio(Camino& solucion_actual, Vecino& conexion_ij_minima_w2){
	//Caso en que reemplazo vk+1 por otro vecino comun vj, convirtiendo vk---->vk+1---->vk+2 en vk---->vj---->vk+2 tal que mejora w2 y w1 no se pasa en el costo total del camino	
	//cout << "-------------------------------Comienza iteracion de busqueda local reemplazando intermedio--------------------------------" << endl;
	if(solucion_actual.obtener_longuitud_camino()<3){
		#ifdef DEBUG_MESSAGES_ON
			cerr << "Camino de menos de 3 nodos. No se puede mejorar nada." << endl;
		#endif		
		return false;
	}

	//aca vale size(camino)>=3

	list<nodo_t>::const_iterator it = solucion_actual.obtener_iterador_const_begin();
	list<nodo_t>::const_iterator it_sig = solucion_actual.obtener_iterador_const_begin();
	list<nodo_t>::const_iterator runner_it = solucion_actual.obtener_iterador_const_begin();
	it_sig++;
	runner_it++;runner_it++;

	list<nodo_t>::const_iterator final_camino = solucion_actual.obtener_iterador_const_end();
	costo_t total_w1 = solucion_actual.obtener_costo_total_w1_camino();
	costo_t total_w2 = solucion_actual.obtener_costo_total_w2_camino();

	costo_t total_inicial_w2 = total_w2;

	//variables para guardar la mejor solucion de la vecindad
	costo_t mejor_costo_w2 = costo_infinito;
	list<nodo_t>::const_iterator punto_de_insercion_mejora_it = final_camino;
	bool hay_mejoras_para_el_camino = false;

	//busco la mejor solucion en la vecindad
	#ifdef DEBUG_MESSAGES_ON
		cout << "Solucion actual: ";
		solucion_actual.imprimir_camino(cout);
	    cout << "Costo total del camino actual:   W1: " << total_w1 << "    W2: " << total_w2 << endl;
	#endif
	while(runner_it != final_camino){
        nodo_t nodo_i = *it;
        nodo_t nodo_medio = *it_sig;
        nodo_t nodo_j = *runner_it;

        costo_t costo_i_medio_w1 = solucion_actual.obtener_costo_w1_entre_nodos(nodo_i, nodo_medio);
    	costo_t costo_i_medio_w2 = solucion_actual.obtener_costo_w2_entre_nodos(nodo_i, nodo_medio);
        costo_t costo_medio_j_w1 = solucion_actual.obtener_costo_w1_entre_nodos(nodo_medio, nodo_j);
    	costo_t costo_medio_j_w2 = solucion_actual.obtener_costo_w2_entre_nodos(nodo_medio, nodo_j);


    	//cout << "Intentando mejorar el nodo intermedio entre (" << nodo_i << ")----[" << costo_i_medio_w1 << ", " << costo_i_medio_w2;
    	//cout << "]---->(" << nodo_medio << ")----[" << costo_medio_j_w1 << ", " << costo_medio_j_w2 << "]---->(" << nodo_j << ")" << endl;

    	Vecino mejor_conexion_ij;
    	//le paso una ref a una var tipo Vecino, si devuelve true, se escribe por referencia el mejor camino, sino, no cambia lo que le pasamos.
        bool encontre_mejora = mejorar_conexion_entre_pares(nodo_i, nodo_j,
        										 (costo_i_medio_w1 + costo_medio_j_w1), (costo_i_medio_w2 + costo_medio_j_w2), 
        										 total_w1, total_w2,
        										 mejor_conexion_ij);

		//hay que ver si encontramos una mejora
		if(encontre_mejora){//la funcion asegura que si dio true, me da el vecino por puntero en mejor_conexion_ij
            //nodo_t nodo_comun = mejor_conexion_ij.obtener_nodo_comun();
            costo_t i_comun_w1 = mejor_conexion_ij.obtener_arista_i_comun().obtener_costo_w1();
            costo_t i_comun_w2 = mejor_conexion_ij.obtener_arista_i_comun().obtener_costo_w2();
            costo_t j_comun_w1 = mejor_conexion_ij.obtener_arista_j_comun().obtener_costo_w1();
            costo_t j_comun_w2 = mejor_conexion_ij.obtener_arista_j_comun().obtener_costo_w2();
            costo_t costo_i_comun_j_w1 = i_comun_w1 + j_comun_w1;
            costo_t costo_i_comun_j_w2 = i_comun_w2 + j_comun_w2;
			//cout << "\tSe encontro una posible mejora. El mejor sendero entre los nodos que se encontro en todos los vecinos entre (" << nodo_i << ") y (" << nodo_j << ") es " << endl;
            //cout << "\tCamino (" << nodo_i << ")----[" << i_comun_w1 << ", " << i_comun_w2 <<  "]---->(" << nodo_comun << ")----[" << j_comun_w1 << ", " << j_comun_w2 << "]---->(" << nodo_j;
            //cout <<	") Nuevo costo del sendero entre (" << nodo_i << ") y (" << nodo_j << ") aplicando a esta modificacion:    W1: " << costo_i_comun_j_w1 << "     W2: " << costo_i_comun_j_w2 << endl;

			costo_t hipotetico_w1_total_camino = (total_w1 - (costo_i_medio_w1 + costo_medio_j_w1) + costo_i_comun_j_w1);
			costo_t hipotetico_w2_total_camino = (total_w2 - (costo_i_medio_w2 + costo_medio_j_w2) + costo_i_comun_j_w2);

			if( (hipotetico_w2_total_camino < mejor_costo_w2) && (hipotetico_w1_total_camino <= this->obtener_limite_w1())){
				mejor_costo_w2 = hipotetico_w2_total_camino;
				conexion_ij_minima_w2 = mejor_conexion_ij;
				punto_de_insercion_mejora_it = it;
				hay_mejoras_para_el_camino = true;
			}

			//cout << "\tSi aplicamos este cambio los costos del camino total quedarian:   W1: " << hipotetico_w1_total_camino << "    W2: "  << hipotetico_w2_total_camino << endl;
		}//else{
			//cout << "\tNo se encontro mejora." << endl;
		//}

        //cout << endl;
		++it;
		++it_sig;
		++runner_it;
	}

	//Si hubo mejoras, tengo guardada la mejor
	if(hay_mejoras_para_el_camino){
		#ifdef DEBUG_MESSAGES_ON
			cout << "Se encontro mejora. La mejor mejora encontrada entre todos los pares de nodos fue:" << endl;
		#endif
		
		//reemplazar conexion_ij_minima_w2 en punto_de_insercion_mejora_it y punto_de_insercion_mejora_it + 2
		//y actualizar todos los atributos necesarios.

		nodo_t nodo_i = conexion_ij_minima_w2.obtener_nodo_i();
		nodo_t nodo_j = conexion_ij_minima_w2.obtener_nodo_j();
		nodo_t nodo_comun = conexion_ij_minima_w2.obtener_nodo_comun();
        costo_t i_comun_w1 = conexion_ij_minima_w2.obtener_arista_i_comun().obtener_costo_w1();
        costo_t i_comun_w2 = conexion_ij_minima_w2.obtener_arista_i_comun().obtener_costo_w2();
        costo_t j_comun_w1 = conexion_ij_minima_w2.obtener_arista_j_comun().obtener_costo_w1();
        costo_t j_comun_w2 = conexion_ij_minima_w2.obtener_arista_j_comun().obtener_costo_w2();
        costo_t costo_i_comun_j_w1 = i_comun_w1 + j_comun_w1;
        costo_t costo_i_comun_j_w2 = i_comun_w2 + j_comun_w2;		
        #ifdef DEBUG_MESSAGES_ON
	        cout << "Se reemplazo el nodo intermedio en: (" << nodo_i << ")----[" << i_comun_w1 << ", " << i_comun_w2 <<  "]---->(" << nodo_comun << ")----[" << j_comun_w1 << ", " << j_comun_w2 << "]---->(" << nodo_j << ")" << endl;
	        cout <<	"Nuevo costo del sendero entre (" << nodo_i << ") y (" << nodo_j << ") aplicando a esta modificacion:    W1: " << costo_i_comun_j_w1 << "     W2: " << costo_i_comun_j_w2 << endl;
	    #endif
	    return (total_inicial_w2 - mejor_costo_w2);
	}else{
		#ifdef DEBUG_MESSAGES_ON
			cout << "No se pudo mejorar la solucion." << endl;
		#endif
		return 0;
	}
}

int Grafo::busqueda_local_entre_triplas_salteando(Camino& solucion_actual, list<nodo_t>::const_iterator& punto_de_salto_it){
	//Caso en los que salteo un nodo vk---->vk+1---->vk+2 convirtiendolo en vk---->vk+2 tal que mejora w2 y w1 no se pasa en el costo total del camino	
	//cout << "-------------------------------Comienza iteracion de busqueda local salteando--------------------------------" << endl;
	if(solucion_actual.obtener_longuitud_camino()<3){
		#ifdef DEBUG_MESSAGES_ON
			cerr << "Camino de menos de 3 nodos. No se puede mejorar nada." << endl;
		#endif
		return false;
	}

	//aca vale size(camino)>=3

	list<nodo_t>::const_iterator it = solucion_actual.obtener_iterador_const_begin();
	list<nodo_t>::const_iterator it_sig = solucion_actual.obtener_iterador_const_begin();
	list<nodo_t>::const_iterator runner_it = solucion_actual.obtener_iterador_const_begin();
	it_sig++;
	runner_it++;runner_it++;

	list<nodo_t>::const_iterator final_camino = solucion_actual.obtener_iterador_const_end();
	costo_t total_w1 = solucion_actual.obtener_costo_total_w1_camino();
	costo_t total_w2 = solucion_actual.obtener_costo_total_w2_camino();

	costo_t total_inicial_w2 = total_w2;

	//variables para guardar la mejor solucion de la vecindad
	costo_t mejor_costo_w2 = costo_infinito;
	Arista conexion_ij_minima_w2;
	punto_de_salto_it = final_camino;
	bool hay_mejoras_para_el_camino = false;

	//busco la mejor solucion en la vecindad
	#ifdef DEBUG_MESSAGES_ON
		cout << "Solucion actual: ";
		solucion_actual.imprimir_camino(cout);
	    cout << "Costo total del camino actual:   W1: " << total_w1 << "    W2: " << total_w2 << endl;
	#endif
	while(runner_it != final_camino){
        nodo_t nodo_i = *it;
        nodo_t nodo_medio = *it_sig;
        nodo_t nodo_j = *runner_it;

        costo_t costo_i_medio_w1 = solucion_actual.obtener_costo_w1_entre_nodos(nodo_i, nodo_medio);
    	costo_t costo_i_medio_w2 = solucion_actual.obtener_costo_w2_entre_nodos(nodo_i, nodo_medio);
        costo_t costo_medio_j_w1 = solucion_actual.obtener_costo_w1_entre_nodos(nodo_medio, nodo_j);
    	costo_t costo_medio_j_w2 = solucion_actual.obtener_costo_w2_entre_nodos(nodo_medio, nodo_j);


    	//cout << "Intentando remover el nodo intermedio entre (" << nodo_i << ")----[" << costo_i_medio_w1 << ", " << costo_i_medio_w2;
    	//cout << "]---->(" << nodo_medio << ")----[" << costo_medio_j_w1 << ", " << costo_medio_j_w2 << "]---->(" << nodo_j << ")" << endl;

    	Arista mejor_conexion_ij;
    	//le paso una ref a una var tipo arista, si devuelve true, se escribe por referencia el mejor camino, sino, no cambia lo que le pasamos.
        bool encontre_mejora = mejorar_conexion_salteando(nodo_i, nodo_j,
        										 (costo_i_medio_w1 + costo_medio_j_w1), (costo_i_medio_w2 + costo_medio_j_w2), 
        										 total_w1, total_w2,
        										 mejor_conexion_ij);

		//hay que ver si encontramos una mejora
		if(encontre_mejora){//la funcion asegura que si dio true, me da el vecino por puntero en mejor_conexion_ij
			costo_t costo_ij_directo_w1 = mejor_conexion_ij.obtener_costo_w1();
			costo_t costo_ij_directo_w2 = mejor_conexion_ij.obtener_costo_w2();
			//cout << "\tSe encontro una posible mejora. Se encontro una arista directa entre (" << nodo_i << ") y (" << nodo_j << ") y es " << endl;
            //cout << "\tCamino (" << nodo_i << ")----[" << costo_ij_directo_w1 << ", " << costo_ij_directo_w2 <<  "]---->(" << nodo_j << ")";
            //cout <<	" Nuevo costo del sendero entre (" << nodo_i << ") y (" << nodo_j << ") aplicando a esta modificacion:    W1: " << costo_ij_directo_w1 << "     W2: " << costo_ij_directo_w2 << endl;

			costo_t hipotetico_w1_total_camino = (total_w1 - (costo_i_medio_w1 + costo_medio_j_w1) + costo_ij_directo_w1);
			costo_t hipotetico_w2_total_camino = (total_w2 - (costo_i_medio_w2 + costo_medio_j_w2) + costo_ij_directo_w2);

			if( (hipotetico_w2_total_camino < mejor_costo_w2) && (hipotetico_w1_total_camino <= this->obtener_limite_w1())){
				mejor_costo_w2 = hipotetico_w2_total_camino;
				hay_mejoras_para_el_camino = true;				
				conexion_ij_minima_w2 = mejor_conexion_ij;
				punto_de_salto_it = it;
			}

			//cout << "\tSi aplicamos este cambio los costos del camino total quedarian:   W1: " << hipotetico_w1_total_camino << "    W2: "  << hipotetico_w2_total_camino << endl;
		}

        //cout << endl;
		++it;
		++it_sig;
		++runner_it;
	}

	//Si hubo mejoras, tengo guardada la mejor
	if(hay_mejoras_para_el_camino){
		#ifdef DEBUG_MESSAGES_ON
			cout << "Se encontro mejora. La mejor mejora encontrada entre todos los pares de nodos fue:" << endl;
		#endif
		//Realizar el salto directo entre 2 nodos dada la arista obtenida
		list<nodo_t>::const_iterator nodo_j_it = punto_de_salto_it;
		nodo_j_it++;nodo_j_it++;

		nodo_t nodo_i = *punto_de_salto_it;
		nodo_t nodo_j = *nodo_j_it;

		costo_t costo_ij_directo_w1 = conexion_ij_minima_w2.obtener_costo_w1();
		costo_t costo_ij_directo_w2 = conexion_ij_minima_w2.obtener_costo_w2();
		#ifdef DEBUG_MESSAGES_ON
			cout << "\tSe encontro una arista directa entre (" << nodo_i << ") y (" << nodo_j << ") y es " << endl;
	        cout << "\tCamino (" << nodo_i << ")----[" << costo_ij_directo_w1 << ", " << costo_ij_directo_w2 <<  "]---->(" << nodo_j << ")";
	        cout <<	" Nuevo costo del sendero entre (" << nodo_i << ") y (" << nodo_j << ") aplicando a esta modificacion:    W1: " << costo_ij_directo_w1 << "     W2: " << costo_ij_directo_w2 << endl;
       	#endif
	    return (total_inicial_w2 - mejor_costo_w2);
	}else{
		#ifdef DEBUG_MESSAGES_ON
			cout << "No se pudo mejorar la solucion." << endl;
		#endif
		return 0;
	}
}

int Grafo::busqueda_local(tipo_ejecucion_bqlocal_t tipo_ejecucion){
	//decido que busquedas se van a correr
	bool bql_subdiv = false;
	bool bql_mejorar = false;
	bool bql_contraer = false;
	switch(tipo_ejecucion){
		case BQL_SUBDIVIDIR_PARES:
			bql_subdiv = true;
			break;
		case BQL_CONTRAER_TRIPLAS_A_PARES:		
			bql_contraer = true;
			break;
		case BQL_MEJORAR_CONEXION_TRIPLAS:		
			bql_mejorar = true;
			break;
		case BQL_COMBINAR:
			bql_subdiv = true;
			bql_contraer = true;
			bql_mejorar = true;
			break;
		default:
			cerr << "busqueda_local: tipo ejecucion invalido!" << endl;
			return 0;	
			break;
	}

	//establezco solucion actual y corro los algoritmos seleccionados previamente
	Camino solucion_actual = this->camino_obtenido;
	costo_t mejora_en_w2_bql_entre_pares = 0;
	Vecino conexion_ij_minima_w2_entre_pares;

	costo_t mejora_en_w2_entre_triplas_reemplazando = 0;
	Vecino conexion_ij_minima_w2_entre_triplas;
	
	costo_t mejora_en_w2_entre_triplas_salteando = 0;
	list<nodo_t>::const_iterator punto_de_salto_it = solucion_actual.obtener_iterador_const_end();

	if(bql_subdiv){
		#ifdef DEBUG_MESSAGES_ON
			cout << endl << "\t---- Ejecutando busqueda local entre pares insertando ----" << endl << endl;
		#endif
		mejora_en_w2_bql_entre_pares = busqueda_local_entre_pares_insertando(solucion_actual, conexion_ij_minima_w2_entre_pares);
		#ifdef DEBUG_MESSAGES_ON
			if(mejora_en_w2_bql_entre_pares > 0){
				cout << "\t---- Mejora tentativa sobre w2 en el camino actual aplicando esta mejora: " << mejora_en_w2_bql_entre_pares << endl;				
			}
		#endif
	}

	if(bql_mejorar){
		#ifdef DEBUG_MESSAGES_ON
			cout << endl << "\t---- Ejecutando busqueda local entre triplas reemplazando intermedio ----" << endl << endl;
		#endif
		mejora_en_w2_entre_triplas_reemplazando = busqueda_local_entre_triplas_reemplazando_intermedio(solucion_actual, conexion_ij_minima_w2_entre_triplas);		
		#ifdef DEBUG_MESSAGES_ON
			if(mejora_en_w2_entre_triplas_reemplazando > 0){
				cout << "\t---- Mejora tentativa sobre w2 en el camino actual aplicando esta mejora: " << mejora_en_w2_entre_triplas_reemplazando << endl;				
			}
		#endif
	}

	if(bql_contraer){
		#ifdef DEBUG_MESSAGES_ON
			cout << endl << "\t---- Ejecutando busqueda local entre triplas salteando ----" << endl << endl;
		#endif
		mejora_en_w2_entre_triplas_salteando = busqueda_local_entre_triplas_salteando(solucion_actual, punto_de_salto_it);		
		#ifdef DEBUG_MESSAGES_ON
			if(mejora_en_w2_entre_triplas_salteando > 0){
				cout << "\t---- Mejora tentativa sobre w2 en el camino actual aplicando esta mejora: " << mejora_en_w2_entre_triplas_salteando << endl;				
			}
		#endif
	}

	//si son todas las mejoras 0, no mejoro ninguna nada
	if( (mejora_en_w2_bql_entre_pares == 0) && (mejora_en_w2_entre_triplas_reemplazando == 0) && (mejora_en_w2_entre_triplas_salteando == 0) ){
		return 0;
	}else{
		//existe al menos uno de los 3 que mejoro, es decir que es > 0
		if(mejora_en_w2_bql_entre_pares > mejora_en_w2_entre_triplas_reemplazando){
			//vale mejora_en_w2_bql_entre_pares > mejora_en_w2_entre_triplas_reemplazando
			if(mejora_en_w2_bql_entre_pares > mejora_en_w2_entre_triplas_salteando){
				//max = mejora_en_w2_bql_entre_pares;
				if(mejora_en_w2_bql_entre_pares > 0){
					if(solucion_actual.insertar_nodo(conexion_ij_minima_w2_entre_pares)){
						#ifdef DEBUG_MESSAGES_ON
							cout << endl << "Nueva solucion obtenida: ";
					        solucion_actual.imprimir_camino(cout);
							cout << "Nuevos costos totales del camino:   W1: " << solucion_actual.obtener_costo_total_w1_camino() << "    W2: "  << solucion_actual.obtener_costo_total_w2_camino() << endl;		
						#endif
						this->establecer_camino_solucion(solucion_actual);
						return mejora_en_w2_bql_entre_pares;
					}else{
						return 0;				
					}
				}else{
					return 0;
				}
			}else{
				//max = mejora_en_w2_entre_triplas_salteando;
				if(mejora_en_w2_entre_triplas_salteando > 0){
					nodo_t nodo_i = *punto_de_salto_it;
					if(solucion_actual.realizar_salto_entre_3_nodos(nodo_i)){
						//No puede haber ciclos, porque el camino quedo igual o con menos nodos
						#ifdef DEBUG_MESSAGES_ON
					        cout << endl << "Nueva solucion obtenida: ";
					        solucion_actual.imprimir_camino(cout);
							cout << "Nuevos costos totales del camino:   W1: " << solucion_actual.obtener_costo_total_w1_camino() << "    W2: "  << solucion_actual.obtener_costo_total_w2_camino() << endl;						
						#endif	
						this->establecer_camino_solucion(solucion_actual);
						return mejora_en_w2_entre_triplas_salteando;
					}else{					
						return 0;
					}
				}else{
					return 0;
				}
			}
		}else{
			//vale mejora_en_w2_bql_entre_pares <= mejora_en_w2_entre_triplas_reemplazando
			if(mejora_en_w2_entre_triplas_reemplazando > mejora_en_w2_entre_triplas_salteando){
				//max = mejora_en_w2_entre_triplas_reemplazando;
				if(mejora_en_w2_entre_triplas_reemplazando > 0){
					if(solucion_actual.mejorar_tripla(conexion_ij_minima_w2_entre_triplas)){
						#ifdef DEBUG_MESSAGES_ON
						    cout << endl << "Nueva solucion obtenida: ";
						    solucion_actual.imprimir_camino(cout);
							cout << "Nuevos costos totales del camino:   W1: " << solucion_actual.obtener_costo_total_w1_camino() << "    W2: "  << solucion_actual.obtener_costo_total_w2_camino() << endl;						
						#endif
						this->establecer_camino_solucion(solucion_actual);
						return mejora_en_w2_entre_triplas_reemplazando;
					}else{
						return 0;
					}
				}else{
					return 0;
				}
			}else{
				//max = mejora_en_w2_entre_triplas_salteando;
				if(mejora_en_w2_entre_triplas_salteando > 0){
					nodo_t nodo_i = *punto_de_salto_it;
					if(solucion_actual.realizar_salto_entre_3_nodos(nodo_i)){
						//No puede haber ciclos, porque el camino quedo igual o con menos nodos
						#ifdef DEBUG_MESSAGES_ON
					        cout << endl << "Nueva solucion obtenida: ";
					        solucion_actual.imprimir_camino(cout);
							cout << "Nuevos costos totales del camino:   W1: " << solucion_actual.obtener_costo_total_w1_camino() << "    W2: "  << solucion_actual.obtener_costo_total_w2_camino() << endl;						
						#endif	
						this->establecer_camino_solucion(solucion_actual);
						return mejora_en_w2_entre_triplas_salteando;
					}else{
						return 0;
					}
				}else{
					return 0;
				}
			}
		}
	}
}

list<Grafo> Grafo::parsear_varias_instancias(formato_entrada_t formato){
	list<Grafo> instancias;
    //parseo todas las instancias
    bool instancia_valida = true;
    do{
        Grafo i(0);
        instancia_valida = i.unserialize(cin, formato);
        if(instancia_valida)
        	instancias.push_back(i);
    }while(instancia_valida);
    //cout << "[Parse input]Se leyeron " << instancias.size() << " instancias de stdin" << endl << endl;
    return instancias;
}

void Grafo::establecer_se_encontro_solucion(bool se_encontro){
	this->sol_valida = se_encontro;
}

bool Grafo::hay_solucion(){
	return this->sol_valida;
}

Camino Grafo::obtener_solucion_golosa(){
	int n = this->cantidad_nodos;
	int k = obtener_limite_w1();
	nodo_t origen = obtener_nodo_origen();
	nodo_t destino = obtener_nodo_destino();

	//camino a devolver
	Camino c(this->mat_adyacencia);

	//dijkstra de inicializacion
	vector<costo_t> costosw1;
	vector<nodo_t> predecesores;
	this->dijkstra(destino, COSTO_W1, costosw1, predecesores);

	//ACA YA PUEDO SABER SI NO VA A HABER SOLUCION FACTIBLE CON EL DIJKSTRA PREPROCESADO
	if(costosw1[origen] == costo_infinito){//dist_w1(origen, destino) == infinito?
        cerr << "No existe solucion factible. No existe camino entre origen(" << origen << ") y destino(" << destino << ") " << endl;
		//ESTO HACE SI LE PONES FALSE, QUE GRAFO IMPRIMA "no" EN EL METODO SERIALIZE DE LA SALIDA.
		//ADEMAS DE SER UTIL PARA PREGUNTAR DESDE EL MAIN SI HUBO SOL.
        this->establecer_se_encontro_solucion(false);
    }else if(costosw1[origen] > k){//dist_w1(origen, destino) > limit_w1 = k?
        cerr << "No existe solucion factible. El camino minimo respecto a w1 de origen(" << origen << ") a destino(" << destino << ") es de costo " << costosw1[origen] << endl;
        //ESTO HACE SI LE PONES FALSE, QUE GRAFO IMPRIMA "no" EN EL METODO SERIALIZE DE LA SALIDA.
		//ADEMAS DE SER UTIL PARA PREGUNTAR DESDE EL MAIN SI HUBO SOL.
        this->establecer_se_encontro_solucion(false);
    }else{
    	//------------------------------------- Comienza Greedy ----------------------------------------------------

		//Estructuras del greedy
		vector<costo_t> costosw2(n, costo_infinito);
		// contiene el costo w1 del camino recorrido hasta cada nodo
		vector<costo_t> costoCamino(n, costo_nulo); 

		//Comienza el algoritmo
		//reseteamos los predecesores
		predecesores.clear();
		predecesores.resize(n, predecesor_nulo);

		costosw2[origen] = costo_nulo;

		set<pair<costo_t, nodo_t> > cola;
		cola.insert(make_pair(costosw2[origen], origen));

		while(!cola.empty()){
			pair<costo_t, nodo_t> actual = *cola.begin();
			cola.erase(cola.begin());

			lista_adyacentes vecinos = this->obtener_lista_vecinos(actual.second);

			for(auto w : vecinos)
			{
				nodo_t nodoW = w.first;
				Arista aristaActualW = w.second;

				if (costoCamino[actual.second] + costosw1[nodoW] + aristaActualW.obtener_costo_w1() <= k){
					costo_t costo_tentativo = costosw2[actual.second] + aristaActualW.obtener_costo_w2();
					if (costosw2[nodoW] > costo_tentativo)
					{
						cola.erase(make_pair(costosw2[nodoW], nodoW));

						costosw2[nodoW] = costo_tentativo;
						costoCamino[nodoW] = costoCamino[actual.second] + aristaActualW.obtener_costo_w1();
						predecesores[nodoW] = actual.second;
						
						cola.insert(make_pair(costosw2[nodoW], nodoW));
					} 	
				}
			}
		}

		//
		//{
			//
			//armar camino encontrado por la greedy
			nodo_t nodo = destino;
			//cout <<"Nodos" << endl;
			do{
				//cout << nodo << " " ;
				c.agregar_nodo_adelante(nodo);
				nodo = predecesores[nodo];
			}while(nodo != predecesor_nulo);

			//cout << endl << "Fin Nodos" << endl;
			this->establecer_se_encontro_solucion(true);
		//}
		//------------------------------------- Fin Greedy ----------------------------------------------------
	}
	return c;
}

//parametro beta en RCL_POR_VALOR indica el porcentaje a alejarse del mejor candidato
//parametro beta en RCL_POR_CANTIDAD indica la candidad de "mejores" candidatos a elegir
set<pair<costo_t, nodo_t> >::iterator Grafo::obtener_candidato_randomizado(tipo_ejecucion_golosa_t tipo_ejecucion, const set<pair<costo_t, nodo_t> > & cola, double parametro_beta){
	set<pair<costo_t, nodo_t> >::iterator retorno;
	if(tipo_ejecucion == RCL_DETERMINISTICO){
		retorno = cola.begin();
	}else if(tipo_ejecucion == RCL_POR_CANTIDAD){		
		//tengo que elegir un numero i, aleatorio entre 0 y min{cola.size(), parametro_beta} -1 elemento de la cola
		//y devolver el i-esimo elemento en orden de la cola
		//dado que la cola esta ordenada, cumple una RCL_POR_CANTIDAD
		uint rcl_target_top = (uint) std::min((uint)cola.size(), (uint) parametro_beta) - 1;

		//generacion numero random c++11 con distribucion uniforme
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dis(0, rcl_target_top);
		uint rcl_target_random = dis(gen); // generates number in the range 0..rcl_target_top

		//cout << "cola size: " << cola.size() << endl;
		//cout << "parametro_beta: " << parametro_beta << endl;
		//cout << "rango random [0.." << rcl_target_top << "]" << endl;
		//cout << "tomando random: " << rcl_target_random << endl;
		//for(int i=0; i<22;i++){
		//	cout << dis(gen) << endl;
		//}

		//sabemos que  esta en rango del iterador pues rcl_target_top <= cola.size() -1 y tomamos begin() que es el primero, es decir 0
		retorno = cola.begin();
		uint avance = 0;
		assert(avance >= 0);
		assert(avance < cola.size());
		while(avance < rcl_target_random){
			retorno ++;
			avance++;
		}
		if(avance == 0){
			assert(retorno == cola.begin());
		}
	}else if (tipo_ejecucion == RCL_POR_VALOR){
		//itero sobre toda la cola, filtrando los elementos que esten dentro del porcentaje del parametro
		//luego selecciono uno al azar del vector de candidatos filtrados
		pair<costo_t, nodo_t> minimo = *cola.begin();
		double valor_limite = (parametro_beta + 1) * minimo.first;
		vector<set<pair<costo_t, nodo_t> >::iterator > candidatos;
		
		set<pair<costo_t, nodo_t> >::iterator it_cola = cola.begin();
		set<pair<costo_t, nodo_t> >::iterator it_cola_fin = cola.end();

		while(it_cola != it_cola_fin){
			if(it_cola->first <= valor_limite){
				candidatos.push_back(it_cola);
			}
			it_cola++;
		}
		
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dis(0, candidatos.size() -1);
		uint rcl_target_random = dis(gen); // generates number in the range 0..candidatos.size() -1
		retorno = candidatos[rcl_target_random];
	}else{
		cerr << "[Error] Parametro no soportado de randomizacion de RCL. Asumiendo RCL_DETERMINISTICO " << tipo_ejecucion << endl;
		retorno = cola.begin();
	}
	return retorno;
}

costo_t Grafo::obtener_costo_actual_w1_solucion(){
	return this->camino_obtenido.obtener_costo_total_w1_camino();
}

costo_t Grafo::obtener_costo_actual_w2_solucion(){
	return this->camino_obtenido.obtener_costo_total_w2_camino();
}

//typedef enum tipo_ejecucion_golosa_t {RCL_DETERMINISTICO, RCL_POR_VALOR, RCL_POR_CANTIDAD} tipo_ejecucion_golosa_t;
Camino Grafo::obtener_solucion_golosa_randomizada(tipo_ejecucion_golosa_t tipo_ejecucion, double parametro_beta){
	int n = this->cantidad_nodos;
	int k = obtener_limite_w1();
	nodo_t origen = obtener_nodo_origen();
	nodo_t destino = obtener_nodo_destino();

	//camino a devolver
	Camino c(this->mat_adyacencia);

	vector<bool> visitados;

	for(int x = 0; x<n;x++)
		visitados.push_back(false);

	//dijkstra de inicializacion
	vector<costo_t> costosw1;
	vector<nodo_t> predecesores;
	this->dijkstra(destino, COSTO_W1, costosw1, predecesores);

	//ACA YA PUEDO SABER SI NO VA A HABER SOLUCION FACTIBLE CON EL DIJKSTRA PREPROCESADO
	if(costosw1[origen] == costo_infinito){//dist_w1(origen, destino) == infinito?
        cerr << "No existe solucion factible. No existe camino entre origen(" << origen << ") y destino(" << destino << ") " << endl;
		//ESTO HACE SI LE PONES FALSE, QUE GRAFO IMPRIMA "no" EN EL METODO SERIALIZE DE LA SALIDA.
		//ADEMAS DE SER UTIL PARA PREGUNTAR DESDE EL MAIN SI HUBO SOL.
        this->establecer_se_encontro_solucion(false);
    }else if(costosw1[origen] > k){//dist_w1(origen, destino) > limit_w1 = k?
        cerr << "No existe solucion factible. El camino minimo respecto a w1 de origen(" << origen << ") a destino(" << destino << ") es de costo " << costosw1[origen] << endl;
        //ESTO HACE SI LE PONES FALSE, QUE GRAFO IMPRIMA "no" EN EL METODO SERIALIZE DE LA SALIDA.
		//ADEMAS DE SER UTIL PARA PREGUNTAR DESDE EL MAIN SI HUBO SOL.
        this->establecer_se_encontro_solucion(false);
    }else{
    	//------------------------------------- Comienza Greedy randomized----------------------------------------------------
		//Estructuras del greedy
		vector<costo_t> costosw2(n, costo_infinito);
		// contiene el costo w1 del camino recorrido hasta cada nodo
		vector<costo_t> costoCamino(n, costo_nulo);

		//Comienza el algoritmo
		//reseteamos los predecesores
		predecesores.clear();
		predecesores.resize(n, predecesor_nulo);

		costosw2[origen] = costo_nulo;

		visitados[origen] = true;

		set<pair<costo_t, nodo_t> > cola;
		cola.insert(make_pair(costosw2[origen], origen));

		while(!cola.empty()){
			//Seleccion de decision greedy segun tipo de ejecucion requerido por parametro
			set<pair<costo_t, nodo_t> >::iterator it_candidato = obtener_candidato_randomizado(tipo_ejecucion, cola, parametro_beta);

			pair<costo_t, nodo_t> actual = *it_candidato;
			visitados[actual.second] = true;
			cola.erase(it_candidato);

			lista_adyacentes vecinos = this->obtener_lista_vecinos(actual.second);

			for(auto w : vecinos)
			{
				nodo_t nodoW = w.first;
				Arista aristaActualW = w.second;

				if ((costoCamino[actual.second] + costosw1[nodoW] + aristaActualW.obtener_costo_w1() <= k) && !(visitados[nodoW])){
					costo_t costo_tentativo = costosw2[actual.second] + aristaActualW.obtener_costo_w2();
					if (costosw2[nodoW] > costo_tentativo)
					{
						cola.erase(make_pair(costosw2[nodoW], nodoW));

						costosw2[nodoW] = costo_tentativo;
						costoCamino[nodoW] = costoCamino[actual.second] + aristaActualW.obtener_costo_w1();
						predecesores[nodoW] = actual.second;
						
						cola.insert(make_pair(costosw2[nodoW], nodoW));
					} 	
				}
			}
		}

		//if(se_encontro_sol?) suponiendo que si hay sol factible el algoritmo encuentra una solucion, esto no hace falta
		//{
			//
			//armar camino encontrado por la greedy
			nodo_t nodo = destino;
			//cout <<"Nodos" << endl;
			do{
				//cout << nodo << " " ;
				c.agregar_nodo_adelante(nodo);
				nodo = predecesores[nodo];
			}while(nodo != predecesor_nulo);

			//assert replaced
			//assert(c.obtener_costo_total_w1_camino() == costoCamino[destino]);
			if(c.obtener_costo_total_w1_camino() != costoCamino[destino]){
				//c.imprimir_camino(cout);
				//cout << "Camino invalido" << endl;
				this->establecer_se_encontro_solucion(false);//no es sol factible		
			}

			//cout << endl << "Fin Nodos" << endl;
			this->establecer_se_encontro_solucion(true);
		//}
		//------------------------------------- Fin Greedy randomized----------------------------------------------------
	}
	return c;
}