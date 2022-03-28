CC = gcc
CFLAGS = -g

aesenc.bin: aesenc.o aes.o aesbcm.o hash.o
	$(CC) $(CFLAGS) aesenc.o aes.o aesbcm.o hash.o -lcrypto -o aesenc.bin

aesenc.o: aesenc.c aesenc.h
	$(CC) $(CFLAGS) aesenc.c -c

aes.o: aes.c aes.h
	$(CC) $(CFLAGS) aes.c -c

aesbcm.o: aesbcm.c aesbcm.h
	$(CC) $(CFLAGS) aesbcm.c -c

hash.o: ../../misc/hash.c ../../misc/hash.h
	$(CC) $(CFLAGS) ../../misc/hash.c -c -o hash.o

test.bin : test.o aes.o
	$(CC) $(CFLAGS) test.o aes.o -o test.bin

test.o: test.c
	$(CC) $(CFLAGS) test.c -c

clean:
	rm *.bin *.o
	#rm testinput.aes
