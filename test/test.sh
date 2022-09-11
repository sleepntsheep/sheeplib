#!/bin/sh
for f in *.c
do
    cc -g "$f" -o "$f.out"
done
