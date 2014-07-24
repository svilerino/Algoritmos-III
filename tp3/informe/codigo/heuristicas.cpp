//#define DEBUG_MESSAGES_ON
//#define CYCLE_PREVENT_MESSAGE_ON
//#define VECINOS_COMUNES_LAZY

using namespace std;

typedef int nodo_t;
typedef int longuitud_t;
typedef double distancia_t;
typedef double costo_t;
typedef enum tipo_costo_t {COSTO_W1, COSTO_W2} tipo_costo_t;
typedef enum tipo_ejecucion_golosa_t {RCL_DETERMINISTICO, RCL_POR_VALOR, RCL_POR_CANTIDAD} tipo_ejecucion_golosa_t;
typedef enum tipo_ejecucion_bqlocal_t {BQL_SUBDIVIDIR_PARES, BQL_CONTRAER_TRIPLAS_A_PARES, BQL_MEJORAR_CONEXION_TRIPLAS, BQL_COMBINAR} tipo_ejecucion_bqlocal_t;
typedef enum criterio_terminacion_grasp_t {CRT_K_ITERS_SIN_MEJORA, CRT_K_ITERS_LIMIT_REACHED} criterio_terminacion_grasp_t;
typedef enum formato_entrada_t {FORMATO_0_N_OPEN, FORMATO_1_N_CLOSED} formato_entrada_t;
typedef formato_entrada_t formato_salida_t;

const costo_t costo_infinito = numeric_limits<double>::infinity();
const distancia_t distancia_infinita = numeric_limits<double>::infinity();
const costo_t costo_nulo = 0;
const nodo_t predecesor_nulo = -1;

class Arista{
private:
	bool presente;
	costo_t costo_w1;
	costo_t costo_w2;
public:
	Arista();
	Arista(bool pres, costo_t w1, costo_t w2);
	~Arista();

	bool esta_presente();
	void desmarcar_presente();
	void marcar_presente(costo_t w1, costo_t w2);
	costo_t obtener_costo_w1() const;
	costo_t obtener_costo_w2() const;

	bool operator== (const Arista &other) const
	{
	    return (other.presente == this->presente &&
	            other.costo_w1 == this->costo_w1 &&
	            other.costo_w2 == this->costo_w2);
	}
};

typedef vector<vector<Arista> > matriz_adyacencia_t;
typedef list<pair<nodo_t, Arista> > lista_adyacentes;
typedef vector<lista_adyacentes> lista_adyacencia_t;

class Vecino{
private:
	nodo_t i;
	nodo_t j;
	nodo_t en_comun;
	Arista desde_i_a_comun;
	Arista desde_j_a_comun;
public:
	Vecino(nodo_t i, nodo_t j, nodo_t comun, Arista desde_i, Arista desde_j);
	Vecino();
	~Vecino();
	nodo_t obtener_nodo_i();
	nodo_t obtener_nodo_j();
	nodo_t obtener_nodo_comun();
	Arista obtener_arista_i_comun();
	Arista obtener_arista_j_comun();
};

typedef vector<vector<vector<Vecino> > > vecinos_comunes_t;

class Camino{
private:
	list<nodo_t> camino;
	vector<bool> esta_en_camino;
	costo_t costo_camino_w1;
	costo_t costo_camino_w2;
	matriz_adyacencia_t mat_adyacencia;
public:
	//Camino();
	Camino(matriz_adyacencia_t& mat_adyacencia);	
	~Camino();

	void agregar_nodo(nodo_t target);
	void agregar_nodo_adelante(nodo_t target);
	costo_t obtener_costo_total_w1_camino();
	costo_t obtener_costo_total_w2_camino();
	//pre: 0 <= i <= j < cantidad_nodos y que i,j sean adyacentes
	costo_t obtener_costo_w1_entre_nodos(nodo_t i, nodo_t j);
	//pre: 0 <= i <= j < cantidad_nodos y que i,j sean adyacentes
	costo_t obtener_costo_w2_entre_nodos(nodo_t i, nodo_t j);

	void imprimir_camino(ostream& out);

