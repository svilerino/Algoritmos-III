#include <iostream>     // std::cout
#include <algorithm>    // std::min
#include <cstdlib>      // atoi

using namespace std;

int main(int argc, char** argv){
	if(argc != 4){
		cout << "Use: min <a> <b> <c>" << endl;
		return 1;
	}else{
		int a = atoi(argv[1]);
		int b = atoi(argv[2]);
		int c = atoi(argv[3]);
		int minimo = min(a, min(b, c));
		cout << minimo;
		return 0;
	}
}