bool compare_function (tuple<int, int, int> e1, tuple<int, int, int> e2){
	//tupla < depreciacion, tiempo, posicion en la entrada >
	return (get<1>(e1)*get<0>(e2)) > (get<0>(e1)*get<1>(e2));
}

void solveInstance(vector<tuple<int, int, int> >& piezas, 
	uint cantidadPiezas, uint& totalPerdido){
	sort(piezas.begin(), piezas.end(), compare_function); // ordenar segun ti/di	
	// recorro el vector de adelante para atras para hacer la acumulacion de perdidas
	int b = cantidadPiezas - 1; 
	//suma de las depreciaciones a sumar
	uint depreciaciones = 0; 
	/*
		Para un calculo de 3 piezas seria = t1*(d1+d2+d3) + t2*(d2+d3) +t3*(d3).   
		En depreciaciones voy acumulando los "d"
	*/
	while(b >= 0)
	{
		depreciaciones = depreciaciones + get<0>(piezas[b]);
		totalPerdido = totalPerdido + get<1>(piezas[b])*depreciaciones;
		b--;
	}
}