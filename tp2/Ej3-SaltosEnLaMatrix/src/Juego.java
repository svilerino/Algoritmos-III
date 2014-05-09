import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map.Entry;
/**
 * 
 * @author svilerino
 *	Implementacion:
 *	El algoritmo consta de varias etapas principales
 *		1- Generacion del grafo a partir de los datos de entrada(Constructor de la clase Juego)
 *		2- Busqueda del camino minimo(en cantidad de pasos) con un BFS.(metodo caminoMinimo)
 *		3- Construccion del camino a través de los predecesores del destino indicados por BFS y
 *			calculo de powerup utilizado en cada salto. (metodo construirCaminoConPredecesores)
 *
 *	Complejidad:
 *		1- Los 3 for anidados indican una complejidad de O(k.pow(n,2)), adentro de estos ciclos
 *			se realizan operaciones, todas toman O(1) salvo la llamada a alcanzables(...) de la que hablaremos
 *			mas abajo, pero toma O(n), dandonos un total de O(k.pow(n,3) en la construccion del grafo para nuestro modelo.
 *			En el informe se detallara mas acerca de esto, pero veamos que cada nodo puede tener a lo sumo 2n alcanzables.
 *			Asumiendo que acotamos potencia(v) <= dimensionMatriz de aqui en adelante tenemos:
 *			Dado que podemos particionar alcanzables(v) = {alcanzables con potTotal <= +-potencia(v)} 
 *			U disj {alcanzables con potTotal = potencia(v) + powerUpK1} U disj {alcanzables con potTotal = potencia(v) + powerUpK2}
 *			...U disj {alcanzables con potTotal = potencia(v) + powerUpKn}, en el mejor de los casos para este analisis, toda la fila y columna
 *			de un nodo seran alcanzables sin utilizar powerup, lo cual nos dara el primer conjunto union conjuntos vacios, el cardinal de
 *			alcanzables(v) es entonces 2n, en otros casos, veamos que si no es alcanzado por la potencia intrinseca de la torre, todos los
 *			conjuntos que se unen tienen distinta potencia total e incluyen a los nodos que son alcanzables estrictamente con esa potenciaTotal.
 *			En el mejor de los casos, de nuevo, usando powerup, entre todos los conjuntos, se pueden alcanzar 2n nodos, con lo cual
 *			viendo los ciclos en el metodo calcularAlcanzables y asumiendo que la operacion put de hashmap es O(1) amortizada, nos da
 *			una complejidad total de O(n).
 *	
 *		2- Sea n = {dimension de la matriz cuadrada de entrada}, k = unidades de powerUp Disponibles al inicio del algoritmo + 1(hay capas [0..k] en el grafo)
 *			El while principal del ciclo recorre todos los nodos en el peor caso(cuando la busqueda no se detiene en el if(esDestino(...))
 *			Veamos porque: En cada iteracion, se visita un nodo, se lo marca para no visitarlo nuevamente, y se verifica que no sea
 *			el nodo que estamos buscando(nodo destino), si lo es, termina el algoritmo de busqueda y se llama a la funcion construirCaminoConPredecesores
 *			Caso contrario, se agregan a la cola todos los nodos adyacentes(alcanzables) NO visitados, ademas se indicarse quien es el precedesor, en este paso.
 *			Veamos ciertos detalles tecnicos, obtener los adyacentes de un nodo es O(1) amortizado(acceder al hashmap por key) y O(n) para encolar todos los adyacentes
 *			(cada nodo tenia 2n alcanzables como maximo). Sea f(G, i) = {cantidad de nodos sin visitar en G en la iteracion i}, esta funcion tiene un valor inicial f(G, 0) = pow(n,2)*k y
 *			en cada iteracion esta funcion decrece y toma el valor cero en f(G, pow(n,2)*k) = f(G, cantNodosG) = 0, en este momento, no hay mas nodos no visitados para agregar 
 *			a la cola. Luego, se procesaran todos los elementos encolados y terminara el algoritmo. Juntando todas estas observaciones vemos que el 
 *			while(!queue.isEmpty()) esta asociado a la funcion f(G, i), que decrece en cada iteracion al menos una unidad, en el peor de los casos, 
 *			tomara O(f(G,0)) = O(pow(n,2)*k) = {cantNodos de G al comenzar} en recorrer todos los nodos.
 *			la funcion esDestino(...) toma tiempo constante, de ingresar en este if, termina el algoritmo con un costo O(f(G, 0)) adicional por armar
 *			el camino con la funcion construirCaminoConPredecesores. La lista de alcanzables se obtenia en O(1), iterar sobre los alcanzables(adyacentes)
 *			realizando operaciones de tiempo constante adentro de este subciclo	era O(n). En total, este metodo tiene una complejidad temporal
 *			{iterar sobre todos los nodos O(k*pow(n, 2))}*({esDestino O(1)} + {obtenerAlzancables O(1} + {encolarAlcanzables O(n}) + {al finalizar armar camino O(k*pow(n, 2))}
 *			= O(k*pow(n, 2))*O(n) + O(k*pow(n, 2)) = O(k*pow(n, 3)) 
 *
 *		3- Asumiendo que BFS funciona correctamente, la lista de nodos del camino sera a lo sumo pow(n,2) (todos los nodos), dandonos
 *			una complejidad de O(ṕow(n,2)) dado el while que arma el camino en base a los predecesores. Adentro del ciclo se realizan
 *			operaciones de tiempo constante, dandonos una complejidad temporal total de O(pow(n, 2))			
 *
 *		Complejidad total: {1- armar el grafo O(k*pow(n, 3))} + {2,3- Algoritmo de busqueda en anchura con armado de camino O(k*pow(n, 3))} = O(k*pow(n, 3))
 *
 *			Referencias de estructuras utilizadas:
 *			http://docs.oracle.com/javase/7/docs/api/java/util/HashMap.html
 *			...This implementation provides constant-time performance for the basic operations (get and put)...
 */
