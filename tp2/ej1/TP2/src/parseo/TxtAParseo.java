package parseo;
import java.io.*;

public class TxtAParseo {
	
	private Linea string_a_linea(String s){
		Linea l = new Linea();
		
		if(s.length()==0){
			return l;
		}
		else{
		
		int i = 0;
		int j = 0;
		
		while(j<=(s.length()-2)){
			if(s.charAt(j)==' '){
				l.agregar_elem(s.substring(i,j));
				j++;
				i=j;
			}
			else{
			j++;	
			}
		}
		
		if (s.charAt(j)==' '){
			l.agregar_elem(s.substring(i,j));
			l.agregar_elem("");
		}
		else{
			l.agregar_elem(s.substring(i,j+1));
		}
		
		return l;
		
		}
	}

	private String origen;
	
	public void set_origen(String orig){
		String copia = orig;
		origen = copia;
	}
	
	public Parseo convertir() throws IOException{
		
		Parseo ret = new Parseo();
		
	    BufferedReader in = new BufferedReader(
	      new FileReader(origen));
	    String s = new String();
	    
	    while((s = in.readLine())!= null){
	    	Linea lin = new Linea();
	    	lin = string_a_linea(s);
	    	ret.agregar_lin(lin);
	    }
	    
	    in.close();
	    
		return ret;
	}
}
