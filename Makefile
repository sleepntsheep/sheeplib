test: test/test.c
	$(CC) $^ -o test/bin -std=c11 $(CFLAGS) $(LDFLAGS)
	./test/bin
