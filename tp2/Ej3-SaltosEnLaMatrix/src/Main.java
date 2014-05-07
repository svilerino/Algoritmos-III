import java.util.List;

public class Main {

	public static void parseInput(){
		//...TODO: complete this
	}
	
	public static String serializeOutput(List<Nodo> caminoOptimo){
		StringBuilder sb = new StringBuilder();
		sb.append(caminoOptimo.size());
		sb.append("\n");
		for(Nodo paso : caminoOptimo){
			sb.append(paso.getX());
			sb.append(" ");
			sb.append(paso.getY());
			sb.append(" ");
			sb.append(paso.getLevel());
			sb.append("\n");
		}
		return sb.toString();
	}
	
	public static void main(String[] args) {
		int[][] matrix = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
		//int[][] matrix = {{1, 1}, {1, 1}};
		int powerUpInicial = 2;
		
		int filaInicial = 0;
		int columnaInicial = 0;
		int filaDestino = 2 ;
		int columnaDestino = 2;
		//Nodo inicial y destino no distinguen el nivel		
		Juego juego = new Juego(matrix, powerUpInicial, filaInicial, columnaInicial, filaDestino, columnaDestino);
		//System.out.println(juego.getGraphString());
		System.out.println(serializeOutput(juego.caminoMinimo()));
	}

}