	list<nodo_t>::iterator obtener_iterador_begin();
	list<nodo_t>::iterator obtener_iterador_end();
	list<nodo_t>::const_iterator obtener_iterador_const_begin();
	list<nodo_t>::const_iterator obtener_iterador_const_end();
	longuitud_t obtener_longuitud_camino();

	//pre: at.obtener_nodo_i() y at.obtener_nodo_i() deben pertenecer al camino
	//Se reemplazara la conexion directa entre i y j por i -> encomun -> j indicado por el Vecino pasado
	//por parametro. Devuelve true si se inserto, false sino.
	bool insertar_nodo(Vecino& at);

	//pre: at.obtener_nodo_i() y at.obtener_nodo_i() deben pertenecer al camino
	//Se reemplazara la conexion i..loquesea..j por i -> encomun -> j indicado por el Vecino pasado
	//por parametro. Devuelve true si se inserto, false sino.
	bool mejorar_tripla(Vecino& at);

	bool realizar_salto_entre_3_nodos(nodo_t punto_de_salto);
	bool pertenece_a_camino(nodo_t target);//O(1)
};

class Grafo{
private:
	//atributos
	matriz_adyacencia_t mat_adyacencia;
	vecinos_comunes_t vecinos_comunes;
	lista_adyacencia_t lista_adyacencia;
	int cantidad_nodos;
	int cantidad_aristas;
	
	//atributos propios del problema
	nodo_t nodo_src;
	nodo_t nodo_dst;
	costo_t cota_w1;
	Camino camino_obtenido;
	bool sol_valida;
	//dado el camino, podemos obtener los pesos de cada "salto" indexando en la matriz de adyacencia el costo de cada salto
	//tanto para w1 como w2

	//metodos auxiliares
	bool mejorar_conexion_entre_pares(nodo_t nodo_i, nodo_t nodo_j, costo_t costo_ij_w1, costo_t costo_ij_w2, costo_t total_w1, costo_t total_w2,
	 Vecino& mejor_vecino);

	bool mejorar_conexion_salteando(nodo_t nodo_i, nodo_t nodo_j, costo_t costo_ij_w1, costo_t costo_ij_w2, costo_t total_w1, costo_t total_w2,
	 Arista& mejor_vecino);

	void precalcular_adyacentes_en_comun(nodo_t i, nodo_t j);

	//Grasp
	set<pair<costo_t, nodo_t> >::iterator obtener_candidato_randomizado(tipo_ejecucion_golosa_t tipo_ejecucion, const set<pair<costo_t, nodo_t> > & cola, double parametro_beta);

	int busqueda_local_entre_pares_insertando(Camino& solucion_actual, Vecino& conexion_ij_minima_w2);
	int busqueda_local_entre_triplas_reemplazando_intermedio(Camino& solucion_actual, Vecino& conexion_ij_minima_w2);
	int busqueda_local_entre_triplas_salteando(Camino& solucion_actual, list<nodo_t>::const_iterator& punto_de_salto_it);
public:
	//constructor y destructor
	Grafo(int cant_inicial_nodos);
	~Grafo();
	
	//Modificadores
	void agregar_nodos(int cantidad_nodos);
	void agregar_arista(nodo_t i, nodo_t j, costo_t w1, costo_t w2);

	//Consultas
	vector<Arista> obtener_vector_fila_vecinos(nodo_t target);
	lista_adyacentes obtener_lista_vecinos(nodo_t target);
	int obtener_cantidad_nodos();
	int obtener_cantidad_aristas();
	Arista obtener_arista(nodo_t i, nodo_t j);	
	//pre: 0 <= i <= j < cantidad_nodos y que i,j sean adyacentes
	vector<Vecino> obtener_adyacentes_en_comun(nodo_t i, nodo_t j);
	nodo_t obtener_nodo_origen();
	nodo_t obtener_nodo_destino();
	costo_t obtener_limite_w1();
	costo_t obtener_costo_actual_w1_solucion();
	costo_t obtener_costo_actual_w2_solucion();
	Camino obtener_camino_solucion();
	void establecer_camino_solucion(Camino c);

