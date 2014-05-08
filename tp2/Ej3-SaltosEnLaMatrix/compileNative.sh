#/bin/sh

pushd src
rm -rf *.runnable
rm -rf *.o
rm -rf ../tests/*.testcase
rm -rf ../tests/*.txt

gcj -ggdb -c *.java

objcopy -L '_ZGr8_$_dummy' Nodo.o
objcopy -L '_ZGr8_$_dummy' NodoMetadata.o
objcopy -L '_ZGr8_$_dummy' Juego.o
objcopy -L '_ZGr8_$_dummy' Main.o

gcj --main=Main *.o -o app.runnable
echo "todo compilado"

# --generate-tests randMinDimension randMaxDimension randMinPowerUp randMaxPowerUp

echo "generando input1.testcase"
./app.runnable --generate-tests 2 4 0 3 > ../tests/input1.testcase
./app.runnable --take-time 100000 < ../tests/input1.testcase

echo "generando input2.testcase"
./app.runnable --generate-tests 2 4 0 3 > ../tests/input2.testcase
./app.runnable --take-time 100000 < ../tests/input2.testcase

echo "generando input3.testcase"
./app.runnable --generate-tests 2 4 0 3 > ../tests/input3.testcase
./app.runnable --take-time 100000 < ../tests/input3.testcase

echo "generando input4.testcase"
./app.runnable --generate-tests 2 4 0 3 > ../tests/input4.testcase
./app.runnable --take-time 100000 < ../tests/input4.testcase

echo "generando input5.testcase"
./app.runnable --generate-tests 2 4 0 3 > ../tests/input5.testcase
./app.runnable --take-time 100000 < ../tests/input5.testcase
popd