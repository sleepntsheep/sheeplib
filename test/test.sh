#!/bin/sh

rm -rf bin
mkdir -p bin
for f in *.c
do
    gcc "$f" -o "./bin/$f.out"  -lcriterion -lcheck -g
    "./bin/$f.out"
done
