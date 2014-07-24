int main(int argc, char** argv){
	int cant_nodos = 0;
	int cant_aristas = 0;
	int pesomin_w1 = 0;
	int pesomax_w1 = 0;
	int pesomin_w2 = 0;
	int pesomax_w2 = 0;
	int limit_w1 = 0;

	if(argc != 8){
		cout << "Use: bin <cant nodos> <cant_aristas> <pesomin_w1> <pesomax_w1> <pesomin_w2> <pesomax_w2> <limit_w1> " << endl;
		return 1;
	}else{
		cant_nodos = atoi(argv[1]);
		cant_aristas = atoi(argv[2]);
		pesomin_w1 = atoi(argv[3]);
		pesomax_w1 = atoi(argv[4]);
		pesomin_w2 = atoi(argv[5]);
		pesomax_w2 = atoi(argv[6]);
		limit_w1 = atoi(argv[7]);
	}
	int aristas_maximas = cant_nodos*(cant_nodos-1)/2;	

	//validacion maximas aristas
	if(cant_aristas > aristas_maximas){
		cant_aristas = aristas_maximas;
	}

	//inicializo en <(0, 1), ..., (0, n-1), (1, 2), ..., (1, n-1), ..., (n - 2, n-1)>
	vector<pair<int, int> > aristas;
	for(int i=0;i<cant_nodos;i++){
		for(int j=i+1;j<cant_nodos;j++){			
			aristas.push_back(make_pair(i, j));
		}
	}

	//ahora shufleo uniformemente con fisher yates
	//http://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle

	random_device rd;
	mt19937 gen(rd());

	uniform_int_distribution<int> dis_srcdst(1, cant_nodos);
	int nodo_src = dis_srcdst(gen);
	int nodo_dst = dis_srcdst(gen);

	//header line: nodos aristas src dst limitw1
	cout << cant_nodos << " " << cant_aristas << " " << nodo_src << " " << nodo_dst << " " << limit_w1 << endl;

	int permutations_number = aristas_maximas;//C(cant_nodos, 2)
	uniform_int_distribution<int> dis(0, permutations_number);
	vector<pair<int, int> >::iterator begin_iter = aristas.begin();
	for(int i=0;i<permutations_number;i++){
		int x = dis(gen);
		iter_swap(begin_iter + i, begin_iter + x);
	}

	//ahora agarro del vector randomizado de aristas, las primeras cant_aristas y las imprimo con pesos aleatorios entre 
	aristas.resize(cant_aristas);

	uniform_int_distribution<int> dis_w1(pesomin_w1, pesomax_w1);
	uniform_int_distribution<int> dis_w2(pesomin_w2, pesomax_w2);
	for(auto arista : aristas){
		//notemos que la salida es entre 1..n!!
		int partida = arista.first + 1;
		int destino = arista.second + 1;
		int peso_w1 = dis_w1(gen);
		int peso_w2 = dis_w2(gen);
		cout << partida << " " << destino << " " << peso_w1 << " " << peso_w2 << endl;
	}
	cout << "0";
}