public class Juego {

	//Nodo inicial
	Nodo nodoInicial;

	// Nodo destino
	Nodo nodoDestino;
	
	//Dada la matriz de valores de cada nodo (i,j) y el powerUpInicial
	//Podemos definir el grafo(una matriz tridimensional de nodos) (i,j,k) donde 0<=k<=powerUpInicial
	//Defino el grafo como un HashMap<Nodo, NodoMetadata> graph 
	//que entre otras cosas almacena los nodos adyacentes para cada nodo
	//El grafo G = <V, E> puede obtenerse como 
	//V = HashMap.keySet() && E = Union(graph.get(V.each()).getAlcanzables())
	HashMap<Nodo, NodoMetadata> graph;
	
	/**
	 * Precalculo todos los adyacentes de todos los nodos y me armo el grafo para luego aplicar el algoritmo que resuelve el problema  
	 * 
	 * Pre: (valoresNodos tiene dimension n*n con n natural > 0) && (0<=powerUpInicial) && (valoresNodos[i][j] = valorNodoIJ)
	 * && (nodoInicial y nodoDestino tienen coord validas)
	 * @param valoresNodos
	 * @param powerUpInicial
	 */
	public Juego(int [][] valoresNodos, int powerUpInicial, int filaInicial, int columnaInicial, int filaDestino, int columnaDestino){
		//inicializo estructuras y atributos
		this.nodoInicial = new Nodo(filaInicial, columnaInicial, powerUpInicial);
		this.nodoDestino = new Nodo(filaDestino, columnaDestino, 0/*dummy level l, l puede ser cualquiera entre [0..powerUpInicial]*/);
		this.graph = new HashMap<Nodo, NodoMetadata>();
		
		//genero el grafo que modela el problema
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
		
		//armo los alcanzables directo
		for(int j=-potenciaIntrinseca;j<=potenciaIntrinseca;j++){
			//validacion de bordes y que no sea el mismo
			if(validarBordes(dimensionMatriz, currentX, j)){
				Nodo alcanzableDirecto = new Nodo(currentX + j, currentY, currentLevel);
				alcanzables.add(alcanzableDirecto);
			}

			//validacion de bordes y que no sea el mismo
			if(validarBordes(dimensionMatriz, currentY, j)){
				Nodo alcanzableDirecto = new Nodo(currentX, currentY + j, currentLevel);
				alcanzables.add(alcanzableDirecto);
			}
		}
		
		//adiciono los alcanzables indirectos que surgen de utilizas unidadesDePowerUpAdicionadas unidades de powerup
		for(int unidadesDePowerUpAdicionadas=1;unidadesDePowerUpAdicionadas<=currentLevel;unidadesDePowerUpAdicionadas++){
			int potenciaTotal = potenciaIntrinseca + unidadesDePowerUpAdicionadas;
			
			if(potenciaTotal>dimensionMatriz){							
				//fix para complejidad, de esta forma es O(dimension) todo el metodo.
				//si potenciaTotal > dimensionMatriz, validarBordes va a dar false, asi que ni hace falta revisarlas			
				//veamos que como potenciaIntrinseca > 0 para cualquier nodo, entonces si unidadesDePowerUpAdicionadas > n-1
				//entonces potenciaTotal > dimensionMatriz lo cual no tiene sentido, porque se podria saltar por fuera de los limites de la matriz
				break;			
			}			
			
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
	 * y reconstruye el camino en base a los predecesores, que se van guardando a medida que se realiza la expansion en anchura
	 * Teniendo en cuenta que bfs encuentra el camino mas corto en cantidad de aristas entre 2 nodos, esto resuelve nuestro problema.
	 * @return Devuelve una lista de nodos indicando (posX, posY, gasto de PowerUp en el salto del predecesor de este nodo al actual)
	 */
	public List<Nodo> caminoMinimo(){		
		LinkedList<Nodo> queue = new LinkedList<Nodo>();
		
		queue.addLast(nodoInicial);
		marcarVisitado(nodoInicial);
		
		while(!queue.isEmpty()){
			Nodo current = queue.removeFirst();			
			if(esDestino(current)){
				//llegue a destino, termino la busqueda y devuelvo el resultado.
				//System.out.println("Camino c <(origen),...(x, y, powerUtilizado)...,(destino)>");
				//armo el camino con los predecesores
				return construirCaminoConPredecesores(current);				
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

	private LinkedList<Nodo> construirCaminoConPredecesores(Nodo current) {
		//Tengamos en cuenta que en esta lista hay tuplas(x,y,z), x,y son las
		//posiciones en el plano z, que indica, la cantidad de powerup disponible.
		//Haciendo la resta entre el valor level de dos nodos v->w, se calcula el
		//powerup utilizado para dicho salto entre v y w
		//Definamos powerUpUsado(u,v) = level(v) - level(u)
		//Asumamos que powerUpUsado(null, v) = 0;				
		//powerUpInicial - level(destino) te da la cantidad de powerUp usado total y
		//level(destino) la cantidad de powerup disponible al salir del laberinto.
		LinkedList<Nodo> camino = new LinkedList<Nodo>();
		//armo una lista de nodos que indican el camino de origen a destino.
		//como tengo el predecesor para cada nodo, puedo ir de atras hacia adelante encadenandolo.
		Nodo nodo = current;
		while(damePredecesor(nodo) != null){
			Nodo predecesor = damePredecesor(nodo);
			nodo.setLevel(predecesor.getLevel() - nodo.getLevel());
			camino.addFirst(nodo);

			//avanzo en el camino
			nodo = predecesor;
		}
		nodo.setLevel(0);
		camino.addFirst(nodo);
		return camino;
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

	private void marcarVisitado(Nodo nodo){
		graph.get(nodo).marcarVisitado();
	}
	
	private List<Nodo> obtenerAlcanzables(Nodo nodo){
		return graph.get(nodo).getAlcanzables();
	}

	private boolean esDestino(Nodo current) {
		//como no nos piden ninguna restriccion ni optimizacion sobre la ultilizacion de powerup
		//los niveles de estos nodos no son tenidos en cuenta en el algoritmo de busqueda para distinguir cuando llego a destino
		return (current.getX() == nodoDestino.getX()) && (current.getY() == nodoDestino.getY());
	}
	
}
