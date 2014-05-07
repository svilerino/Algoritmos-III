import java.util.List;
import java.util.Scanner;
import java.util.StringTokenizer;

public class Main {

	private static int powerUpInicial;	
	private static int filaInicial;
	private static int columnaInicial;
	private static int filaDestino;
	private static int columnaDestino;
	private static int[][] matrix;
	private static int matrixDim;
	
	public static void parseInput(){
		Scanner in = new Scanner(System.in);
		String firstLine = in.nextLine();
		//dimension fo co fd cd powerupDisp
		String[] splitUp = firstLine.split(" ");
		matrixDim = Integer.parseInt(splitUp[0]);
		filaInicial = Integer.parseInt(splitUp[1]);
		columnaInicial = Integer.parseInt(splitUp[2]);
		filaDestino = Integer.parseInt(splitUp[3]);
		columnaDestino = Integer.parseInt(splitUp[4]);
		powerUpInicial = Integer.parseInt(splitUp[5]);
		
		matrix = new int[matrixDim][matrixDim];
		
		for(int i=0;i<matrixDim;i++){
			String line = in.nextLine();
			StringTokenizer st = new StringTokenizer(line);
			for(int j=0;j<matrixDim;j++){
				matrix[i][j] = Integer.parseInt(st.nextToken(" "));				
			}							
		}
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
		parseInput();
		//matrix = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
		//matrix = {{1, 1}, {1, 1}};
		
		Juego juego = new Juego(matrix, powerUpInicial, filaInicial, columnaInicial, filaDestino, columnaDestino);
		//System.out.println(juego.getGraphString());
		System.out.println(serializeOutput(juego.caminoMinimo()));
	}

}
