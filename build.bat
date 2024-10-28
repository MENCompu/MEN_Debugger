@echo off

IF NOT EXIST bin mkdir bin
pushd bin
gcc ..\src\WindowsLayer.c -o menc_debugger.exe
popd

IF NOT EXIST test\bin mkdir test\bin
pushd test\bin
gcc ..\src\test.cpp -g -o test.exe
popd
