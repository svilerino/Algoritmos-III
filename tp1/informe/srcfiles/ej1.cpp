/**
	PRE:  	(diasInspector>0)

	POST:   res_1 = dia inicial conveniente de contratacion
			res_2 = cantidad de camiones capturados para revision en dicho periodo

	COMPL:	O(n.log(n))
**/
pair<int, int> solveInstance(int diasInspector, vector<int> entrada){
	//Ordeno los dias de la entrada de menor a mayor
	//la STL me garaniza que este sorting es O(n.log(n))
	sort(entrada.begin(), entrada.end());

	//agrupar repetidos y armar tuplas (dia, suma de camiones acumulada hasta ese dia inclusive)
	//Sea n = |entrada|
	vector<pair<int, int> > tablaDiaCantidad;//O(1)
	int longuitud = entrada.size();//O(1)
	tablaDiaCantidad.reserve(longuitud);//O(n)
	int i=0;//O(1)
	int apariciones = 0;//O(1)
	while(i < longuitud) {//O(n)
		apariciones++;//O(1)
		if((entrada[i] != entrada[i+1]) || (i==longuitud-1)){
			pair<int, int> par = make_pair(entrada[i], apariciones);//O(1)
			tablaDiaCantidad.push_back(par);//O(1) - no pueden ocurrir 
			//realocamientos pues reserve  n elementos, y en el peor caso,
			//no hay dias repetidos y quedan n entradas en la tabla esta.
		}
		i++;//O(1)
	}

	//buscar ventanas entre las cuales revisa el inspector computando cuantos camiones
	//revisa y tomando la ventana que mas camiones revise
	int maximo=-1;
	int idxComienzoIntervaloOptimo=-1;
	for (uint i = 0; i < tablaDiaCantidad.size(); ++i)
	{
		pair<int, int> target = tablaDiaCantidad[i];
		target.first += (diasInspector-1);
		//la busqueda se reduce al intervalo [i..n] pues como D>0, y tablaDiaCantidad esta ordenada 
		//por dia, i-esimo dia + D va a estar mas adelante en el arreglo que i-esimo dia.
		vector<pair<int, int> >::iterator upperBound = upper_bound (tablaDiaCantidad.begin() + i,
		tablaDiaCantidad.end(), target.first, compare_pair_number);//O(log n)		

		tablaDiaCantidad.end(), target.first, compare_pair_number);//O(log n)		
		pair<int, int> res = tablaDiaCantidad[(upperBound - tablaDiaCantidad.begin() - 1)];
		
		int resta=0;
		//supongo que vino el primer dia. no se le resta nada a la suma acumulada
		if(i>0){//si no vino el primer dia, corrijo la resta
			//asumo i>0(no puede ser negativo)
			resta = tablaDiaCantidad[i-1].second;
		}
		int cantCamionesDelIntervalo = (res.second-resta);
		if(cantCamionesDelIntervalo>maximo){
			maximo=cantCamionesDelIntervalo;
			idxComienzoIntervaloOptimo = tablaDiaCantidad[i].first;
		}
	}
	return make_pair(idxComienzoIntervaloOptimo, maximo);
}
// ---------------------------------------------------------------------------------------- //

/**
	PRE:	True
	
	POST:	(n < p_1)

	COMPL:	O(1)
**/
bool compare_pair_number(int n, pair<int, int> const& p)
{
    return (n < p.first);
}