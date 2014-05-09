package parseo;
import java.io.*;

public class ParseoATxt {
	
	private String linea_a_string(Linea l){
		
		String s="";
		int i = l.num_elem();
		while(i>0){
			if(s==""){
				s = s + l.dame_elem(l.num_elem()-i);
			}
			else{
				s = s + " " + l.dame_elem(l.num_elem()-i);	
			}

			i--;
		}
		
		return s;
	}

	private Parseo origen;
	private String destino;
	
	public void set_origen(Parseo orig){
		origen = orig;
	}
	
	public void set_destino(String dest){
		destino = dest;
	}
	
	public void convertir() throws IOException{
		
		int i = origen.num_lin();
		
		PrintWriter out = new PrintWriter(
		        new BufferedWriter(new FileWriter(destino)));
		
		while(i>0){
			out.println(linea_a_string(origen.dame_lin(origen.num_lin()-i)));
			i--;
		}
			
		out.close();
	}
}