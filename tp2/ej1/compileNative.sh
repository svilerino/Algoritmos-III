#/bin/sh
reset
./cleanTestsAndObjects.sh

pushd src

printf "Compilando java nativo con gcj..."
gcj -ggdb -c *.java

objcopy -L '_ZGr8_$_dummy' Main.o
objcopy -L '_ZGr8_$_dummy' Main.o

gcj --main=Main *.o -o app.runnable
rm -rf *.o
echo "Ok"

for cardsNumber in {10..200}
do
	
	printf "Generando test$cardsNumber.testcase..."
	./app.runnable --generate-tests "$cardsNumber" -100 100 > ../tests/test$cardsNumber.testcase
	echo "Ok"
	
	printf "Corriendo ../tests/test$cardsNumber.testcase ; stdout > ../testsout/test$cardsNumber.testout ; stderr > ../timingout/test$cardsNumber.timingout..."
	./app.runnable --take-time 100 < ../tests/test$cardsNumber.testcase > ../testsout/test$cardsNumber.testout	2> ../timingout/test$cardsNumber.timingout
	echo "Ok"
done

#concateno los resultados
printf "Concatenando y ordenando los resultados en ../timingout/complete.timingout..."
cat ../timingout/*.timingout > ../timingout/tmp_plot.out

#ordeno plot por la primer columna de numeros 
sort ../timingout/tmp_plot.out -k1,1 --numeric-sort > ../timingout/complete.timingout
rm -f ../timingout/tmp_plot.out
echo "Ok"

for graphicType in {0..2}
do
	#"Use with parameters: plotter.py <outputPNGPath> <inputDataFile of pairs (x, y, z = f(x,y)) separated by space>"
	printf "Leyendo ../timingout/complete.timingout... y guardando grafico en ../timingout/complete.timingout.png..."
		python ../plotter.py ../timingout/complete.timingout.$graphicType ../timingout/complete.timingout "$graphicType"
	echo "Ok"		
done

popd