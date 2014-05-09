#/bin/sh

pushd src
rm -rf *.runnable
rm -rf *.o
rm -rf ../tests/*.testcase
rm -rf ../testsout/*.testout
rm -rf ../timingout/*.timingout

gcj -ggdb -c *.java

objcopy -L '_ZGr8_$_dummy' Nodo.o
objcopy -L '_ZGr8_$_dummy' NodoMetadata.o
objcopy -L '_ZGr8_$_dummy' Juego.o
objcopy -L '_ZGr8_$_dummy' Main.o

gcj --main=Main *.o -o app.runnable
echo "todo compilado"

# --generate-tests randMinDimension randMaxDimension randMinPowerUp randMaxPowerUp
for i in {1..5}
do
	echo "generando test$i.testcase"
	./app.runnable --generate-tests 2 4 0 3 > ../tests/test$i.testcase
	echo "corriendo ../tests/test$i.testcase en ../testsout/test$i.testout"
	./app.runnable --take-time 100000 < ../tests/test$i.testcase > ../testsout/test$i.testout	2> ../timingout/test$i.timingout
done

#concateno los resultados
cat ../timingout/*.timingout > ../timingout/tmp_plot.out
#ordeno plot por la primer columna de numeros 
sort ../timingout/tmp_plot.out -k1,1 --numeric-sort > ../timingout/complete.timingout
rm -f ../timingout/tmp_plot.out

popd