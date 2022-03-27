CC=gcc

test.bin: test.o aesbcm.o aes.o
	$(CC) test.o aesbcm.o aes.o -o test.bin -g

test.o: test.c
	$(CC) test.c -c -g -o test.o

aesbcm.o: aesbcm.c
	$(CC) aesbcm.c -c -g -o aesbcm.o

aesenc.bin: aesenc.o aes.o
	$(CC) aesenc.o aes.o -o aesenc.bin -g

aesenc.o: aesenc.c
	$(CC) aesenc.c -c -g -o aesenc.o

aes.o: aes.c aes.h
	$(CC) aes.c -c -g -o aes.o

clean:
	rm *.bin
	rm *.o
	rm testinput.aes
