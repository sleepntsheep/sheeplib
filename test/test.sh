#!/bin/sh
for f in *.c
do
    cc "$f" -o "$f.out" ../src/sjson.c -g
done
