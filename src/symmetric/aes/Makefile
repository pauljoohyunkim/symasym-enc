CC=gcc

test: test.o aes.o
	$(CC) test.o aes.o -o test -g

test.o: test.c
	$(CC) test.c -c -g -o test.o

aesenc: aesenc.o aes.o
	$(CC) aesenc.o aes.o -o aesenc -g

aesenc.o: aesenc.c
	$(CC) aesenc.c -c -g -o aesenc.o

aes.o: aes.c aes.h
	$(CC) aes.c -c -g -o aes.o

clean: aesenc
	rm aesenc
	rm test
	rm *.o
