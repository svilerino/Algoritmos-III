package parseo;

public class Linea {
	
	private class Nodo{
		public String s;
		public Nodo sig;
		public Nodo ant;
		
		Nodo(){
			s="";
			sig=null;
			ant=null;
		}
	}
	
	private int numelem;
	private Nodo primero;
	private Nodo ultimo;
	
	public Linea(){
		numelem = 0;
		primero = null;
		ultimo=null;
	} 
	
	public int num_elem(){
		int ret = numelem;
		return ret;
	}
	
	public String dame_elem(int i){
		if(numelem<=i){
			return "";
		}
		else{
			Nodo iterador = primero;
			while(i>0){
				iterador = iterador.sig;
				i--;
			}
			return iterador.s;
		}
	}
	
	public void agregar_elem(String elem){
		if(ultimo==null){
			ultimo = new Nodo();
			primero = ultimo;
			ultimo.sig=null;
			ultimo.ant=null;
			ultimo.s=elem;
			numelem++;
		}
		else{
			ultimo.sig = new Nodo();
			ultimo.sig.sig=null;
			ultimo.sig.ant=ultimo;
			ultimo = ultimo.sig;
			ultimo.s = elem;
			numelem++;
		}
		
	}
}
