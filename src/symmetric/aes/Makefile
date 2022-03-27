CC=gcc

aesenc.bin: aesenc.o aes.o aesbcm.o hash.o
	$(CC) aesenc.o aes.o aesbcm.o hash.o -g -lcrypto -o aesenc.bin

aesenc.o: aesenc.c aesenc.h
	$(CC) aesenc.c -c -g

aes.o: aes.c aes.h
	$(CC) aes.c -c -g

aesbcm.o: aesbcm.c aesbcm.h
	$(CC) aesbcm.c -c -g

hash.o: ../../misc/hash.c ../../misc/hash.h
	$(CC) ../../misc/hash.c -c -g -o hash.o

clean:
	rm *.bin
	rm *.o
	#rm testinput.aes
