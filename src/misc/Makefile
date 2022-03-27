CC=gcc

test: test.c hash.c hash.h
	$(CC) test.c hash.c -o test -g -lcrypto

clean: test
	rm test