package parseo;

import java.io.IOException;

public class pruebas {
	
	public static void ImprimeLinea(Linea lin){
		int i;
		
		for(i=1 ; i<=lin.num_elem() ; i++){
			System.out.print(lin.dame_elem(i-1));
			System.out.print(" ");
		}
	}
	
	public static void ImprimeParseo(Parseo par){
		int i;
		
		for(i=1 ; i<=par.num_lin() ; i++){
			ImprimeLinea(par.dame_lin(i-1));
			System.out.println();
		}
	}

	
	public static void main(String[] args) throws IOException{
		
		TxtAParseo conv = new TxtAParseo();
		conv.set_origen("C:/Documents and Settings/Administrador/workspace/parseo/bin/parseo/hola.txt");
		Parseo salida = conv.convertir();
		ImprimeParseo(salida);
		
		ParseoATxt conv2 = new ParseoATxt();
		conv2.set_origen(salida);
		conv2.set_destino("C:/Documents and Settings/Administrador/workspace/parseo/bin/parseo/salida.txt");
		conv2.convertir();
	}
}
