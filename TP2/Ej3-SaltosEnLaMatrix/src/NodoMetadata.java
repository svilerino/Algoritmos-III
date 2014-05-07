import java.util.List;

public class NodoMetadata {
	//predecesor
	private Nodo predecesor;
	// potencia del resorte
	private int nodoValue;
	// flag para bfs
	private boolean fueVisitado;
	// Lista de nodos adyacentes
	/**
	 * Tengamos en cuenta que en esta lista hay tuplas(x,y,z), x,y son las
	 * posiciones en el plano z, que indica, la cantidad de powerup disponible.
	 * Haciendo la resta entre el valor level de dos nodos, se calcula el
	 * powerup utilizado para dicho salto
	 */
	private List<Nodo> alcanzables;

	public int getNodoValue() {
		return nodoValue;
	}	

	public boolean fueVisitado() {
		return fueVisitado;
	}

	public void marcarVisitado() {
		this.fueVisitado = true;
	}

	public List<Nodo> getAlcanzables() {
		return alcanzables;
	}

	public NodoMetadata(int nodoValue, boolean fueVisitado, List<Nodo> alcanzables, Nodo predecesor) {
		super();
		this.nodoValue = nodoValue;
		this.fueVisitado = fueVisitado;
		this.alcanzables = alcanzables;
		this.predecesor = predecesor;
	}

	public Nodo getPredecesor() {
		return predecesor;
	}

	public void setPredecesor(Nodo predecesor) {
		this.predecesor = predecesor;
	}
	
}