
public class Main {

	public static void main(String[] args) {
		int[][] matrix = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
		//int[][] matrix = {{1, 1}, {1, 1}};
		int powerUpInicial = 1;
		
		int filaInicial = 0;
		int columnaInicial = 0;
		int filaDestino = 2 ;
		int columnaDestino = 2;
		//Nodo inicial y destino no distinguen el nivel		
		Juego juego = new Juego(matrix, powerUpInicial, filaInicial, columnaInicial, filaDestino, columnaDestino);
		System.out.println(juego.getGraphString());
		System.out.println(juego.caminoMinimo());
	}

}
