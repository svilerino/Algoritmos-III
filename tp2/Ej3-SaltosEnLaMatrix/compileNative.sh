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

for dim in {2..50}
do
	for powerup in {1..0}
	do
		printf "Generando test$dim.$powerup.testcase..."
		./app.runnable --generate-tests "$dim" "$powerup" > ../tests/test$dim.$powerup.testcase
		echo "Ok"
		
		printf "Corriendo ../tests/test$dim.$powerup.testcase ; stdout > ../testsout/test$dim.$powerup.testout ; stderr > ../timingout/test$dim.$powerup.timingout..."
		./app.runnable --take-time 100 < ../tests/test$dim.$powerup.testcase > ../testsout/test$dim.$powerup.testout	2> ../timingout/test$dim.$powerup.timingout
		echo "Ok"
	done
done

#concateno los resultados
printf "Concatenando y ordenando los resultados en ../timingout/complete/timingout.timingout..."
cat ../timingout/*.timingout > ../timingout/tmp_plot.out
#ordeno plot por la primer columna de numeros 
sort ../timingout/tmp_plot.out -k1,1 --numeric-sort > ../timingout/complete.timingout
rm -f ../timingout/tmp_plot.out
echo "Ok"

popd