	//Entrada - Salida
	void imprimir_matriz_adyacencia(ostream& out);
	void imprimir_lista_adyacencia(ostream& out);
	void serialize(ostream& out, formato_salida_t formato);
	bool unserialize(istream& in, formato_entrada_t formato);

	//Algoritmos
	//Realiza la busqueda local sobre una solucion inicial factible creada por dijkstra sobre COSTO_W1 entre src y dst
	int busqueda_local(tipo_ejecucion_bqlocal_t tipo_ejecucion);
	//Devuelve el camino minimo entre origen y destino(calcula el arbol, pero reconstruye solo el camino de origen a destino)
	Camino dijkstra(nodo_t origen, nodo_t destino, tipo_costo_t target_a_minimizar);
	//Aplica dijkstra desde nodo origen y calcula el arbol de caminos minimos por referencia a los vectores por parametro
	void dijkstra(nodo_t origen, tipo_costo_t target_a_minimizar, vector<costo_t>& costo_minimo, vector<nodo_t>& predecesor);
	//Dado un nodo_t origen se calcula para cada nodo, la distancia minima en cantidad de aristas de peso constante 1 de cualquier nodo a origen
	void breadth_first_search(nodo_t origen, vector<distancia_t>& distancias_en_aristas_a_origen);

	//Golosa
	Camino obtener_solucion_golosa();
	Camino obtener_solucion_golosa_randomizada(tipo_ejecucion_golosa_t tipo_ejecucion, double parametro_beta);

	//Metodos utilitarios
	static list<Grafo> parsear_varias_instancias(formato_entrada_t formato);
	void establecer_se_encontro_solucion(bool se_encontro);
	bool hay_solucion();
	Camino obtener_camino_vacio();
};

//-----------------------------------------------------------------------------------------------------------------------


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
		//random_device rd;
		//mt19937 gen(rd());
		//uniform_int_distribution<> dis(0, rcl_target_top);
		//uint rcl_target_random = dis(gen); // generates number in the range 0..rcl_target_top

		//lo cambio por esto de C-legacy porque lo de c++11 me aumenta violentamente el tiempo de ejecucion
		srand(time(NULL));
		uint rcl_target_random = 0;
		if(rcl_target_top != 0){
			rcl_target_random = (unsigned int) rand() % rcl_target_top;			
		}

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
		
		//random_device rd;
		//mt19937 gen(rd());
		//uniform_int_distribution<> dis(0, candidatos.size() -1);
		//uint rcl_target_random = dis(gen); // generates number in the range 0..candidatos.size() -1

		//lo cambio por esto de C-legacy porque lo de c++11 me aumenta violentamente el tiempo de ejecucion
		uint rcl_target_top = candidatos.size() -1;
		srand(time(NULL));
		uint rcl_target_random = 0;
		if(rcl_target_top > 0){
			rcl_target_random = (unsigned int) rand() % rcl_target_top;			
		}

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

	vector<bool> visitados(n, false);

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
			cola.erase(it_candidato);
			visitados[actual.second] = true;

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

//-----------------------------------------------------------------------------------------------------------------------

#include "grafo.h"
#include "timing.h"
#include <fstream>
#include <cmath>

#define FILE_ITERS_MEJORA "evolucion_iteraciones_grasp.txt"
#define FILE_ITERS_COSTOS_ABSOLUTOS "costos_absolutos_iteraciones_grasp.txt"
#define FILE_ITERS_COSTOS_ABSOLUTOS_STATISTICS "costos_absolutos_iteraciones_grasp_analisis.txt"

void ejecutar_grasp(Grafo &g);

// -------------- Main ---------------------------------
int main(int argc, char **argv){
    list<Grafo> instancias = Grafo::parsear_varias_instancias(FORMATO_1_N_CLOSED);
    uint64_t instance_number = 1;
    for(Grafo &g : instancias){
        #ifdef DEBUG_MESSAGES_ON
            cout << endl << endl << "Aplicando metaheuristica GRASP a la " << instance_number << "-esima instancia de input..." << endl;
        #endif
        ejecutar_grasp(g);
        instance_number++;
    }
    return 0;
}

