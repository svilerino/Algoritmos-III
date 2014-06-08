#/bin/sh
reset
pushd src
rm -rf *.runnable
rm -rf *.o

printf "Compilando java nativo con gcj..."
gcj -ggdb -c *.java

objcopy -L '_ZGr8_$_dummy' Nodo.o
objcopy -L '_ZGr8_$_dummy' NodoMetadata.o
objcopy -L '_ZGr8_$_dummy' Juego.o
objcopy -L '_ZGr8_$_dummy' Main.o

gcj --main=Main *.o -o app.runnable
rm -rf *.o
echo "Ok"


printf "Corriendo ../tests/test.mejorcaso ; stdout > ../testsout/test.mejorcasoparticular ; stderr > ../timingout/test.mejorcasoparticular..."
./app.runnable --take-time 100 < ../tests/test.mejorcaso > ../testsout/test.mejorcasoparticular	2> ../timingout/test.mejorcasoparticular
echo "Ok"

printf "Corriendo ../tests/test.casopromedio ; stdout > ../testsout/test.casopromedioparticular ; stderr > ../timingout/test.casopromedioparticular..."
./app.runnable --take-time 100 < ../tests/test.casopromedio > ../testsout/test.casopromedioparticular	2> ../timingout/test.casopromedioparticular
echo "Ok"

printf "Corriendo ../tests/test.peorcaso ; stdout > ../testsout/test.peorcasoparticular ; stderr > ../timingout/test.peorcasoparticular..."
./app.runnable --take-time 100 < ../tests/test.peorcaso > ../testsout/test.peorcasoparticular	2> ../timingout/test.peorcasoparticular
echo "Ok"

#concateno los resultados
printf "Concatenando y ordenando los resultados en ../timingout/complete.casosparticulares.timingout..."
cat ../timingout/test.mejorcasoparticular ../timingout/test.casopromedioparticular ../timingout/test.peorcasoparticular > ../timingout/casosparticulares.timingout
echo "Ok"

printf "Leyendo ../timingout/casosparticulares.timingout... y guardando grafico en ../timingout/casosparticulares.timingout.png..."
	python ../plotter.py ../timingout/casosparticulares.timingout ../timingout/casosparticulares.timingout 3
echo "Ok"		



						#test correctitud.