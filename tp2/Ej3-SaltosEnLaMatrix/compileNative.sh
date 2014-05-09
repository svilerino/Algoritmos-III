#/bin/sh
reset
./cleanTestsAndObjects.sh

pushd src

printf "Compilando java nativo con gcj..."
gcj -ggdb -c *.java

objcopy -L '_ZGr8_$_dummy' Nodo.o
objcopy -L '_ZGr8_$_dummy' NodoMetadata.o
objcopy -L '_ZGr8_$_dummy' Juego.o
objcopy -L '_ZGr8_$_dummy' Main.o

gcj --main=Main *.o -o app.runnable
rm -rf *.o
echo "Ok"

for powerup in {0..5}
do
	for dim in {2..100}
	do
		printf "Generando test$dim.$powerup.testcase..."
		./app.runnable --generate-tests "$dim" "$powerup" > ../tests/test$dim.$powerup.testcase
		echo "Ok"
		
		printf "Corriendo ../tests/test$dim.$powerup.testcase ; stdout > ../testsout/test$dim.$powerup.testout ; stderr > ../timingout/test$dim.$powerup.timingout..."
		./app.runnable --take-time 100 < ../tests/test$dim.$powerup.testcase > ../testsout/test$dim.$powerup.testout	2> ../timingout/test$dim.$powerup.timingout
		echo "Ok"
	done
	#concateno los resultados
	printf "Concatenando y ordenando los resultados en ../timingout/complete.powerup_$powerup.timingout..."
	cat ../timingout/*."$powerup".timingout > ../timingout/tmp_plot.out
	#ordeno plot por la primer columna de numeros 
	sort ../timingout/tmp_plot.out -k1,1 --numeric-sort > ../timingout/complete.powerup_"$powerup".timingout
	rm -f ../timingout/tmp_plot.out
	echo "Ok"

	for graphicType in {0..2}
	do
		#"Use with parameters: plotter.py <outputPNGPath> <inputDataFile of pairs (x, y, z = f(x,y)) separated by space>"
		printf "Leyendo ../timingout/complete.powerup_$powerup.timingout... y guardando grafico en ../timingout/complete.timingout.powerup_$powerup..."
			python ../plotter.py ../timingout/complete.powerup_"$powerup" ../timingout/complete.powerup_"$powerup".timingout "$graphicType"
		echo "Ok"		
	done
done

popd