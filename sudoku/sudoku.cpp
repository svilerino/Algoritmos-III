#include <list>
#include <utility>
#include <vector>
#include <iostream>
using namespace std;

//juez online http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=11&page=show_problem&problem=930

// Varios typedefs
typedef vector<int> Vec;
typedef vector<Vec> Tablero;
typedef pair<int, int> Coord;
typedef list<Coord> LCoord;

// Prototipado de funciones
bool resolver(Tablero& p, int nsubs);
void mostrar(const Tablero& m, bool esSolucion);
int check(const Tablero& c, int n);
bool algunRepetido(const Vec &v);

bool backtrack(Tablero& p, int n, int i, int j);
void primerCero(int start_x, int start_y, int& target_x, int& target_y, int n, Tablero p);

/**
 * La función main toma el tablero por std input:
 * recibe primero el tamaño de bloque (si se le pasa n se
 * asume un bloque de n*n, con lo cual la matriz final será
 * de (n*n)*(n*n)). Luego recibe un numero por cada casillero
 * del tablero. Un cero indica casilla vacia.
 * Finalmente, llama al método resolver(...) y al finalizar éste
 * se muestra el resultado (de haberlo).
 */
int main()
{
	int n;

	while(true) {
		cin >> n;
		if(cin.eof()){
			break;
		}
		int n2 = n*n;
		Tablero p(n2, Vec(n2, 0));
		for(int i = 0; i < n2; ++i)
			for(int j = 0; j < n2; ++j)
				cin >> p[i][j];
			
		cout << "Tablero a resolver:" << endl;
		mostrar(p, false);
		bool sol = resolver(p,n);
		if(!sol){
			cout << endl << "El tablero no tene solucion...";
		} else {
			cout << endl << "Sudoku solucionado:" << endl;
			mostrar(p, true);
		}
		cout << endl;
	}

	return 0;
}

/**
 * Recibe el tablero inicial y el tamaño de bloque y fabrica
 * una lista con las casillas vacias del tablero.
 * Luego llama a la función backtrack(...) e informa e resultado.
 */
bool resolver(Tablero& p, int n)
{
	//busco el primer cero en el tablero para realizar la llamada inicial
	int i = 0;
	int j = 0;
	primerCero(0,0, i, j, n, p);
	return(backtrack(p, n, i, j));
}

void primerCero(int start_x, int start_y, int& target_x, int& target_y, int n, Tablero p){
	//busco los ceros del tablero y les aplico backtrack
	int n2 = n*n;
    int i = start_x;
    int j = start_y;
	for(i = start_x; i < n2; i++){
		for(j = start_y; j < n2; j++){
			if(p[i][j] == 0){
				target_x = i;
				target_y = j;
				return;				
			}
		}
	}
}

/**
	p tablero por referencia
	n*n dimension del tablero
	x_start coordenada x del primer cero disponible en el tablero 
	y_start coordenada y del primer cero disponible en el tablero
*/
bool backtrack(Tablero& p, int n, int x_start, int y_start){
	/**
	 * La siguiente función devuelve:
	 *    0 --> Si no tiene repetidos pero le faltan casillas por llenar
	 *    1 --> Si no tiene repetidos y es una solucion de Sudoku
	 *    2 --> Si el tablero tiene repetidos en alguna fila, columna o cuadrante
	 */
	switch(check(p, n)){
		case 0:
			//es solucion parcial, pero no completa, sigo la ejecucion de la funcion
			break;
		case 1:
			//es solucion parcial, y esta completa, es solucion total, devuelvo true, caso base de la recursion.
			return true;
		case 2:
			//no es solucion parcial, rechazar toda esta rama de ejecución.
			return false;
	}

	//en este punto vale que p es solucion parcial pero no completa,
	//me fijo para todos los valores entre i=1..9 si puede completarse con el y ser solucion
	    
	//en (x_start, y_start) tengo la primer casilla con cero
	//en caso que la llamada con la posible solucion parcial falle, me guardo un backup de lo que hay en esa posicion.
	int aux = p[x_start][y_start];
	
	//pongo un valor dummy para que la busqueda del cero abajo no tome las coordenadas (x_start, y_start).
	p[x_start][y_start] = 11;

	//en caso de ser solucion parcial, necesito pasarle a la llamada recursiva el proximo lugar con un cero
	//busco el primer cero en el tablero para realizar la llamada en esa posicion
	int pos_cero_x = 0;
	int pos_cero_y = 0;
	primerCero(x_start, y_start, pos_cero_x, pos_cero_y, n, p);

	//hago las llamadas recursivas para los posibles valores entre [1..9]
	int candidatoNumerico = 1;
	do{
		//asigno un valor como posible solucion parcial
		p[x_start][y_start] = candidatoNumerico;
		//doy un paso recursivo con la posible solucion parcial
		bool result = backtrack(p, n, pos_cero_x, pos_cero_y);
		if(!result){
			//si no era solucion, deshago los cambios e intento con otro valor 
			p[x_start][y_start] = aux;
		}else{
			return true;
		}
		candidatoNumerico++;
	}while(candidatoNumerico<=9);
}

/**
 * Recibe un tablero y lo muestra por pantalla
 */
void mostrar(const Tablero& m, bool esSolucion)
{
	cout << endl;
	int n = m.size();
	for(int i = 0; i < n; ++i)
	{
		//\033[1;31mbold		
		for (int j = 0; j < n; j++)
		{
			if(esSolucion)
				cout << " \033[1;31m" << m[i][j];
			else
				cout << " " << m[i][j];
		}
		cout << "\033[0;0m"<< endl;
	}
}


/**
 * La siguiente función devuelve:
 *    2 --> Si el tablero tiene repetidos en alguna fila, columna o cuadrante
 *    1 --> Si no tiene repetidos y es una solucion de Sudoku
 *    0 --> Si no tiene repetidos pero le faltan casillas por llenar
 */
int check(const Tablero& p, int n)
{
	int n2 = p.size();
	bool ceros = false;
	for (int i = 0; i < n2; i++)
	{
		// verifico la fila i
		Vec flag_fila (n2, 0);
		for (int j = 0; j < n2; j++)
		{
			if (p[i][j] == 0)
				ceros = true;
			else
				flag_fila[p[i][j]-1]++;
		}

		if (algunRepetido(flag_fila))
			return 2;

		// verifico la columna i
		Vec flag_col (n2, 0);
		for (int j = 0; j < n2; j++)
		{
			if (p[j][i] == 0)
				ceros = true;
			else
				flag_col[p[j][i]-1]++;
		}

		if (algunRepetido(flag_col))
			return 2;

		// verifico el cuadrante i
		Vec flag_cuad(n2, 0);
		int f = (i/n)*n; // fila donde empieza el cuadrante
		int c = (i % n)*n; // col donde empieza el cuadrante
		for (int j = 0; j < n; j++)
		for (int k = 0; k < n; k++)
		{
			if (p[f+j][c+k] == 0)
				ceros = true;
			else
				flag_cuad[p[f+j][c+k]-1]++;
		}

		if (algunRepetido(flag_cuad))
			return 2;
	}

	return ceros? 0 : 1;
}

/**
 * Método auxiliar del método check
 */
bool algunRepetido(const Vec& v)
{
	int k = v.size();
	for (int i = 0; i < k; i++)
		if (v[i] > 1)
			return true;

	return false;
}

