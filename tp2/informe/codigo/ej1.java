public class RobaNumeros {
	
	private int turnosJugados;
	private int puntosJugador1;
	private int puntosJugador2;
	private TuplaTurnos turnos;
	
	public void calcular( int[] entrada ){
		//Calcula las tablas S y P
		Tablas_S_P tabla_res = new Tablas_S_P(entrada);
		
		//Las usa para obtener los datos requeridos por el enunciado
		
		turnos = new TuplaTurnos(entrada.length); //Como maximo se juegan tantos turnos como cantidad de cartas
		turnosJugados=1; //se juega siempre al menos un turno, el primero
		puntosJugador1 = 0;
		puntosJugador2 = 0;
		int i = 1;
		int j = entrada.length;
		boolean jugador = true; //true es jugador1
		while(tabla_res.P()[i-1][j-1][0] != 0){ //La condicion es true si no se agarraron todas las cartas
			
			//calculo lado
			if(tabla_res.P()[i-1][j-1][0] != i){
				turnos.lado[turnosJugados-1] = true;
			}
			else{
				turnos.lado[turnosJugados-1] = false;
			}
			//calculo cant de cartas robadas
			turnos.cant[turnosJugados-1] = (j - i) - (tabla_res.P()[i-1][j-1][1] - tabla_res.P()[i-1][j-1][0]);
			
			//sumo puntos para el jugador que juega actualmente
			if(turnos.lado[turnosJugados-1] == true){ //Si se saco de izquierda
				if(jugador == true){
					puntosJugador1 += tabla_res.S()[ i-1 ][ i + turnos.cant[turnosJugados-1] -2];
				}
				else{
					puntosJugador2 += tabla_res.S()[ i-1 ][ i + turnos.cant[turnosJugados-1] -2];
				}
			}
			else{	//Si se saco de derecha
				if(jugador == true){
					puntosJugador1 += tabla_res.S()[ j - (turnos.cant[turnosJugados-1] -1) -1][j-1];
				}
				else{
					puntosJugador2 += tabla_res.S()[ j - (turnos.cant[turnosJugados-1] -1) -1][j-1];
				}
			}
			
			int i2 = i;
			i = tabla_res.P()[i-1][j-1][0];
			j = tabla_res.P()[i2-1][j-1][1];
			jugador = !jugador;
			turnosJugados++;
		}
		
		//Falta lo correspondiente al ultimo turno
		turnos.lado[turnosJugados-1] = true; // Se elige lado por defecto "izq" para cuando se sacan todas
		turnos.cant[turnosJugados-1] = j - i + 1;
		if(jugador == true){
			puntosJugador1 += tabla_res.S()[i-1][j-1];
		}
		else{
			puntosJugador2 += tabla_res.S()[i-1][j-1];
		}
		
	}
	
	public int getTurnosJugados(){
		return turnosJugados;
	}
	
	public int getPuntosJugador1(){
		return puntosJugador1;
	}
	
	public int getPuntosJugador2(){
		return puntosJugador2;
	}
	
	public boolean esTurnoIzquierdo(int turno){
		return turnos.lado[turno-1];
	}
	
	public int getCantidadRobadaPorTurno(int turno){
		return turnos.cant[turno-1];
	}
	
	private class TuplaTurnos{
		
		public TuplaTurnos(int cant_turnos){
			lado = new boolean[cant_turnos];
			cant = new int[cant_turnos];
		}
			
		public boolean lado[]; //true es "izq"
		public int cant[]; 
	
	}
	
	private class Tablas_S_P {
		
		private int S[][];
		private int P[][][];// P[i][j] es una tupla que en las dos primeras coordenadas tiene el i y el j que quedan en la mesa despues de jugado el turno (� (0,0) si se levantan todas las cartas) y en la tercer coordenada tiene a la funcion P propiamente.
		private int tira_entrada[];
		
		public Tablas_S_P(int entrada[]){
			tira_entrada = entrada;
			S = new int[tira_entrada.length][tira_entrada.length];
			//Calcula tabla S
			calcular_S();
			P = new int[tira_entrada.length][tira_entrada.length][3];
			//Calcula tabla P a partir de la S
			calcular_P();
		}
		
		public int [][][] P(){
			return P;
		}
		public int [][] S(){
			return S;
		}
		
		private void calcular_S(){
			int i;
			for(i=1;i<=tira_entrada.length;i++){
				int j;
				for(j=i;j<=tira_entrada.length;j++){
					if(i==j){
						S[i-1][j-1] = tira_entrada[i-1];
					}
					else{
						S[i-1][j-1] = S[i-1][j-2] + tira_entrada[j-1];
					}
				}
			}
		}
		
		private void calcular_P(){ //Recorre la tabla de columna 1 hacia columna n, cada una de abajo hacia arriba.
			int j;
			for (j=1; j<=tira_entrada.length;j++){
				int i;
				for(i=j; i>=1;i--){
					
						int[] restante = {i,j};
						Double MAX = Double.NEGATIVE_INFINITY;
						int k;
						//Para cada posicion, que representa un intervalo (i,j) de cartas, calcula la eleccion de cartas a robar, primero desde la izquierda y luego desde la derecha
						//En cada posicion queda guardada una tupla con el valor de P y ademas los indices (i,j) del intervalo de cartas que quedan por jugarse, � (0,0) en caso robar todo.
						for(k=0;k<=j-i;k++){
							int m;
							m = S[i-1][i+k-1] - ( (k==j-i)?0:P[i+k][j-1][2] );
							if(m > MAX){
								MAX = (double) m;
								if(k==j-i){
									restante[0]=0;
									restante[1]=0;
								}
								else{
									restante[0]=i+k+1;
									restante[1]=j;
								}
							}
						}
						
						for(k=0;k<=j-i;k++){
							int m;
							m = S[j-k-1][j-1] - ( (k==j-i)?0:P[i-1][j-k-2][2] );
							if(m > MAX){
								MAX = (double) m;
								if(k==j-i){
									restante[0]=0;
									restante[1]=0;
								}
								else{
									restante[0]=i;
									restante[1]=j-k-1;
								}
							}
						}
						double _MAX = MAX;
						int tupla[] = {restante[0],restante[1],(int)_MAX};
						P[i-1][j-1] = tupla;
					
					
				}
			}
		}
	}
	

}
