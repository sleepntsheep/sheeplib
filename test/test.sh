#!/bin/sh

rm -rf bin
mkdir -p bin
for f in *.c
do
    gcc "$f" -o "./bin/$f.out"  -DSNOW_ENABLED -std=c11 -pedantic -Wall
    "./bin/$f.out"
done

for f in *.cpp
do 
    g++ "$f" -o "./bin/$f.out" -std=c++11 -pedantic -Wall
done