void ejecutar_grasp(Grafo &g){
    #ifdef DEBUG_MESSAGES_ON
        //g.imprimir_lista_adyacencia(cout);
        //g.imprimir_matriz_adyacencia(cout);
    #endif

//----- Configuracion del criterio de terminacion de GRASP -----

    //typedef enum criterio_terminacion_grasp_t {CRT_K_ITERS_SIN_MEJORA, CRT_K_ITERS_LIMIT_REACHED} criterio_terminacion_grasp_t;
    criterio_terminacion_grasp_t criterio_terminacion = CRT_K_ITERS_SIN_MEJORA;
    //este parametro denota la cantidad de iteraciones maxima, dependiendo del tipo de criterio, de cantidad fija de iteraciones o cantidad de iters
    //consecutivas sin mejora
    uint64_t ITERS_LIMIT = 5;
    //consecutivas sin que greedy rand me de una solucion factible
    uint64_t RAND_GREEDY_BAD_ITERS_LIMIT = 3;
    //este parametro denota el valor aceptable de la funcion objetivo w2 a partir del cual, dejamos de mejorar la solucion y consideramos que es lo suficientemente buena

//----- Configuracion de los modos de la busqueda local y golosa -----

    //typedef enum tipo_ejecucion_bqlocal_t {BQL_SUBDIVIDIR_PARES, BQL_CONTRAER_TRIPLAS_A_PARES, BQL_MEJORAR_CONEXION_TRIPLAS, BQL_COMBINAR} tipo_ejecucion_bqlocal_t;
    tipo_ejecucion_bqlocal_t modo_busqueda_local = BQL_COMBINAR;
    //typedef enum tipo_ejecucion_golosa_t {RCL_DETERMINISTICO, RCL_POR_VALOR, RCL_POR_CANTIDAD} tipo_ejecucion_golosa_t;
    tipo_ejecucion_golosa_t modo_golosa = RCL_POR_CANTIDAD;
    //si el tipo de golosa es RCL_POR_VALOR, este parametro indica el porcentaje de alejamiento del minimo de los candidatos de la lista
    //mas formalmente filtra todos los candidatos factibles locales que no cumplan candidato->costo_w2 <= valor_limite
    //donde valor limite es  (parametro_beta + 1) * minimo.second.obtener_costo_w2();
    //si el tipo de golosa es RCL_POR_CANTIDAD, este parametro indica la cantidad min{cant_candidatos, parametro_beta} de soluciones a considerar en la lista
    //si el tipo es RCL_DETERMINISTICO, este parametro es ignorado por el metodo.    
    double parametro_beta = ceil(g.obtener_cantidad_nodos()/(float)1);
    if(modo_golosa == RCL_POR_CANTIDAD){
        assert(parametro_beta >= 1);        
    }else if(modo_golosa == RCL_POR_VALOR){
        assert(parametro_beta > 0);
    }

    //-------------------------------------------------------

    bool condicion_terminacion = false;
    Camino mejor_solucion = g.obtener_camino_vacio();
    costo_t costo_mejor_solucion = costo_infinito;
    uint64_t cant_iters = 0;
    uint64_t cant_iters_sin_sol_greedy_rand_factible = 0;
    uint64_t cant_iters_sin_mejora = 0;
    double tiempo_golosa_randomized = 0;
    double promedio = 0;
    Camino camino = g.obtener_camino_vacio();
    bool sol_valida_greedy = false;
    vector<pair<uint, costo_t> > mejora_iters_grasp;
    vector<pair<costo_t,costo_t> > costo_camino_en_iteraciones;//costos w1, w2

    do{
        tiempo_golosa_randomized=0;
         MEDIR_TIEMPO_PROMEDIO(
            camino = g.obtener_solucion_golosa_randomizada(modo_golosa, parametro_beta);                
            //cout << "Solucion inicial de la greedy:" << endl;
            //camino.imprimir_camino(cout);
        , 1, &tiempo_golosa_randomized);

        //la sol greedy rand a veces da cosas no factibles, asi que verifico:
        //g.hay_solucion() nos indica si existe una sol factible(greedy rand lo setea en false si el minimo dijktra sobre w1 > limit_w1)
        //(camino.obtener_costo_total_w1_camino() <= g.obtener_limite_w1()); para chequear la validez del camino final de greedy rand
        sol_valida_greedy = (camino.obtener_costo_total_w1_camino() <= g.obtener_limite_w1());
        if(g.hay_solucion()){
            if(sol_valida_greedy){//puede que la greedy randomized no encuentre solucion!
                //hago iteraciones de busqueda local hasta que no haya mejora(la funcion devuelve true si hubo mejora, false sino)   
                g.establecer_camino_solucion(camino);

                //reseteo el contador de sol malas greedy rand
                cant_iters_sin_sol_greedy_rand_factible=0;

                int mejora_current_iteration = 0;
                uint64_t cant_iters_bqlocal = 0;
                double promedio_parcial_bqlocal = 0;
                double promedio_bqlocal = 0;
                do{
                    promedio_parcial_bqlocal = 0;
                    MEDIR_TIEMPO_PROMEDIO(
                        mejora_current_iteration = g.busqueda_local(modo_busqueda_local);
                        , 1, &promedio_parcial_bqlocal);
                    cant_iters_bqlocal++;
                    promedio_bqlocal += promedio_parcial_bqlocal;            
                }while(mejora_current_iteration > 0);
                promedio_bqlocal = promedio_bqlocal /(double) cant_iters_bqlocal;

                //el tiempo de esta iteracion es la greedy randomized + bqlocal sobre esa sol inicial
                promedio += tiempo_golosa_randomized;
                promedio += promedio_bqlocal;
                
                //en este punto la bqlocal mejoro todo lo que pudo la sol. inicial obtenida con la randomized greedy
                //me fijo si esta solucion es mejor que la que tenia guardada, de ser asi actualizo el maximo y guardo la sol actual como la nueva mejor.
                camino = g.obtener_camino_solucion();
                costo_t costo_solucion_actual = camino.obtener_costo_total_w2_camino();

                //almaceno el costo total en esta iteracion
                if(costo_solucion_actual < costo_mejor_solucion){
                    //guardamos que en esta iteracion se encontro una mejora
                    if(cant_iters>0){//sino la primera vez pone infinito(costo_mejor_solucion es infinito al inicializar)
                        mejora_iters_grasp.push_back(make_pair(cant_iters, costo_mejor_solucion - costo_solucion_actual));                      
                    }
                    costo_mejor_solucion = costo_solucion_actual;
                    mejor_solucion = g.obtener_camino_solucion();
                    //reseteo el contador
                    cant_iters_sin_mejora = 0;
                }else{
                    //una iteracion consecutiva mas sin mejora
                    //guardamos que en esta iteracion se encontro una mejora
                    if(cant_iters>0){//sino la primera vez pone infinito(costo_mejor_solucion es infinito al inicializar)
                        mejora_iters_grasp.push_back(make_pair(cant_iters, 0));                    
                        
                    }
                    cant_iters_sin_mejora++;
                }
                //costo de la mejor sol guardada
                costo_t costo_w1_mejor_solucion = mejor_solucion.obtener_costo_total_w1_camino();
                costo_t costo_w2_mejor_solucion = mejor_solucion.obtener_costo_total_w2_camino();
                costo_camino_en_iteraciones.push_back(make_pair(costo_w1_mejor_solucion, costo_w2_mejor_solucion));
                cant_iters++;
            }else{
                cant_iters_sin_sol_greedy_rand_factible++;
                //si ya no tengo mas chanches, modifico los parametros de la metaheuristica
                if(cant_iters_sin_sol_greedy_rand_factible >= RAND_GREEDY_BAD_ITERS_LIMIT){
                    //si es rcl por cantidad bajo el parametro beta
                    if(modo_golosa == RCL_POR_CANTIDAD){
                        if(parametro_beta>2){
                            parametro_beta--;
                        }else{
                            //cout << "[GRASP] Golosa deterministica seteada." << endl;
                            parametro_beta=1;
                            modo_golosa = RCL_DETERMINISTICO;
                        }
                    }else if(modo_golosa == RCL_POR_VALOR){
                        //si es rcl por valor, seteo greedy deterministica
                        //es muy delicado ajustar el porcentaje adaptativamente!
                        parametro_beta = 0; //es lo mismo que poner golosa deterministico!
                        modo_golosa = RCL_DETERMINISTICO;
                    }
                    //cerr << "[GRASP] Greedy randomized dio sol mala. Cambiando parametro_beta: " << parametro_beta << endl;
                    cant_iters_sin_sol_greedy_rand_factible = 0;
                }
            }
        }else{
            break;//no hay solucion factible
        }
        if(criterio_terminacion == CRT_K_ITERS_LIMIT_REACHED){
            condicion_terminacion = (cant_iters < ITERS_LIMIT);
        }else if(criterio_terminacion == CRT_K_ITERS_SIN_MEJORA){
            condicion_terminacion = (cant_iters_sin_mejora < ITERS_LIMIT);
        }
    }while(condicion_terminacion);
    promedio = promedio / (double) cant_iters;

    //imprimo mediciones en stderr
    if(g.hay_solucion()){
        cerr << g.obtener_cantidad_nodos() << " " << g.obtener_cantidad_aristas() << " " << cant_iters << " " << promedio;        
        //mejora en iteraciones
        ofstream evolucion_iteraciones;
        evolucion_iteraciones.open(FILE_ITERS_MEJORA);
        for(auto element : mejora_iters_grasp){
            evolucion_iteraciones << element.first << " " << element.second << endl;
        }
        evolucion_iteraciones.close();

        //evolucion absoluta
        evolucion_iteraciones.open(FILE_ITERS_COSTOS_ABSOLUTOS);        
        for(uint i=1;i<=costo_camino_en_iteraciones.size();i++){
            //imprimr <iteracion> <costo_w2> <costo_w1>
            evolucion_iteraciones << i << " " << costo_camino_en_iteraciones[i-1].second << " " << costo_camino_en_iteraciones[i-1].first << endl;
        }
        evolucion_iteraciones.close();

        costo_t costo_w2_inicial = costo_camino_en_iteraciones.front().second;
        costo_t costo_w2_final = costo_camino_en_iteraciones.back().second;
        costo_t mejora_total_costo_w2 = costo_w2_inicial - costo_w2_final;

        evolucion_iteraciones.open(FILE_ITERS_COSTOS_ABSOLUTOS_STATISTICS);
            evolucion_iteraciones << costo_w2_inicial << " " << costo_w2_final << " " << mejora_total_costo_w2;
        evolucion_iteraciones.close();
    }

    g.serialize(cout, FORMATO_1_N_CLOSED);
}


