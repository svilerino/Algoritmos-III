import java.util.List;

public class NodoMetadata {
	//predecesor en el grafo
	private Nodo predecesor;
	
	// potencia del resorte
	private int nodoValue;
	
	// flag para bfs
	private boolean fueVisitado;
	
	// Lista de nodos adyacentes	
	private List<Nodo> alcanzables;

	public NodoMetadata(int nodoValue, boolean fueVisitado, List<Nodo> alcanzables, Nodo predecesor) {
		super();
		this.nodoValue = nodoValue;
		this.fueVisitado = fueVisitado;
		this.alcanzables = alcanzables;
		this.predecesor = predecesor;
	}
	
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
	public Nodo getPredecesor() {
		return predecesor;
	}

	public void setPredecesor(Nodo predecesor) {
		this.predecesor = predecesor;
	}
	
}