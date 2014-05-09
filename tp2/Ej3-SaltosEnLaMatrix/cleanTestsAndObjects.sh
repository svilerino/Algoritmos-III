#/bin/sh

pushd src
rm -rf *.runnable
rm -rf *.o
rm -rf ../tests/*.testcase
rm -rf ../testsout/*.testout
rm -rf ../timingout/*.timingout
popd