//-----------------------------------------------------------------------------------------------------------------------

void ejecutar_greedy(Grafo &g){
    int limit_w1 = g.obtener_limite_w1();
    nodo_t nodo_src = g.obtener_nodo_origen();
    nodo_t nodo_dst = g.obtener_nodo_destino();
    #ifdef DEBUG_MESSAGES_ON
        //g.imprimir_lista_adyacencia(cout);
        //g.imprimir_matriz_adyacencia(cout);    
    #endif
    #ifdef DEBUG_MESSAGES_ON
        cout << "Se requiere un camino entre (" << nodo_src << ") y (" << nodo_dst<< ") que no exceda el costo " << limit_w1 << endl;
    #endif
    double promedio_medicion = 0;
    Camino camino = g.obtener_camino_vacio();
    MEDIR_TIEMPO_PROMEDIO(
                camino = g.obtener_solucion_golosa();
    , CANT_ITERS_MEDICION, &promedio_medicion);    
    
    g.establecer_camino_solucion(camino);
    if(g.hay_solucion()){
        cerr << g.obtener_cantidad_nodos() << " " << g.obtener_cantidad_aristas() << " " << CANT_ITERS_MEDICION << " " << promedio_medicion;
        #ifdef DEBUG_MESSAGES_ON
            cout << endl << "Solucion obtenida con golosa" << endl;
            camino.imprimir_camino(cout);
        #endif
    }

    g.serialize(cout, FORMATO_1_N_CLOSED);
}

