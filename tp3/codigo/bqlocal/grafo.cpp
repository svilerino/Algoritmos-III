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

Camino::Camino(){
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
		nodo_t penultimo_nodo = camino.front();
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
		cerr << "Costo infinito(no son adyacentes) entre nodos (" << i << ") y (" << j << ")" << endl;
		return costo_infinito;
	}	
}

//pre: 0 <= i <= j < cantidad_nodos y que i,j sean adyacentes
costo_t Camino::obtener_costo_w2_entre_nodos(nodo_t i, nodo_t j){
	if(this->mat_adyacencia[i][j].esta_presente()){
		return this->mat_adyacencia[i][j].obtener_costo_w2();
	}else{
		cerr << "Costo infinito(no son adyacentes) entre nodos (" << i << ") y (" << j << ")" << endl;
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

list<nodo_t>::const_iterator Camino::obtener_iterador_begin(){
	return this->camino.begin();
}

list<nodo_t>::const_iterator Camino::obtener_iterador_end(){
	return this->camino.end();
}

longuitud_t Camino::obtener_longuitud_camino(){
	return this->camino.size();
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
	//this->desde_i_a_comun = desde_i;
	//this->desde_j_a_comun = desde_j;
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

costo_t Grafo::obtener_limite_w1(){
	return this->cota_w1;
}

Camino& Grafo::obtener_camino_solucion(){
	return this->camino_obtenido;
}

void Grafo::serialize(ostream& out){
	out << this->camino_obtenido.obtener_costo_total_w1_camino() << " ";
	out << this->camino_obtenido.obtener_costo_total_w2_camino() << " ";
	out << this->camino_obtenido.obtener_longuitud_camino() << " ";
	list<nodo_t>::const_iterator it = this->camino_obtenido.obtener_iterador_begin();
	while(it != camino_obtenido.obtener_iterador_end()){
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
Camino& Grafo::dijkstra(nodo_t origen, nodo_t destino, tipo_costo_t target_a_minimizar){
	vector<costo_t> costo_minimo;
	vector<nodo_t> predecesor;
	return this->dijkstra(origen, destino, target_a_minimizar, costo_minimo, predecesor);
}

//Devuelve el camino minimo entre origen y destino y quedan modificados por referencia los costos_minimos y los predecesores de todos los nodos de G
Camino& Grafo::dijkstra(nodo_t origen, nodo_t destino, tipo_costo_t target_a_minimizar, vector<costo_t>& costo_minimo, vector<nodo_t>& predecesor){
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
		//cout << nodo << " " ;
		c.agregar_nodo_adelante(nodo);
		nodo = predecesor[nodo];
	}while(nodo != predecesor_nulo);
	//cout << origen << endl;	
	this->camino_obtenido = c;
	return this->camino_obtenido;
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

nodo_t Grafo::obtener_nodo_origen(){
	return this->nodo_src;
}

nodo_t Grafo::obtener_nodo_destino(){
	return this->nodo_dst;
}

bool Grafo::mejorar_conexion_entre_pares(nodo_t nodo_i, nodo_t nodo_j, costo_t costo_ij_w1, costo_t costo_ij_w2, costo_t total_w1, costo_t total_w2, Vecino& mejor_vecino){	
    cout << "Buscando mejorar la conexion (" << nodo_i << ")----[" << costo_ij_w1 << ", " << costo_ij_w2 << "]---->(" << nodo_j << ") agregando un nodo intermedio..." << endl;
    //busco alguna conexion de 2 aristas entre vecinos en comun tal que la suma de esas 2 aristas
	//sea menor al peso de la arista directa
 	list<Vecino> vecinosEnComun = this->obtener_adyacentes_en_comun(nodo_i, nodo_j);
	list<Vecino>::iterator vecinos_it = vecinosEnComun.begin();
	list<Vecino>::iterator final_vecinos = vecinosEnComun.end();
	
    //me fijo todos los caminos alternativos agregando un nodo entre los nodos ij,
    //me quedo con la mejor y despues del while, si hay mejora, la aplico al camino.
    list<Vecino>::iterator mejor_vecino_it = vecinosEnComun.end();//inicializamos en algo que indique que no hay mejora
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
		if(hipotetico_w1_total_camino < this->obtener_limite_w1()){
			//es factible, veamos si mejora al ultimo mejor revisado
			if(costo_i_comun_j_w2 < mejor_camino_ij_w2){
				//encontre mejora!
				//actualizo variables
				mejor_camino_ij_w2 = hipotetico_w2_total_camino;
				mejor_vecino_it = vecinos_it;
			}
		}//else{
			//si no lo es, ignoramos esta modificacion al sendero
			//cout << "\tDescartando este cambio. No ofrece mejora sobre w2 o se pasa de la cota de w1" << endl;
		//}
		++vecinos_it;
	}
	if(mejor_vecino_it != final_vecinos){
		mejor_vecino = *mejor_vecino_it;		
		return true;
	}
	return false;
 }

void Grafo::busqueda_local_entre_pares_insertando(){
	//caso donde reemplazo un camino vk---->vk+1 por otro vk---->vj---->vk+1 tal que mejora w2 y w1 no se pasa en el costo total del camino
	list<nodo_t>::const_iterator it = this->camino_obtenido.obtener_iterador_begin();
	list<nodo_t>::const_iterator runner_it = this->camino_obtenido.obtener_iterador_begin();
	runner_it++;
	list<nodo_t>::const_iterator final_camino = this->camino_obtenido.obtener_iterador_end();
	costo_t total_w1 = this->camino_obtenido.obtener_costo_total_w1_camino();
	costo_t total_w2 = this->camino_obtenido.obtener_costo_total_w2_camino();

    cout << "Costo total del camino:   W1: " << total_w1 << "    W2: " << total_w2 << endl << endl;
	while(runner_it != final_camino){
        nodo_t nodo_i = *it;
        nodo_t nodo_j = *runner_it;
        costo_t costo_ij_w1 = this->camino_obtenido.obtener_costo_w1_entre_nodos(nodo_i, nodo_j);
    	costo_t costo_ij_w2 = this->camino_obtenido.obtener_costo_w2_entre_nodos(nodo_i, nodo_j);

    	Vecino mejor_conexion_ij;
    	//le paso una ref a una var tipo vecino, si devuelve true, se escribe por referencia el mejor camino, sino, no cambia lo que le pasamos.
        bool encontre_mejora = mejorar_conexion_entre_pares(nodo_i, nodo_j,
        										 costo_ij_w1, costo_ij_w2,
        										 total_w1, total_w2,
        										 mejor_conexion_ij);

		//hay que ver si encontramos una mejora
		if(encontre_mejora){//la funcion asegura que si dio true, me da el vecino por puntero en mejor_conexion_ij
            nodo_t nodo_comun = mejor_conexion_ij.obtener_nodo_comun();
            costo_t i_comun_w1 = mejor_conexion_ij.obtener_arista_i_comun().obtener_costo_w1();
            costo_t i_comun_w2 = mejor_conexion_ij.obtener_arista_i_comun().obtener_costo_w2();
            costo_t j_comun_w1 = mejor_conexion_ij.obtener_arista_j_comun().obtener_costo_w1();
            costo_t j_comun_w2 = mejor_conexion_ij.obtener_arista_j_comun().obtener_costo_w2();
            costo_t costo_i_comun_j_w1 = i_comun_w1 + j_comun_w1;
            costo_t costo_i_comun_j_w2 = i_comun_w2 + j_comun_w2;
			cout << "\tSe encontro una posible mejora. Ahora el sendero entre los nodos (" << nodo_i << ") y (" << nodo_j << ") es " << endl;
            cout << "\tCamino (" << nodo_i << ")----[" << i_comun_w1 << ", " << i_comun_w2 <<  "]---->(" << nodo_comun << ")----[" << j_comun_w1 << ", " << j_comun_w2 << "]---->(" << nodo_j;
            cout <<	") Nuevo costo del sendero entre (" << nodo_i << ") y (" << nodo_j << ") aplicando a esta modificacion:    W1: " << costo_i_comun_j_w1 << "     W2: " << costo_i_comun_j_w2 << endl;

			costo_t hipotetico_w1_total_camino = (total_w1 - costo_ij_w1 + costo_i_comun_j_w1);
			costo_t hipotetico_w2_total_camino = (total_w2 - costo_ij_w2 + costo_i_comun_j_w2);

			cout << "\tSi aplicamos este cambio los costos del camino total quedarian:   W1: " << hipotetico_w1_total_camino << "    W2: "  << hipotetico_w2_total_camino << endl;
		}else{
			cout << "\tNo se encontro mejora." << endl;
		}
        cout << endl;
		++it;
		++runner_it;
	}
	cout << "Actualmente se iteran todos los pares (it, runner_it) y se verifica si mejorarian la solucion actual" << endl;
	cout << "Todo: Buscar la maxima mejora entre todos los pares(it, runner_it), esta es la mejor solucion de la vecindad de los caminos que difieren en un nodo" << endl;
	cout << "Modificar el camino actual segun la maxima mejora de la vecindad y actualizar las estructuras de la solucion obtenida en la clase Grafo/Camino/etc" << endl;
}


void busqueda_local_entre_triplas_salteando(){
	//aca van el caso en los que salteo un nodo vk---->vk+1---->vk+2 convirtiendolo en vk---->vk+2 tal que mejora w2 y w1 no se pasa en el costo total del camino
}

void busqueda_local_entre_triplas_reemplazando_intermedio(){
	//y el caso en que reemplazo vk+1 por otro vecino comun vj, convirtiendo vk---->vk+1---->vk+2 en vk---->vj---->vk+2 tal que mejora w2 y w1 no se pasa en el costo total del camino
}