public class Nodo {
	//posicion x en el grafo
	private int x;
	
	//posicion y en el grafo
	private int y;
	
	//posicion z en el grafo
	private int level;

	public Nodo(int x, int y, int level) {
		super();
		this.x = x;
		this.y = y;
		this.level = level;
	}	
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + level;
		result = prime * result + x;
		result = prime * result + y;
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Nodo other = (Nodo) obj;
		if (level != other.level)
			return false;
		if (x != other.x)
			return false;
		if (y != other.y)
			return false;
		return true;
	}

	public int getX() {
		return x;
	}

	public void setX(int x) {
		this.x = x;
	}

	public int getY() {
		return y;
	}

	public void setY(int y) {
		this.y = y;
	}

	public int getLevel() {
		return level;
	}

	public void setLevel(int level) {
		this.level = level;
	}

	public String toString(){
		StringBuilder sb = new StringBuilder();
		sb.append("(");
		sb.append(getX());
		sb.append(", ");
		sb.append(getY());
		sb.append(", ");
		sb.append(getLevel());
		sb.append(")");
		return sb.toString();
	}
	
}
