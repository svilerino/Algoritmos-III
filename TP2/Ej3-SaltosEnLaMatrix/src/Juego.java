import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map.Entry;

public class Juego {

	/**
	 * Nodo inicial
	 */
	Nodo nodoInicial;
	/**
	 * Nodo destino
	 */
	Nodo nodoDestino;
	
	/**
	 * Defino el grafo como un HashMap<Nodo, NodoMetadata> graph que entre otras cosas almacena <Nodo, adyacentes> para cada nodo
	 * El grafo G = <V, E> puede obtenerse como V = HashMap.keySet() && E = Union(graph.get(V.each()).getAlcanzables())
	 */
	HashMap<Nodo, NodoMetadata> graph;
	
	/**
	 * Precalculo todos los adyacentes de todos los nodos y me armo el grafo para luego aplicar el algoritmo  
	 * 
	 * Pre: (valoresNodos tiene dimension n*n con n natural > 0) && (0<=powerUpInicial) && (valoresNodos[i][j] = valorNodoIJ)
	 * && (nodoInicial y nodoDestino tienen coord validas)
	 * @param valoresNodos
	 * @param powerUpInicial
	 */
	public Juego(int [][] valoresNodos, int powerUpInicial, int filaInicial, int columnaInicial, int filaDestino, int columnaDestino){
		//como no nos piden ninguna restriccion ni optimizacion sobre la ultilizacion de powerup
		//los niveles de estos nodos no son tenidos en cuenta en el algoritmo de busqueda
		this.nodoInicial = new Nodo(filaInicial, columnaInicial, powerUpInicial/*dummy level*/);
		this.nodoDestino = new Nodo(filaDestino, columnaDestino, 0/*dummy level, puede ser cualquiera entre [0..powerUpInicial]*/);
		this.graph = new HashMap<Nodo, NodoMetadata>();
		//Dada la matriz de valores de cada nodo (i,j) y el powerUpInicial
		//Podemos definir el grafo(una matriz tridimensional de nodos) (i,j,k) donde 0<=k<=powerUpInicial
		
		int dimension = valoresNodos.length;
		
		for(int level=powerUpInicial;level>=0;level--){
			for(int i=0;i<dimension;i++){
				for(int j=0;j<dimension;j++){
					Nodo nodo = new Nodo(i, j, level);
					int nodoValue = valoresNodos[i][j];
					boolean fueVisitado = false;
					List<Nodo> alcanzables = calcularAlcanzables(nodo, nodoValue, dimension);
					NodoMetadata metaData = new NodoMetadata(nodoValue, fueVisitado, alcanzables, null);
					graph.put(nodo, metaData);
				}
			}			
		}
	}
	
	public String getGraphString(){		
		StringBuilder sb = new StringBuilder();
		sb.append("Grafo G:\nNodo v = potencia(v) | Adyacentes(v)\n");
		for(Entry<Nodo, NodoMetadata> entry : graph.entrySet()){
			NodoMetadata metadata = entry.getValue();
			sb.append(entry.getKey());
			sb.append(" = ");
			sb.append(metadata.getNodoValue());
			sb.append(" | ");
			for(Nodo adyacente : metadata.getAlcanzables()){
				sb.append(adyacente);
				sb.append(" -> ");
			}
			sb.append(" Nil\n");
		}		
		return sb.toString();
	}

