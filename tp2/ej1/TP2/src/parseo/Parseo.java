package parseo;

public class Parseo {
	
	private class Nodo{
		public Linea l;
		public Nodo sig;
		public Nodo ant;
		
		Nodo(){
			l = new Linea();
			sig=null;
			ant=null;
		}
	}
	
	private int numlin;
	private Nodo primero;
	private Nodo ultimo;
	
	public Parseo(){
		numlin = 0;
		primero = null;
		ultimo=null;
	} 
	
	public int num_lin(){
		return numlin;
	}
	
	public Linea dame_lin(int i){
		if(numlin<=i){
			return new Linea();
		}
		else{
			Nodo iterador = primero;
			while(i>0){
				iterador = iterador.sig;
				i--;
			}
			return iterador.l;
		}
	}
	
	public void agregar_lin(Linea lin){
		if(ultimo==null){
			ultimo = new Nodo();
			primero = ultimo;
			ultimo.sig=null;
			ultimo.ant=null;
			ultimo.l=lin;
			numlin++;
		}
		else{
			ultimo.sig = new Nodo();
			ultimo.sig.sig=null;
			ultimo.sig.ant=ultimo;
			ultimo = ultimo.sig;
			ultimo.l = lin;
			numlin++;
		}
		
	}
}