//-----------------------------------------------------------------------------------------------------------------------


#include "grafo.h"
#include "timing.h"
#include <fstream>
#define FILE_ITERS_MEJORA "evolucion_iteraciones.txt"
#define FILE_ITERS_COSTOS_ABSOLUTOS "costos_absolutos_iteraciones_bqlocal.txt"
#define FILE_ITERS_COSTOS_ABSOLUTOS_STATISTICS "costos_absolutos_iteraciones_bqlocal_analisis.txt"

void ejecutar_busqueda_local(Grafo &g);

// -------------- Main ---------------------------------
int main(int argc, char **argv){
    list<Grafo> instancias = Grafo::parsear_varias_instancias(FORMATO_1_N_CLOSED);
    uint64_t instance_number = 1;
    for(Grafo &g : instancias){
        #ifdef DEBUG_MESSAGES_ON
            cout << endl << endl << "Aplicando busqueda local a la " << instance_number << "-esima instancia de input..." << endl;
        #endif
        ejecutar_busqueda_local(g);
        instance_number++;
    }
    return 0;
}

void ejecutar_busqueda_local(Grafo &g){
//    #ifdef DEBUG_MESSAGES_ON
//        g.imprimir_lista_adyacencia(cout);
//        //g.imprimir_matriz_adyacencia(cout);
//    #endif

    costo_t limit_w1 = g.obtener_limite_w1();
    nodo_t nodo_src = g.obtener_nodo_origen();
    nodo_t nodo_dst = g.obtener_nodo_destino();
    
    //--------------------------------- Busco solucion inicial ----------------------

    vector<costo_t> costo_minimo;
    vector<nodo_t> predecesor;
    g.dijkstra(nodo_src, COSTO_W1, costo_minimo, predecesor);
    
    costo_t costo_src_dst = costo_minimo[nodo_dst];//costo(src, dst)

    //--------------------------------- Valido la factibilidad de la solucion----------------
    #ifdef DEBUG_MESSAGES_ON
        cout << "Se requiere un camino entre (" << nodo_src << ") y (" << nodo_dst<< ") que no exceda el costo " << limit_w1;
    #endif
    if(costo_src_dst == costo_infinito){
        cerr << "No existe solucion factible. No existe camino entre origen(" << nodo_src << ") y destino(" << nodo_dst << ") " << endl;
        g.establecer_se_encontro_solucion(false);
    }else if(costo_src_dst > limit_w1){
        cerr << "No existe solucion factible. El camino minimo respecto a w1 de origen(" << nodo_src << ") a destino(" << nodo_dst << ") es de costo " << costo_src_dst << endl;
        g.establecer_se_encontro_solucion(false);
    }else{
        //armar camino entre origen y destino y lo establezco como sol inicial
        Camino c = g.obtener_camino_vacio();
        nodo_t nodo = nodo_dst;
        do{
            //cout << nodo << " " ;
            c.agregar_nodo_adelante(nodo);
            nodo = predecesor[nodo];
        }while(nodo != predecesor_nulo);
        g.establecer_camino_solucion(c);

        //Camino c = g.obtener_solucion_golosa();
        //g.establecer_camino_solucion(c);

        //imprimo sol inicial.
        #ifdef DEBUG_MESSAGES_ON
            cout << ". Costo minimo obtenido: " << c.obtener_costo_total_w1_camino();
            cout << "...Ok!!" << endl;
            cout << "Camino inicial: ";
            c.imprimir_camino(cout);
            cout << endl;
        #endif

        //--------------------------------- Comienzo la busqueda local -------------------   
        //typedef enum tipo_ejecucion_bqlocal_t {BQL_SUBDIVIDIR_PARES, BQL_CONTRAER_TRIPLAS_A_PARES, BQL_MEJORAR_CONEXION_TRIPLAS, BQL_COMBINAR} tipo_ejecucion_bqlocal_t;
        tipo_ejecucion_bqlocal_t tipo_ejecucion = BQL_COMBINAR;

        //hago iteraciones de busqueda local hasta que no haya mejora(la funcion devuelve true si hubo mejora, false sino)
        uint64_t cant_iters = 0;
        double promedio_parcial = 0;
        double promedio = 0;
        int mejora_current_iteration = 0;
        vector<costo_t> mejora_en_iteraciones;
        vector<pair<costo_t,costo_t> > costo_camino_en_iteraciones;//costos w1, w2
        costo_t costo_w1_current_iteration = c.obtener_costo_total_w1_camino();
        costo_t costo_w2_current_iteration = c.obtener_costo_total_w2_camino();
        
        //ponemos el costo inicial de la iteracino 0 del camino
        mejora_en_iteraciones.push_back(0);//mejora 0 en iteracion 0
        costo_camino_en_iteraciones.push_back(make_pair(costo_w1_current_iteration, costo_w2_current_iteration));
        do{
            promedio_parcial = 0;
            MEDIR_TIEMPO_PROMEDIO(
                mejora_current_iteration = g.busqueda_local(tipo_ejecucion);
                , 1, &promedio_parcial);
            cant_iters++;
            promedio += promedio_parcial;         
            costo_w1_current_iteration = g.obtener_costo_actual_w1_solucion();
            costo_w2_current_iteration = g.obtener_costo_actual_w2_solucion();
            costo_camino_en_iteraciones.push_back(make_pair(costo_w1_current_iteration, costo_w2_current_iteration));

            mejora_en_iteraciones.push_back(mejora_current_iteration);
        }while(mejora_current_iteration > 0);
        promedio = promedio /(double) cant_iters;

        #ifdef DEBUG_MESSAGES_ON
            switch(tipo_ejecucion){
                case BQL_SUBDIVIDIR_PARES:
                    cout << "Finalizo la busqueda local insertando entre pares porque no se obtuvieron nuevas mejoras." << endl;        
                    break;
                case BQL_CONTRAER_TRIPLAS_A_PARES:
                    cout << "Finalizo la busqueda local salteando entre triplas porque no se obtuvieron nuevas mejoras." << endl;
                    break;
                case BQL_MEJORAR_CONEXION_TRIPLAS:
                    cout << "Finalizo la busqueda local reemplazando entre triplas porque no se obtuvieron nuevas mejoras." << endl;
                    break;
                case BQL_COMBINAR:
                    cout << "Finalizo la busqueda local combinada porque no se obtuvieron nuevas mejoras." << endl;
                    break;                    
            }
        #endif
        g.establecer_se_encontro_solucion(true);
        cerr << g.obtener_cantidad_nodos() << " " << g.obtener_cantidad_aristas() << " " << cant_iters << " " << promedio;
        
        //mejora en iteraciones
        ofstream evolucion_iteraciones;
        evolucion_iteraciones.open(FILE_ITERS_MEJORA);
        for(uint i=1;i<=mejora_en_iteraciones.size();i++){
            evolucion_iteraciones << i << " " << mejora_en_iteraciones[i-1] << endl;
        }
        evolucion_iteraciones.close();
        
        evolucion_iteraciones.open(FILE_ITERS_COSTOS_ABSOLUTOS);        
        for(uint i=1;i<=costo_camino_en_iteraciones.size();i++){
            //imprimr <iteracion> <costo_w2> <costo_w1>
            evolucion_iteraciones << i << " " << costo_camino_en_iteraciones[i-1].second << " " << costo_camino_en_iteraciones[i-1].first << endl;
        }
        evolucion_iteraciones.close();

        costo_t costo_w2_inicial = costo_camino_en_iteraciones.front().second;
        costo_t costo_w2_final = costo_camino_en_iteraciones.back().second;
        costo_t mejora_total_costo_w2 = costo_w2_inicial - costo_w2_final;

        evolucion_iteraciones.open(FILE_ITERS_COSTOS_ABSOLUTOS_STATISTICS);
            evolucion_iteraciones << costo_w2_inicial << " " << costo_w2_final << " " << mejora_total_costo_w2;
        evolucion_iteraciones.close();
    }

    g.serialize(cout, FORMATO_1_N_CLOSED);
}

//-----------------------------------------------------------------------------------------------------------------------