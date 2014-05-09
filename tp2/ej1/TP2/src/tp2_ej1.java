import java.io.IOException;

import parseo.*;


public class tp2_ej1 {

	public static void main(String[] args) throws IOException{
		/*
		//Toma la entrada de un archivo de texto
		TxtAParseo conv = new TxtAParseo();
		conv.set_origen("C:/Documents and Settings/Administrador/Escritorio/entrada.txt");
		Parseo p_entrada = conv.convertir();
		
		
		//Pasa la entrada a array
		int a_entrada[] = new int[p_entrada.dame_lin(0).num_elem()-1];
		int i;
		for(i=1;i<=p_entrada.dame_lin(0).num_elem()-1;i++){
			a_entrada[i-1]=Integer.parseInt(p_entrada.dame_lin(0).dame_elem(i));
		}
		*/
		
		/*
		//Corre el algoritmo
		double t1_com = System.currentTimeMillis();
		algo_robanum algoritmo = new algo_robanum();
		algoritmo.calcular(a_entrada);
		double t2_com = System.currentTimeMillis();
		*/
		Parseo p_salida = new Parseo();
		Parseo p_salida2 = new Parseo();
		
		int j;
		for(j=100;j<=400;j++){
			int a_entrada[] = new int[j];
			int i;
			for(i=1;i<=j;i++){
				a_entrada[i-1] = (int) Math.floor(Math.random()*100);
			}
			double t1_com = System.currentTimeMillis();
			algo_robanum algoritmo = new algo_robanum();
			algoritmo.calcular(a_entrada);
			double t2_com = System.currentTimeMillis();
			
			Linea lin = new Linea();
			lin.agregar_elem(Integer.toString(j));
			p_salida.agregar_lin(lin);
			lin = new Linea();
			lin.agregar_elem(Integer.toString((int)(t2_com-t1_com)));
			p_salida2.agregar_lin(lin);
		}
		/*
		//Parsea la salida
		Parseo p_salida = new Parseo();
		Linea lin = new Linea();
		lin.agregar_elem(Double.toString(t2_com - t1_com));
		p_salida.agregar_lin(lin);
		lin = new Linea();
		lin.agregar_elem(Integer.toString(algoritmo.ret_turnos_jug()));
		lin.agregar_elem(Integer.toString(algoritmo.ret_pt_j1()));
		lin.agregar_elem(Integer.toString(algoritmo.ret_pt_j2()));
		p_salida.agregar_lin(lin);
		
		for(i=1;i<=algoritmo.ret_turnos_jug();i++){
			lin = new Linea();
			if(algoritmo.ret_turnos_lado(i)==true){
				lin.agregar_elem("izq");
			}
			else{
				lin.agregar_elem("der");
			}
			lin.agregar_elem(Integer.toString(algoritmo.ret_turnos_cant_rob(i)));
			p_salida.agregar_lin(lin);
		}
		*/
		//Escribe el parseo de salida en un texto
		ParseoATxt conv2 = new ParseoATxt();
		conv2.set_origen(p_salida);
		conv2.set_destino("C:/Documents and Settings/Administrador/Escritorio/salida.txt");
		conv2.convertir();
		
		conv2.set_origen(p_salida2);
		conv2.set_destino("C:/Documents and Settings/Administrador/Escritorio/salida2.txt");
		conv2.convertir();
	}

}
