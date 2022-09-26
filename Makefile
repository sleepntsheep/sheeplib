all: build run

build: test/test.c
	$(CC) $^ -o test/bin -std=c11 -ggdb -O0

run: test/bin
	./test/bin