	/**
	 * Sea un nodo v(i,j,k) sus alcanzables directos son 
	 * los nodos w de la forma (a, b, k) / ((i-c <= a <= i+c) && (b==j)) || ((j-c <= b <= j+c) && (a==i)) && c = potencia(v)
	 * ahora , sea l una variable que mueve en el rango [1..k]
	 * los nodos t de la forma (a, b, l) / ((a = i-c) || (a = i+c) && (b==j)) || ((b = j-c) || (b = j+c) && (a==i)) && c = potencia(v) + l
	 * @param nodo
	 * @param nodoValue 
	 * @return
	 */
	private List<Nodo> calcularAlcanzables(Nodo nodo, int nodoValue, int dimensionMatriz) {
		int currentLevel = nodo.getLevel();
		int currentX = nodo.getX();
		int currentY = nodo.getY();
		int potenciaIntrinseca = nodoValue;
		LinkedList<Nodo> alcanzables = new LinkedList<Nodo>();
		
		//armo los alcanzables directos en el eje X
		for(int j=-potenciaIntrinseca;j<=potenciaIntrinseca;j++){
			//validacion de bordes y que no sea el mismo
			if(validarBordes(dimensionMatriz, currentX, j)){
				Nodo alcanzableDirecto = new Nodo(currentX + j, currentY, currentLevel);
				alcanzables.add(alcanzableDirecto);
			}
		}
		
		//armo los alcanzables directo en el eje Y
		for(int j=-potenciaIntrinseca;j<=potenciaIntrinseca;j++){
			//validacion de bordes y que no sea el mismo
			if(validarBordes(dimensionMatriz, currentY, j)){
				Nodo alcanzableDirecto = new Nodo(currentX, currentY + j, currentLevel);
				alcanzables.add(alcanzableDirecto);
			}
		}
		
		//adiciono los alcanzables indirectos que surgen de utilizas unidadesDePowerUpAdicionadas unidades de powerup
		for(int unidadesDePowerUpAdicionadas=1;unidadesDePowerUpAdicionadas<=currentLevel;unidadesDePowerUpAdicionadas++){
			int potenciaTotal = potenciaIntrinseca + unidadesDePowerUpAdicionadas;
			if(validarBordes(dimensionMatriz, currentY, potenciaTotal)){
				Nodo alcanzableIndirecto = new Nodo(currentX, currentY + potenciaTotal, currentLevel - unidadesDePowerUpAdicionadas);
				alcanzables.add(alcanzableIndirecto);
			}
			
			if(validarBordes(dimensionMatriz, currentY, -potenciaTotal)){
				Nodo alcanzableIndirecto = new Nodo(currentX, currentY - potenciaTotal, currentLevel - unidadesDePowerUpAdicionadas);
				alcanzables.add(alcanzableIndirecto);
			}
			
			if(validarBordes(dimensionMatriz, currentX, potenciaTotal)){
				Nodo alcanzableIndirecto = new Nodo(currentX + potenciaTotal, currentY, currentLevel - unidadesDePowerUpAdicionadas);
				alcanzables.add(alcanzableIndirecto);
			}
			
			if(validarBordes(dimensionMatriz, currentX, -potenciaTotal)){
				Nodo alcanzableIndirecto = new Nodo(currentX - potenciaTotal, currentY, currentLevel - unidadesDePowerUpAdicionadas);
				alcanzables.add(alcanzableIndirecto);
			}
			
		}
		return alcanzables;
	}

	private boolean validarBordes(int dimensionMatriz, int coordenada, int corrimiento) {
		return (0<=coordenada+corrimiento) && (coordenada+corrimiento<dimensionMatriz) && (corrimiento!=0);
	}

	/**
	 * Modificacion de bfs que busca un camino entre inicial y target(sin importar la coordenada level)
	 * es decir, cuando bfs encuentra un nodo w tal que las primeras 2 coordenadas de w coinciden con target, termina el algoritmo
	 * Teniendo en cuenta que bfs encuentra el camino mas corto en cantidad de aristas entre 2 nodos, esto resuelve nuestro problema
	 * @return
	 */
	public List<Nodo> caminoMinimo(){
		LinkedList<Nodo> camino = new LinkedList<Nodo>();
		LinkedList<Nodo> queue = new LinkedList<Nodo>();
		
		queue.addLast(nodoInicial);
		marcarVisitado(nodoInicial);
		
		while(!queue.isEmpty()){
			Nodo current = queue.removeFirst();			
			if(esDestino(current)){
				//llegue a destino, termino la busqueda.
				System.out.print("Encontre camino: ");
				//armo el camino con los predecesores
				Nodo nodo = current;
				while(damePredecesor(nodo) != null){
					camino.addFirst(nodo);
					nodo = damePredecesor(nodo);
				}
				camino.addFirst(nodo);
				return camino;
			}
			List<Nodo> alcanzables = obtenerAlcanzables(current);
			for(Nodo alcanzable : alcanzables){				
				if(!estaVisitado(alcanzable)){
					marcarVisitado(alcanzable);
					asignarPredecesor(alcanzable, current);
					queue.addLast(alcanzable);				
				}
			}
		}
		//como bfs recorre todos los nodos, llegamos a este punto unicamente si no existe nodo destino en el grafo
		return null;
	}
	
	private Nodo damePredecesor(Nodo nodo) {
		return graph.get(nodo).getPredecesor();
	}

	private void asignarPredecesor(Nodo aMarcar, Nodo predecesor) {
		graph.get(aMarcar).setPredecesor(predecesor);
	}

	private boolean estaVisitado(Nodo nodo) {
		return graph.get(nodo).fueVisitado();
	}

	private boolean esDestino(Nodo current) {
		return (current.getX() == nodoDestino.getX()) && (current.getY() == nodoDestino.getY());
	}

	private void marcarVisitado(Nodo nodo){
		graph.get(nodo).marcarVisitado();
	}
	
	private List<Nodo> obtenerAlcanzables(Nodo nodo){
		return graph.get(nodo).getAlcanzables();
	}
	
}
