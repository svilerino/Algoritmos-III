import java.util.Scanner;

public class Main {

	private static boolean randomGeneration;
	private static boolean takingTime;

	public static void main(String[] args) {				
		if(args.length>0){
			for(int i=0;i<args.length;i++){
				if("--generate-tests".equals(args[i])){
					/**
					 * Parameters later this flag
					 	*   cards number 	
					 	* 	randMin
						*	randMax
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
			//parse input
			int[] cards = parseInput();			
			int CANT_REPETICIONES = 0;
			try{
				CANT_REPETICIONES = Integer.parseInt(args[1]);
			}catch(Exception e){
				System.out.println("[Bad parameters] --take-time <cant_repeticiones>");
				System.exit(-1);
			}
			
			if(takingTime){
				//run algorithm taking time.
				double promedioNanoSeconds = 0;
				RobaNumeros juego = null;
				for(int k=0;k<CANT_REPETICIONES;k++){
					long start = System.nanoTime();
						//--starts algorithm
						juego = new RobaNumeros();
						juego.calcular(cards);
						//para obtener los resultados usar los getters de la clase RobaNumeros luego de llamar a calcular(...)
						//--ends algorithm
					long stop = System.nanoTime();
					long elapsed = stop - start;
					promedioNanoSeconds += elapsed;
				}
				promedioNanoSeconds/=CANT_REPETICIONES;
				double promedioMicroSeconds = Math.round(promedioNanoSeconds)/(double)1000;
				System.err.println(cards.length + "	" + promedioMicroSeconds);
				serializeOutput(juego);
			}else{
				//algoritmo
				RobaNumeros juego = new RobaNumeros();
				juego.calcular(cards);
				serializeOutput(juego);
			}
		}
	}

	private static void serializeOutput(RobaNumeros juego) {
		int turnosJugados = juego.getTurnosJugados();
		
		System.out.print(turnosJugados);
		System.out.print(" ");
		
		System.out.print(juego.getPuntosJugador1());
		System.out.print(" ");
		
		System.out.print(juego.getPuntosJugador2());
		System.out.println(" ");
		
		for(int turnoActual=1;turnoActual<=turnosJugados;turnoActual++){
			System.out.print((juego.esTurnoIzquierdo(turnoActual)) ? "izq" : "der");
			System.out.print(" ");
			System.out.println(juego.getCantidadRobadaPorTurno(turnoActual));
		}		
	}

	private static void generateTest(String[] args) {
		/**
		 * Parameters later this flag
		 	*   cards number 	
		 	* 	randMin
			*	randMax
		 */
		int cardsNumber = Integer.parseInt(args[1]);		
		int randMin = Integer.parseInt(args[2]);
		int randMax = Integer.parseInt(args[3]);
		System.out.print(cardsNumber);
		System.out.print(" ");
		for(int i=0;i<cardsNumber;i++){
			System.out.print(randomInt(randMin, randMax));
			if(i<cardsNumber-1)
				System.out.print(" ");
		}
		System.out.println("");
	}

	private static int[] parseInput() {
		Scanner in = new Scanner(System.in);
		String input = in.nextLine();
		String[] splittedInput = input.split(" ");
		
		//La primera linea contiene <longuitud k> <v1>,...,<vl>
		
		int cardCount = splittedInput.length - 1;
		int[] robanumerosInput = new int[cardCount];
		for(int i=1;i<cardCount;i++){
			robanumerosInput[i] = Integer.parseInt(splittedInput[i]);
		}
		return robanumerosInput;
	}
	
	private static int randomInt(int min, int max){
		return (int) ((max-min + 1)*Math.random() + min);
	}

}
