#!/bin/sh
for f in *.c
do
    cc "$f" -o "$f.out" -g3 -fsanitize=address,undefined 
done
export ASAN_OPTIONS=abort_on_error=1
