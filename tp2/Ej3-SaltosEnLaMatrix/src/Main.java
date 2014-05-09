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
	private static boolean randomGeneration = false;
	private static boolean takingTime = false;
	
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
		if(args.length>0){
			for(int i=0;i<args.length;i++){
				if("--generate-tests".equals(args[i])){
					/**
					 * Parameters later this flag
					 	* 	randMinDimension
						*	randMaxDimension
						*	randMinPowerUp
						*	randMaxPowerUp
					 */
					randomGeneration = true;
				}
				if("--take-time".equals(args[i])){
					takingTime = true;
				}
			}
		}
		
		if(randomGeneration){
			generateTest(args);
		}else{
			parseInput();	
			int CANT_REPETICIONES = 0;
			try{
				CANT_REPETICIONES = Integer.parseInt(args[1]);
			}catch(Exception e){
				System.out.println("[Bad parameters] --take-time <cant_repeticiones>");
				System.exit(-1);
			}
			List<Nodo> res = null;
			if(takingTime){
				double promedioNanoSeconds = 0;
				for(int k=0;k<CANT_REPETICIONES;k++){
					long start = System.nanoTime();
					Juego juego = new Juego(matrix, powerUpInicial, filaInicial, columnaInicial, filaDestino, columnaDestino);
					res = juego.caminoMinimo();
					long stop = System.nanoTime();
					long elapsed = stop - start;
					promedioNanoSeconds += elapsed;
				}
				promedioNanoSeconds/=CANT_REPETICIONES;
				double promedioMicroSeconds = Math.round(promedioNanoSeconds)/(double)1000;
				System.err.println(matrixDim * powerUpInicial + " " + promedioMicroSeconds + " " + CANT_REPETICIONES);
			}else{
				Juego juego = new Juego(matrix, powerUpInicial, filaInicial, columnaInicial, filaDestino, columnaDestino);
				res = juego.caminoMinimo();
			}
			
			System.out.println(serializeOutput(res));
		}		
	}

	private static void generateTest(String args[]) {		
		Integer randMinDimension = 0;
		Integer randMaxDimension = 0;
		Integer randMinPowerUp = 0;
		Integer randMaxPowerUp = 0;
		try{
			randMinDimension = Integer.parseInt(args[1]);
			if(randMinDimension<=1) throw new Exception("RandMinDimension <=1");
			
			randMaxDimension = Integer.parseInt(args[2]);

			randMinPowerUp = Integer.parseInt(args[3]);
			if(randMinPowerUp<0) throw new Exception("randMinPowerUp negativo!");
			
			randMaxPowerUp = Integer.parseInt(args[4]);				
		}catch(Exception e){
			System.out.println("[Bad Parameters] --generate-tests randMinDimension randMaxDimension randMinPowerUp randMaxPowerUp");
			System.out.println("Error!:" + e.getMessage());
			System.exit(-1);
		}
		
		matrixDim = randomInt(randMinDimension, randMaxDimension);
		filaInicial = randomInt(0, matrixDim-1);
		columnaInicial = randomInt(0, matrixDim-1);
		
		do{
			filaDestino = randomInt(0, matrixDim-1);
			columnaDestino = randomInt(0, matrixDim-1);
		}while(filaDestino==filaInicial && columnaDestino==columnaInicial);

		powerUpInicial = randomInt(randMinPowerUp, randMaxPowerUp);
		
		matrix = new int[matrixDim][matrixDim];
		System.out.println(matrixDim + " " + filaInicial + " " + columnaInicial + " " + filaDestino + " " + columnaDestino + " " + powerUpInicial);
		for(int i=0;i<matrixDim;i++){
			for(int j=0;j<matrixDim;j++){
				matrix[i][j] = randomInt(1, matrixDim-1);
				System.out.print(matrix[i][j]);
				if(j!=matrixDim-1){
					System.out.print(" ");
				}
			}
			System.out.println(" ");
		}
	}
	
	private static int randomInt(int min, int max){
		return (int) ((max-min + 1)*Math.random() + min);
	}

}
